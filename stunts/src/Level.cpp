/* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup,
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Christian Morbach <GameDevelopment@paratronic.de>
 *                    Art Tevs <tevs@mpi-sb.mpg.de>
 *                    Andreas Maurer <andi@andile.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
 * USA.
 */

#include "Level.hpp"
#include "Utils.hpp"
#include "OgreTask.hpp"
#include "CKI.h"

#include "ObjectInstantiator.hpp"
#include <math.h>

using boost::shared_ptr;

namespace stunts
{

	//--------------------------------------------------------------------------
	CLevel::CLevel()
	{

		// Setup default grid count size
		mGridCountInX = 30;
		mGridCountInZ = 30;

		//reset physics members
		mGravity = 9.80665;
		mCFM = 10e-5;
		mERP = 0.8;
		mCCV = 1.0;

		mTerrainFriction = 18.0;

		mLevelNode = COgreTask::GetSingleton().mSceneMgr->getRootSceneNode()->createChildSceneNode("Level_Node");

		//(re-)set the Physics World
		if (!(OgreOde::World::getSingletonPtr()))
			mPhysicsWorld = new OgreOde::World(COgreTask::GetSingleton().mSceneMgr);
		else
			mPhysicsWorld = OgreOde::World::getSingletonPtr();

	}


	//--------------------------------------------------------------------------
	CLevel::~CLevel()
	{

		// delete the input
		mUserInput.reset();

		// delete all waypoints
		for (unsigned int i=0; i < mWaypoints.size(); i++)
		{
			mWaypoints[i].reset();
		}


		// First we need to delete all used objects
		for (unsigned int i=0; i < mObjects.size(); i++)
		{
			(mObjects[i]).reset();
		}

		// now remove all cars
		mHumanCar.reset();
		mAICar.reset();

		// Now remove terrain and atmosphere
		mTerrain.reset();
		mAtmosphere.reset();


		// Now clear the rest of the scene
		//COgreTask::GetSingleton().mSceneMgr->clearScene();
		if (mLevelNode)
		{
			try {
				mLevelNode->detachAllObjects();
				COgreTask::GetSingleton().mSceneMgr->getRootSceneNode()->removeAndDestroyChild(mLevelNode->getName());
				mLevelNode = NULL;
			}catch(...){

			}
		}

		//clear the collision geometry (and unregister it from ODE)
		mCollisionGeometry.reset();

		//COgreTask::GetSingleton().mSceneMgr->clearScene();
	}


