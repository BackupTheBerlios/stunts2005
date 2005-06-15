/* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup,
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Christian Morbach <GameDevelopment@paratronic.de>
 *                    Art Tevs <tevs@mpi-sb.mpg.de>
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

#include "ObjectInstantiator.hpp"

using boost::shared_ptr;

namespace stunts
{

	//--------------------------------------------------------------------------
	CLevel::CLevel()
	{
		mIsLoaded = false;
		mShouldLoadLevel = false;
		mLevelFileName = "";

		// Setup default grid count size
		mGridCountInX = 30;
		mGridCountInZ = 30;

		registerVariables();
	}


	//--------------------------------------------------------------------------
	CLevel::~CLevel()
	{
		deregisterVariables();
	}


	//--------------------------------------------------------------------------
	void CLevel::registerVariables()
	{
		nrSettingsRegisterString(mLevelFileName, 	"level_file");

		nrSettingsRegister(bool, mShouldLoadLevel, 	"load_level");
		nrSettingsRegister(bool, mIsLoaded, 		"level_is_loaded");
	}

	//--------------------------------------------------------------------------
	void CLevel::deregisterVariables()
	{
		nrSettings.deregisterVariable("level_file");
		nrSettings.deregisterVariable("load_level");
		nrSettings.deregisterVariable("level_is_loaded");
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

        // Get the gravity for this level
		elem = rootElem->FirstChildElement("gravity");
		if (elem)
			readGravity(elem);

		// check for terrain file
		elem = rootElem->FirstChildElement("terrain");
		if (elem)
			readTerrain(elem);

//######################################
//unproper!!

// if you don't call the OGRE task
// you cannot get the terrain height!

//FIX THIS!

		COgreTask::GetSingleton().taskUpdate();
//######################################

		// check now for object in the file
		elem = rootElem->FirstChildElement("objects");
		if (elem)
			readObjects(elem);

		// check for the atmosphere
		elem = rootElem->FirstChildElement("atmosphere");
		if (elem)
			readAtmosphere(elem);

		mIsLoaded = true;

		// Erstelle Gitter model
/*		for (int32 i = 0; i < mGridCountInX; i++)
			for (int32 j=0; j < mGridCountInZ; j++){

				char name[256];
				sprintf(name, "Gitter_%d_%d", i,j);
				fprintf(stderr, "%s\n", name);

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
		}*/

		return false;
	}

	//--------------------------------------------------------------------------
	void CLevel::readGravity(TiXmlElement* elem)
	{
		nrLog.Log(NR_LOG_APP, "CLevel::readGravity(): Read the gravity value for the level");

		// The gravity node should contain only the text containing only a float number
		// We can easely setup the gravity from the string by using the dator class
		nrCDator<float32>	gravity(this->mGravity);

		// assign string to the gravity value
		gravity = elem->Attribute("value");

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
			mTerrain.reset (new CTerrain (OgreTask()->mSceneMgr));
			mTerrain->importFromFile((mLevelFilePath + smElem->Attribute("file")).c_str(), smElem->Attribute("root"));
		}

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
	nrResult CLevel::taskInit()
	{
		//get all tasks and set member attributes
		getEngineTasks();

		//create terrain (after the engine tasks have been gotten)
		//mTerrain.reset(new CTerrain (OgreTask()->mSceneMgr));

		//return
		return NR_OK;
	}


	//--------------------------------------------------------------------------
	nrResult CLevel::taskStart()
	{
		//activate input class
		UserInput()->activate(true);

		return NR_OK;
	}


	//--------------------------------------------------------------------------
	nrResult CLevel::taskUpdate()
	{
		// if we are forced to load the level file
		if (mShouldLoadLevel && mLevelFileName.length() > 0){
			mShouldLoadLevel = false;
			if (!loadLevel(mLevelFileName))
				return NR_UNKNOWN_ERROR;
		}

		return NR_OK;
	}


	//--------------------------------------------------------------------------
	nrResult CLevel::taskStop()
	{
		return NR_OK;
	}


	//--------------------------------------------------------------------------
	void CLevel::getEngineTasks()
	{
		//get tasks
		mOgreTask = boost::dynamic_pointer_cast<COgreTask, nrITask>(nrKernel.getTaskByName("OgreTask"));
		mUserInput = boost::dynamic_pointer_cast<CUserInput, nrITask>(nrKernel.getTaskByName("UserInput"));

		//check for errors
		if (!mOgreTask || !mUserInput)
		{
			nrLog.Log(NR_LOG_APP, "CLevel::getEngineTasks(): Error in getting\
				Tasks");

			#ifdef ENABLE_DEBUG_MESSAGES
				std::cout << "CLevel::getEngineTasks(): Error in getting\
					Tasks" << std::endl;
			#endif

			nrKernel.KillAllTasks();
		}
	}


	//--------------------------------------------------------------------------
	boost::shared_ptr< COgreTask >  CLevel::OgreTask()
	{
		//COgreTask::GetSingletonPtr();
		return mOgreTask;
	}


	//--------------------------------------------------------------------------
	boost::shared_ptr< CUserInput >  CLevel::UserInput()
	{
		return mUserInput;
	}

	//--------------------------------------------------------------------------
	boost::shared_ptr< CTerrain >  CLevel::Terrain()
	{
		return mTerrain;
	}
};