	//--------------------------------------------------------------------------
	bool CLevel::loadLevel(const std::string& levelFile)
	{

		nrLog.Log(NR_LOG_APP, "CLevel::loadLevel(): Start loading of a level from file \"%s\"", levelFile.c_str());

		// we open the file for parsing.
		// Later we can open the file through the virtual file system if we had got more time
		// for development
		shared_ptr<TiXmlDocument> mLevelDoc (new TiXmlDocument(levelFile.c_str()));
		if (!mLevelDoc->LoadFile())
		{
			nrLog.Log(NR_LOG_APP, "CLevel::loadLevel(): There is an error occurs by loading level specification file \"%s\"", levelFile.c_str());
			return true;
		}

		// Load elements form the level file and handle with them in according way
		TiXmlElement* elem = NULL;
		TiXmlElement* rootElem = mLevelDoc->FirstChildElement("level");

		// check if the root element is correct one
		if (!rootElem)
		{
			nrLog.Log(NR_LOG_APP, "CLevel::loadLevel(): Level file containing not valid data");
			return true;
		}

		// get the path from the file name
		mLevelFilePath = getPathFromFileName(levelFile);
		mLevelFileName = levelFile;

		// remove all objects that we are have got at now
		mObjects.clear();

        // Get the physics values for this level
		elem = rootElem->FirstChildElement("physics");
		if (elem)
			readPhysics(elem);

		// initialize ODE (after the global level settings and
		//	before the terrain or the objects)
		InitializeODE();

		// check for terrain file and get its friction
		elem = rootElem->FirstChildElement("terrain");
		if (elem)
			readTerrain(elem);

		// initialize Ogre (fefore the objects are loaded, so they can get
		//	the terrain height)
		COgreTask::GetSingleton().taskUpdate();

		// check now for object in the file
		elem = rootElem->FirstChildElement("objects");
		if (elem)
			readObjects(elem);


		// move the level node, so we get all objects on the right place
		mLevelNode->translate(unitToMeter(0.5f), 0, unitToMeter(0.5f));


		//create collision geometry out of the objects
		Ogre::Vector3* MeshVertices = NULL;
		int* MeshIndices = NULL;
		int MeshVertexCount = 0;
		int MeshIndexCount = 0;


		//get count
		for (int c=0; c<mObjects.size();c++)
		{
			MeshVertexCount += mObjects.at(c)->mMeshVertexCount;
			MeshIndexCount += mObjects.at(c)->mMeshIndexCount;
		}

		//store geometry
		MeshVertices = new Ogre::Vector3[MeshVertexCount];
		MeshIndices = new int[MeshIndexCount];

		MeshVertexCount = 0;
		MeshIndexCount = 0;

		for (int c=0; c<mObjects.size();c++)
		{
			if (mObjects.at(c)->mMeshVertexCount <= 0)
				continue;

			//copy vertices
			for (int d=0; d<mObjects.at(c)->mMeshVertexCount;d++)
				MeshVertices[d + MeshVertexCount] =
					(mObjects.at(c)->mObjNode->_getFullTransform() * mObjects.at(c)->mMeshVertices[d]);

			//copy indices
			for (int d=0; d<mObjects.at(c)->mMeshIndexCount;d++)
				MeshIndices[d + MeshIndexCount] = (mObjects.at(c)->mMeshIndices[d] + MeshVertexCount);


			MeshVertexCount += mObjects.at(c)->mMeshVertexCount;
			MeshIndexCount += mObjects.at(c)->mMeshIndexCount;
		}

		//attach the vertices
		mCollisionGeometry.reset(
			new OgreOde::TriangleMeshGeometry(MeshVertices, MeshVertexCount,
			MeshIndices, MeshIndexCount, mPhysicsWorld->getDefaultSpace()) );




		// check for the atmosphere
		elem = rootElem->FirstChildElement("atmosphere");
		if (elem)
			readAtmosphere(elem);

		//mIsLoaded = true;

/*		// Erstelle Gitter model
		for (int32 i = 0; i < mGridCountInX; i++)
			for (int32 j=0; j < mGridCountInZ; j++){

				char name[256];
				sprintf(name, "Gitter_%d_%d", i,j);

				/// Create the mesh via the MeshManager
				Ogre::MeshPtr msh = MeshManager::getSingleton().createManual(name, "General");

				/// Set bounding information (for culling)
				Vector3 min = unitsToMeters(i,j);
				Vector3 max = unitsToMeters(i+1,j+1);
				min.y = 10.0f;
				max.y = 10.0f;

				AxisAlignedBox aabb(min, max);
				msh->_setBounds(aabb);

				/// Notify Mesh object that it has been loaded
				msh->load();

				// Now add this into scene graph
				char ename[256];
				sprintf(ename, "%s_entity", name);
				Entity* ent;
				ent 	= COgreTask::GetSingleton().mSceneMgr->createEntity(std::string(ename), std::string(name));
				SceneNode* mSceneNode 	= COgreTask::GetSingleton().mSceneMgr->getRootSceneNode()->createChildSceneNode();
				mSceneNode->attachObject( ent );
				mSceneNode->showBoundingBox(true);
		}
*/
		// Put BaseObject-Waypoints together
		if (buildWaypointPath())
			nrLog.Log(NR_LOG_APP, "CLevel::loadlevel(): Waypoint path successfully built");
		else
			nrLog.Log(NR_LOG_APP, "CLevel::loadlevel(): Unable to build waypoint path");

		// Search for cars
		searchCars();

		return false;
	}




	//--------------------------------------------------------------------------
	void CLevel::drawWaypoint(Ogre::Vector3 pos, int i)
	{
		char name[256];

		sprintf(name, "Waypoint__%i__%f__%f__%f__", i, pos.x,pos.y,pos.z);

		/// Create the mesh via the MeshManager
		Ogre::MeshPtr msh = MeshManager::getSingleton().createManual(name, "General");

		/// Set bounding information (for culling)
		Vector3 min = pos;
		Vector3 max = pos;
		max.y += 100.0f;

		AxisAlignedBox aabb(min, max);
		msh->_setBounds(aabb);

		/// Notify Mesh object that it has been loaded
		msh->load();

		// Now add this into scene graph
		char ename[256];
		sprintf(ename, "%s_entity", name);
		Entity* ent;
		ent 	= COgreTask::GetSingleton().mSceneMgr->createEntity(std::string(ename), std::string(name));
		SceneNode* mSceneNode 	= COgreTask::GetSingleton().mSceneMgr->getRootSceneNode()->createChildSceneNode();
		mSceneNode->attachObject( ent );
		mSceneNode->showBoundingBox(true);
	};




	//--------------------------------------------------------------------------
	void CLevel::drawWaypoint(Ogre::Vector3 min, Ogre::Vector3 max, int i)
	{

		char name[256];
		sprintf(name, "Waypoint2__%i__from__%f__%f__%f__to__%f__%f__%f", i, min.x,min.y, min.z, max.x, max.y, max.z);


		// Create the mesh via the MeshManager
		Ogre::MeshPtr msh = MeshManager::getSingleton().createManual(name, "General");

		// Set bounding information (for culling)
		AxisAlignedBox aabb(min, max);
		msh->_setBounds(aabb);

		// Notify Mesh object that it has been loaded
		msh->load();


		// Now add this into scene graph
		char ename[256];
		sprintf(ename, "%s_entity", name);
		Entity* ent;
		ent 	= COgreTask::GetSingleton().mSceneMgr->createEntity(std::string(ename), std::string(name));
		SceneNode* mSceneNode 	= COgreTask::GetSingleton().mSceneMgr->getRootSceneNode()->createChildSceneNode();
		mSceneNode->attachObject( ent );
		mSceneNode->showBoundingBox(true);

	};




	//--------------------------------------------------------------------------
	void CLevel::readPhysics(TiXmlElement* elem)
	{
		nrLog.Log(NR_LOG_APP, "CLevel::readPhysics(): Read the physics values for the level");

		// The gravity node should contain only the text of a float number
		// We can easely setup the gravity from the string by using the dator class
		nrCDator<float32>	gravity(this->mGravity);
		nrCDator<float32>	cfm(this->mCFM);
		nrCDator<float32>	erp(this->mERP);
		nrCDator<float32>	ccv(this->mCCV);

		// assign string to the gravity value
		gravity = elem->Attribute("gravity");
		cfm = elem->Attribute("CFM");
		erp = elem->Attribute("ERP");
		ccv = elem->Attribute("CCV");
	}
	//--------------------------------------------------------------------------
	void CLevel::readGridsize(TiXmlElement* elem)
	{

		nrCDator<int32>		x(this->mGridCountInX);
		nrCDator<int32>		z(this->mGridCountInZ);

		// assign string to the values
		x = elem->Attribute("x");
		z = elem->Attribute("z");

		nrLog.Log(NR_LOG_APP, "CLevel::readGridsize(): The track use grid of size %dx%d", (int32)x, (int32)z);
	}

	//--------------------------------------------------------------------------
	void CLevel::readAtmosphere(TiXmlElement* elem)
	{
		nrLog.Log(NR_LOG_APP, "CLevel::readAtmosphere(): Read atmosphere node form level file");

		// check for importing of a file
		TiXmlElement*	smElem = elem->FirstChildElement("import");
		if (smElem)
		{
			mAtmosphere.reset(new CAtmosphere());
			mAtmosphere->importFromFile((mLevelFilePath + smElem->Attribute("file")).c_str(), mLevelFilePath);
		}
	}

	//--------------------------------------------------------------------------
	void CLevel::readTerrain(TiXmlElement* elem)
	{
		nrLog.Log(NR_LOG_APP, "CLevel::readTerrain(): Read terrain node from level file");

		// check for importing of a file
		TiXmlElement*	smElem = elem->FirstChildElement("import");
		if (smElem)
		{
			//create terrain (after the engine tasks have been gotten)
			mTerrain.reset (new CTerrain (this));
			mTerrain->importFromFile((mLevelFilePath + smElem->Attribute("file")).c_str(), smElem->Attribute("root"));
		}

		//get its friction
		mTerrainFriction = mTerrain->getFriction();
	}

	//--------------------------------------------------------------------------
	void CLevel::readObjects(TiXmlElement* elem)
	{
		nrLog.Log(NR_LOG_APP, "CLevel::readObjects(): Read objects definition of the level file");

		// Scan for all elements in this node
		for (TiXmlElement* smElem = elem->FirstChildElement("object"); smElem != 0;
					smElem = smElem->NextSiblingElement("object"))
        {

			// if it is an object node, so load data from it
			shared_ptr<CBaseObject>	obj(CObjectInstantiator::createInstance(smElem->Attribute("type")));
			if (obj == NULL){
				nrLog.Log(NR_LOG_APP, "CLevel::readObjects(): Non valid object type \"%s\"", smElem->Attribute("type"));
				break;
			}

			// set the level file
			obj->mLevel = this;

			// Now let the object parse the settings by itself
			if (obj->parseSettings(smElem, mLevelFilePath)){
				nrLog.Log(NR_LOG_APP, "CLevel::readObjects(): Error by parsing object settings");
				break;
			}

			// store the object in a vector
			this->mObjects.push_back(obj);
		}

		// now import a file if one was specified
		TiXmlElement*	smElem = elem->FirstChildElement("import");
		if (smElem)
			importTrackFile ((mLevelFilePath + smElem->Attribute("file")).c_str(), smElem->Attribute("root"));

	}

	//--------------------------------------------------------------------------
	void CLevel::importTrackFile(const char* fileName, const char* root)
	{

		nrLog.Log(NR_LOG_APP, "CLevel::importTrackFile(): Import a track file \"%s\" into the level", fileName);

		// open the document for loading
		shared_ptr<TiXmlDocument>	mDoc (new TiXmlDocument(fileName));
		if (!mDoc->LoadFile())
		{
			nrLog.Log(NR_LOG_APP, "CLevel::importTrackFile(): Can not load the track file \"%s\"", fileName);
			return;
		}

		// get the root element
		TiXmlElement*	elem = NULL;
		TiXmlElement*	rootElem = mDoc->FirstChildElement(root);
		if (rootElem == NULL)
		{
			nrLog.Log(NR_LOG_APP, "CLevel::importTrackFile(): Not valid track file. Can not find root node \"%s\"", root);
			return;
		}

		// Get the grid size for this level
		elem = rootElem->FirstChildElement("gridsize");
		if (elem)
			readGridsize(elem);

		// check now for object in the file
		elem = rootElem->FirstChildElement("objects");
		if (elem)
			readObjects(elem);

		// Now move the terrain, so we get all objects origin in the cells middlepoint

	}

	//--------------------------------------------------------------------------
	Ogre::Vector3 CLevel::unitsToMeters(int32 x, int32 z){

		Ogre::Vector3 vec;

		if (mTerrain != NULL)
		{
			vec.x = (mTerrain->getWidthX() / Ogre::Real(mGridCountInX)) * Ogre::Real(x);
			vec.y = Ogre::Real(0);
			vec.z = (mTerrain->getWidthZ() / Ogre::Real(mGridCountInZ)) * Ogre::Real(z);
		}

		return vec;
	}

	//--------------------------------------------------------------------------
	Ogre::Real CLevel::unitToMeter(int32 x)
	{
		if (mTerrain != NULL)
			return (Terrain()->getWidthX() / float(mGridCountInX)) * float(x);
		else
			return 1.0f;
	}
	//--------------------------------------------------------------------------
	Ogre::Real CLevel::unitToMeter(float32 x){
		return (Terrain()->getWidthX() / float(mGridCountInX)) * x;
	}

	//--------------------------------------------------------------------------
	nrResult CLevel::start()
	{
		//get all tasks and set member attributes
		//getEngineTasks();

		//userInput
		mUserInput.reset (new CUserInput(this));
		mUserInput->setTaskPriority(NR_PRIORITY_VERY_HIGH);
		nrKernel.AddTask(mUserInput);
		nrKernel.StartTask(mUserInput->getTaskID());

		//AI
		mAI.reset (new CKI(this));
		mAI->setTaskPriority(NR_PRIORITY_VERY_HIGH);
		nrKernel.AddTask(mAI);
		nrKernel.StartTask(mAI->getTaskID());

		//activate input class
		mUserInput->activate(true);

		return NR_OK;
	}


	//--------------------------------------------------------------------------
	nrResult CLevel::update()
	{
		//activate input class
		mUserInput->activate(true);

		//executeODE
		executeODE(COgreTask::GetSingleton().mTimer->getFrameInterval());

		return NR_OK;
	}


	//--------------------------------------------------------------------------
	nrResult CLevel::stop()
	{
		//userInput
		mUserInput->activate(false);
		nrKernel.RemoveTask(mUserInput->getTaskID());
		mUserInput.reset();

		//mAI
		nrKernel.RemoveTask(mAI->getTaskID());
		mAI.reset();

		return NR_OK;
	}


	//--------------------------------------------------------------------------
	boost::shared_ptr< CTerrain >  CLevel::Terrain()
	{
		return mTerrain;
	}


	//--------------------------------------------------------------------------
	std::vector<boost::shared_ptr<CBaseObject> >& CLevel::Objects()
	{
		return mObjects;
	}


	//--------------------------------------------------------------------------
	boost::shared_ptr<CWaypoint> CLevel::findNearestWaypoint(boost::shared_ptr<CWaypoint> from, int nr)
	{

		// Variables to save nearest Waypoint, set it to far away
		Ogre::Vector3 			dummy;
		dummy.x 					= -10000.0;
		dummy.y 					= -10000.0;
		dummy.z 					= -10000.0;
		boost::shared_ptr<CWaypoint> 	nearest;
		nearest.reset(new CWaypoint(dummy, -1));


		// Variable to save current Waypoint while going trough all
		boost::shared_ptr<CWaypoint> 	current;


		// Variables to save div. vectors for better performance :D
		Ogre::Vector3 			nearestVector	= nearest->getVector();
		Ogre::Vector3 			currentVector;
		Ogre::Vector3 			fromVector	= from->getVector();


		// Helpers
		float sumFromCurrent = 0.0f;
		float sumFromNearest = 0.0f;
		bool goOn;

		// Go for each waypoint
		int i = 0;
		while (i < mWaypoints.size())
		{
			// Set current vector to one of vector
			current				= mWaypoints[i];

			goOn = true;

			if (current->getPrev())			goOn = false;
			if (current == from)			goOn = false;
			if (((from->getFirst() == current)&& (nr < 10)))	goOn = false;

			// Ignore the same object!
			if (goOn == true)
			{
				// The nearest vector is found, when the sum of the difference between
				// the current and the from vector is smaller than the difference of
				// the nearest and the from vector. Height is factor 5, so that a bridge
				// over a street is more far away and the waypoints of the street under it
				// will be ignored
				currentVector	 = current->getVector();
				sumFromCurrent	 = fabs (currentVector.x - fromVector.x);
				sumFromCurrent	+= fabs (currentVector.y - fromVector.y);
				sumFromCurrent	+= fabs (currentVector.z - fromVector.z);

				if ((sumFromCurrent < sumFromNearest) || (sumFromNearest == 0.0f))
				{
					sumFromNearest = sumFromCurrent;
					nearest = current;
				};
			};

			i++;
		};


		return nearest;
	}

	//--------------------------------------------------------------------------
	bool CLevel::buildWaypointPath()
	{
		// Go for each obejct
		int i = 0;
		boost::shared_ptr<CWaypoint> waypoint;
		Ogre::Vector3 pos;
		boost::shared_ptr<CBaseObject> currentObject ;


		while (i < mObjects.size())
		{
		 	currentObject = mObjects[i];
			i++;

			// Go for each item of object
			while (currentObject->hasWaypoints())
			{
				pos = currentObject->getWaypoint();
				waypoint.reset(new CWaypoint(pos, i));
				mWaypoints.push_back(waypoint);
			};
		};


		boost::shared_ptr<CWaypoint> current;
		boost::shared_ptr<CWaypoint> nearest;
		boost::shared_ptr<CWaypoint> first;
		boost::shared_ptr<CWaypoint> last;

		// Debugging output
		/*i = 0;
		while (i < mWaypoints.size())
		{
			current = mWaypoints[i];
			this->drawWaypoint(current->getVector(), i);
			i++;
		}*/
		i=0;

		// Startpoint
		if (mWaypoints.size() == 0){
			nrLog.Log(NR_LOG_APP, "CLevel::buidlWaypointPath(): There were no waypoints found!!!");
			return false;
		}

		current = mWaypoints[0];
		first	= current;

		bool goOn = true;

		// Go for all waypoints
		while (goOn == true)
		{
			nearest = this->findNearestWaypoint(current, i);

			//Save
			current->setNext(nearest);
			nearest->setPrev(current);
			nearest->setFirst(first);

			// Debugging output
			// this->drawWaypoint(nearest->getVector(),current->getVector(), i);

			i++;
			current = nearest;

			if (i >= mWaypoints.size()-1) 	goOn = false;
			if (first == current) 		goOn = false;
		};

		// Last waypoint
		last = current;

		// Save last waypoint for all waypoints
		i = 0;
		while (i < mWaypoints.size())
		{
			current = mWaypoints[i];
			current->setLast(last);
			i++;
		};

		// Check if any waypoints were found
		if (mWaypoints.size() > 1) return true;
		return false;
	};


	//--------------------------------------------------------------------------
	boost::shared_ptr<CWaypoint> CLevel::getFirstWaypoint()
	{
		if (mWaypoints.size() > 1) return mWaypoints[0];
	};



	//--------------------------------------------------------------------------
	boost::shared_ptr<CWaypoint> CLevel::getNextWaypoint(boost::shared_ptr<CWaypoint> waypoint, int nr = 1)
	{
		for (int i = 0; i < nr; i++)
		{
			if (waypoint->getNext())
			{
				waypoint = waypoint->getNext();
			};
		};
		return waypoint;
	};


	//--------------------------------------------------------------------------
	//handle ODE collision
	bool CLevel::collision(OgreOde::Contact* contact)
	{
		if(!OgreOde_Prefab::Vehicle::handleTyreCollision(contact))
		{
			contact->setBouncyness(0.0);
			contact->setCoulombFriction(mTerrainFriction);
		}
		return true;
	}


	//--------------------------------------------------------------------------
	void CLevel::InitializeODE()
	{

		//set its parameters
		mPhysicsWorld->setGravity(Vector3(0,-mGravity,0));
		mPhysicsWorld->setCFM(mCFM);
		mPhysicsWorld->setERP(mERP);
		mPhysicsWorld->setAutoSleep(true);
		mPhysicsWorld->setContactCorrectionVelocity(mCCV);

		mPhysicsWorld->setCollisionListener(this);


		//TEST vehiclee here
//		mVehicle.reset(new OgreOde_Prefab::Vehicle("Jeep"));
//		mVehicle->load("jeep_ode.xml");
		//mVehicle.reset(new OgreOde_Prefab::Vehicle("Subaru"));
		//mVehicle->load("subaru_ode.xml");

		//set position to the vehicle
/*		Vector3 v_pos(150,5,150);
		mVehicle->setPosition(v_pos);
		//mVehicle->getSceneNode()->setScale(3.0f ,3.0f ,3.0f);*/


		//ADD TEST PHYSICS OBJECT
		//load mesh
/*		SceneNode *track_node = mOgreTask->mSceneMgr->getRootSceneNode()->createChildSceneNode("jump1");
		Entity *track_mesh = mOgreTask->mSceneMgr->createEntity("jump1","jump.mesh");
		track_node->attachObject(track_mesh);

		//move node
		track_node->setPosition(Vector3(120,3.4,120));

		//get geometry
		OgreOde::EntityInformer ei1(track_mesh);

		int meshVertexCount = ei1.getVertexCount();
		int meshIndexCount = ei1.getIndexCount();

		Ogre::Vector3* meshVertices = (Ogre::Vector3*)ei1.getVertices();
		int* meshIndices = (int*)ei1.getIndices();

		//move vertices
		for (int c=0; c<meshVertexCount; c++)
		{
			meshVertices[c] += Vector3(120,3.4,120);
		}

		//create new geometry
		OgreOde::TriangleMeshGeometry* geom =
			new OgreOde::TriangleMeshGeometry(meshVertices, meshVertexCount,
			meshIndices, meshIndexCount, mPhysicsWorld->getDefaultSpace());



		//load mesh
		track_node = mOgreTask->mSceneMgr->getRootSceneNode()->createChildSceneNode("jump2");
		track_mesh = mOgreTask->mSceneMgr->createEntity("jump2","jump.mesh");
		track_node->attachObject(track_mesh);

		//move node
		track_node->setPosition(Vector3(420,3.4,420));

		//get geometry
		OgreOde::EntityInformer ei2(track_mesh);

		meshVertexCount = ei2.getVertexCount();
		meshIndexCount = ei2.getIndexCount();

		meshVertices = (Ogre::Vector3*)ei2.getVertices();
		meshIndices = (int*)ei2.getIndices();

		//move vertices
		for (int c=0; c<meshVertexCount; c++)
		{
			meshVertices[c] += Vector3(420,3.4,420);
		}

		//create new geometry
		geom =
			new OgreOde::TriangleMeshGeometry(meshVertices, meshVertexCount,
			meshIndices, meshIndexCount, mPhysicsWorld->getDefaultSpace());*/





/*		// Create a box for ODE and attach it to the Ogre version
		SceneNode *crate_node = mOgreTask->mSceneMgr->getRootSceneNode()->createChildSceneNode("crate");
		Entity *crate_mesh = mOgreTask->mSceneMgr->createEntity("crate","crate.mesh");
		crate_node->attachObject(crate_mesh);
		crate_node->setPosition(Vector3(150,20,120));

		OgreOde::Body* body;
		//OgreOde::TriangleMeshGeometry *crate_geom;
		OgreOde::EntityInformer ei2(crate_mesh);

		body = ei2.createSingleDynamicBox(0.02, mPhysicsWorld->getDefaultSpace());
*/
	}


	//--------------------------------------------------------------------------
	void CLevel::executeODE(float delaySeconds)
	{
		/*while (delaySeconds > 0.02)
		{
			mPhysicsWorld->synchronise();
			mPhysicsWorld->getDefaultSpace()->collide();

			mPhysicsWorld->quickStep(0.02);
			mPhysicsWorld->clearContacts();

			delaySeconds -= 0.02;
		}*/
		mPhysicsWorld->synchronise();
		mPhysicsWorld->getDefaultSpace()->collide();

		//mPhysicsWorld->quickStep(delaySeconds);
		mPhysicsWorld->step(delaySeconds);
		mPhysicsWorld->clearContacts();
	}


	//--------------------------------------------------------------------------
	boost::shared_ptr<CCarObject> CLevel::getCar(int whichCar)
	{
		if      (whichCar == CLevel::CAR_AI)    return this->mAICar;
		else if (whichCar == CLevel::CAR_HUMAN) return this->mHumanCar;
	}



	//--------------------------------------------------------------------------
	void CLevel::searchCars()
	{
		shared_ptr<CBaseObject> currentObject;
		int i = 0;
		while (i < mObjects.size())
		{
		 	currentObject = mObjects[i];
			i++;

			if (currentObject->getName() == "AICar")
			{
				// Check if another AI Car exists, if true, then log it
				if (!this->mAICar)	this->mAICar = boost::static_pointer_cast<CCarObject, CBaseObject>(currentObject);
				else nrLog.Log(NR_LOG_APP, "CLevel::searchCars(): There already exists an AI Car!");
			}

			if (currentObject->getName() == "HumanPlayerCar")
			{
				// Check if another AI Car exists, if true, then log it
				if (!this->mHumanCar) this->mHumanCar = boost::static_pointer_cast<CCarObject, CBaseObject>(currentObject);
				else nrLog.Log(NR_LOG_APP, "CLevel::searchCars(): There already exists an Human Player Car!");
			}
		}
	}
};
