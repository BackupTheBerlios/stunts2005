/* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup,
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Andreas Maurer <andi@andile.de
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

#include "BaseObject.hpp"
#include "ObjectInstantiator.hpp"

#include "External/tinyxml/tinyxml.h"
#include "Utils.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <nrEngine/nrEngine.h>

#include <OgreTask.hpp>

using boost::shared_ptr;

namespace stunts {

	//--------------------------------------------------------------------------
	// Small Helping function to parse position
	// <node unit="gridUnit">12</node>
	//--------------------------------------------------------------------------
	float parsePosition(TiXmlElement* node, CLevel* level){

		if (node == NULL || level == NULL) return 0.0f;

		// Use Dators to easy convert between strings and normal types like int,float
		float32 fvalue = 0.0f;
		nrCDator<float32> fval(fvalue);

		// Parsing the value and convert from grid units
		const char* str = node->GetText();
		if (str){
			fval = std::string(str);
			if (node->Attribute("unit") != NULL){
				// If we have got grid units, so convert them and store
				if (!strcmp(node->Attribute("unit"), "gridUnits"))
					fvalue = level->unitToMeter((float32)fval);
			}

		}

		return fvalue;
	}



	//--------------------------------------------------------------------------
	// Small Helping function to parse rotation node
	// <rotate x="." y="." z="." unit="." value="." />
	//--------------------------------------------------------------------------
	Ogre::Quaternion parseRotation(TiXmlElement* node)
	{
		if (node == NULL) return Ogre::Quaternion();

		float32 x = 0.0f;
		float32 y = 0.0f;
		float32 z = 0.0f;
		float32 w = 0.0f;

		nrCDator<float32> _x(x);
		nrCDator<float32> _y(y);
		nrCDator<float32> _z(z);
		nrCDator<float32> _w(w);

		_x = std::string(node->Attribute("x"));
		_y = std::string(node->Attribute("y"));
		_z = std::string(node->Attribute("z"));
		_w = std::string(node->Attribute("value"));

		// check for units
		if (node->Attribute("unit")){
			if (!strcmp(node->Attribute("unit"), "quarters")){
				w *= 90.0f;
			}
		}

		//Ogre::Quaternion quater(Ogre::Radian(Ogre::Degree(Ogre::Real(w))),
		//			Ogre::Vector3((Ogre::Real)x,(Ogre::Real)y,(Ogre::Real)z));
		//quater.normalise();
		Radian  rf (Real(TORAD(w)));
		Vector3 v  (x,y,z);

		Quaternion quater(rf, v);

		return quater;
	}


	//--------------------------------------------------------------------------
	CBaseObject::CBaseObject()
	{
		mLevel = NULL;
		mObjNode = NULL;
		mEntity = NULL;

		mEntityAnim = NULL;
		mObjNodeAnim = NULL;

		mMeshVertices = NULL;
		mMeshIndices = NULL;
		mEntityInformer = NULL;
		mMeshVertexCount = 0;
		mMeshIndexCount = 0;

		//set default values
		m_orientation = Ogre::Quaternion::IDENTITY;

		m_speed = 0;
		m_speedVector = Ogre::Vector3::ZERO;

		m_mass = 0;
		m_masspoint = Ogre::Vector3::ZERO;

		m_frictionCoefficient = 0;

		m_torque = 0;
		m_torqueAxis = Ogre::Vector3::ZERO;

		m_scale = Ogre::Vector3::UNIT_SCALE;

		mControl = NULL;


		setName(createName().c_str());
		setPosition(Vector3(0,0,0));
	}

	//--------------------------------------------------------------------------
	CBaseObject::CBaseObject(char* xmlSettingsString, const std::string& xmlPath)
	{
		mLevel = NULL;
		mObjNode = NULL;
		mEntity = NULL;

		mEntityAnim = NULL;
		mObjNodeAnim = NULL;

		mMeshVertices = NULL;
		mMeshIndices = NULL;
		mEntityInformer = NULL;
		mMeshVertexCount = 0;
		mMeshIndexCount = 0;


		//set default values
		m_orientation = Ogre::Quaternion::IDENTITY;

		m_speed = 0;
		m_speedVector = Ogre::Vector3::ZERO;

		m_mass = 0;
		m_masspoint = Ogre::Vector3::ZERO;

		m_frictionCoefficient = 0;

		m_torque = 0;
		m_torqueAxis = Ogre::Vector3::ZERO;

		m_scale = Ogre::Vector3::UNIT_SCALE;

		mControl = NULL;


		setName(createName().c_str());

		// call the import from string function which will call the right
		// overwrited parseSettings method
		importFromString(xmlSettingsString, xmlPath);
	}


	//--------------------------------------------------------------------------
	CBaseObject::~CBaseObject()
	{
		//
		if (mObjNodeAnim)
		{
			try {

				mObjNodeAnim->detachAllObjects();
				COgreTask::GetSingleton().mSceneMgr->getRootSceneNode()->removeAndDestroyChild(mName + "_Animation");

				mObjNodeAnim = NULL;
			}catch(...){

			}
		}

		if (mEntityAnim){
			COgreTask::GetSingleton().mSceneMgr->removeEntity(mEntityAnim);
			mEntityAnim = NULL;
		}


		// RemoveObject from memory
		if (mObjNode)
		{
			try {

				mObjNode->detachAllObjects();
				COgreTask::GetSingleton().mSceneMgr->getRootSceneNode()->removeAndDestroyChild(mName);

				mObjNode = NULL;
			}catch(...){

			}

		}

		if (mEntity){
			COgreTask::GetSingleton().mSceneMgr->removeEntity(mEntity);
			mEntity = NULL;
		}


		//delete the geometry
		if (mEntityInformer)
			delete mEntityInformer;
		mMeshVertices = NULL;
		mMeshIndices = NULL;
		mEntityInformer = NULL;

	}


	//--------------------------------------------------------------------------
	bool CBaseObject::parseSettings(TiXmlElement* rootElem, const std::string& xmlPath)
	{
		//std::cout << "Level:" << (mLevel != NULL) << std::endl;
		//std::cout << "Terrain:" << (mLevel->Terrain() != NULL) << std::endl;

		nrLog.Log(NR_LOG_APP, "CBaseObject::parseSettings(): Start parsing the settings");

		if (rootElem == NULL){
			nrLog.Log(NR_LOG_APP, "CBaseObject::parseSettings(): Not valid XML-Element given");
			return true;
		}

		// variables
		TiXmlElement* elem	= NULL;
		TiXmlElement* elemWp	= NULL;
		TiXmlElement* elemPos	= NULL;

		// get the name of the object
		elem = rootElem->FirstChildElement("name");
		if (elem){
			const char* name = elem->GetText();
			if (name){
				nrLog.Log(NR_LOG_APP, "CBaseObject::parseSettings(): Set name to \"%s\"", name);
				setName(name);
			}
		}

		// Get the geometry of the object
		elem = rootElem->FirstChildElement("geometry");
		if (elem)
			loadGeometry(elem, xmlPath);


		// Get the animation of the object
		elem = rootElem->FirstChildElement("animation");
		if (elem)
			loadAnimation(elem, xmlPath);


		// find if we want to import a file
		elem = rootElem->FirstChildElement("import");
		if (elem)
			CBaseObject::importFromFile(xmlPath + elem->Attribute("file"), xmlPath);


		// if we found a controller, so bind it
		elem = rootElem->FirstChildElement("control");
		if (elem)
			bindController(elem->Attribute("name"));


		// NOTE: The geometry has to be parsed before, so we can access to created nodes
		Ogre::Vector3 pos(0.0f, 0.0f, 0.0f);

		// Read rotation out
		elem = rootElem->FirstChildElement("rotate");
		if (elem)
			setOrientation(parseRotation(elem));

		// read proportion values
		elem = rootElem->FirstChildElement("proportion");
		if (elem)
		{
			const char* axis = elem->Attribute("axis");
			const char* val  = elem->Attribute("size");
			const char* unit = elem->Attribute("units");

			bool  useGrid = unit ? (!strcmp(unit, "gridUnits") ? true : false) : false;
			float32 value = val ? boost::lexical_cast<float32>(val) : 1.0f;

			scaleObjectProportionaly (axis[0], value, useGrid);
		}


		// check for scaling properties
		elem = rootElem->FirstChildElement("stretch");
		if (elem)
		{

			float32 x = 0.0f;
			float32 y = 0.0f;
			float32 z = 0.0f;

			nrCDator<float32> _x(x);
			nrCDator<float32> _y(y);
			nrCDator<float32> _z(z);

			_x = std::string(elem->Attribute("x"));
			_y = std::string(elem->Attribute("y"));
			_z = std::string(elem->Attribute("z"));

			m_scale.x = (Ogre::Real)x;
			m_scale.y = (Ogre::Real)y;
			m_scale.z = (Ogre::Real)z;

			/*
			const char* x = elem->Attribute("x");
			const char* y = elem->Attribute("y");
			const char* z = elem->Attribute("z");

			try{
				float fx = x ? boost::lexical_cast<float>(x) : 1.0f;
				float fy = y ? boost::lexical_cast<float>(y) : 1.0f;
				float fz = z ? boost::lexical_cast<float>(z) : 1.0f;
				mObjNode->scale(fx, fy, fz);
			}catch(...){
				return true;
			}*/
		}

		// check whenever such kind of position tag exists
		elem = rootElem->FirstChildElement("posX");
		if (elem)
			pos.x = parsePosition(elem, mLevel);

		elem = rootElem->FirstChildElement("posZ");
		if (elem)
			pos.z = parsePosition(elem, mLevel);

		elem = rootElem->FirstChildElement("posY");
		if (elem){
			// setup normal y-position
			pos.y = parsePosition(elem, mLevel);

			// check whenever we want to setup bottom position
			if (elem->Attribute("type") != NULL && mLevel->Terrain() != NULL){
				if (!strcmp(elem->Attribute("type"), "bottom")){
					Vector3 npos = Position() + pos;
					if (mLevel->Terrain()->getHeight(npos))
						pos.y += npos.y;
				}
			}

		}

		// Here we correct the node's position to the underlying grid model
		Vector3 newPos = Position() + pos;

		// Auskommentiert, da diese kleine Verschiebung zu falschen Ergebnissen f�hrt
		//if (mObjNode && mEntity){
		//	const AxisAlignedBox& aabb = mEntity->getBoundingBox();
		//	newPos.x += mObjNode->getScale().x  * (aabb.getMaximum().x - aabb.getMinimum().x) / 2.0f;
		//	newPos.z += mObjNode->getScale().z  * (aabb.getMaximum().z - aabb.getMinimum().z) / 2.0f;
		//}
		setPosition(newPos);


		// Read waypoints
		elem =  rootElem->FirstChildElement("waypoints");
		if (elem)
		{
			// Waypoint in level file
			elemWp = (elem->FirstChildElement("file"));
			if (elemWp)
			{
				const char* waypointFile = elemWp->GetText();
				if (waypointFile){
					nrLog.Log(NR_LOG_APP, "CBaseObject::parseSettings(): Load XML file \"%s\"",  waypointFile);
					try
					{
						importFromWaypointFile(std::string("../media/waypoints/") + waypointFile, xmlPath);

					} catch (...)
					{
						nrLog.Log(NR_LOG_APP, "CBaseObject::parseSettings(): An error occurs by loading of the waypoint XML file");
						return true;
					}
				}
			}
		  };

		// checkj whenever light can cast shadows
		elem = rootElem->FirstChildElement("shadow");
		if (elem)
		{
			const char* x = elem->Attribute("cast");

			bool can = x ? (bool)boost::lexical_cast<int>(x) : false;

			mEntity->setCastShadows(can);
		}

		nrLog.Log(NR_LOG_APP, "CBaseObject::parseSettings(): parsing is complete now");
		return false;

	}


	//--------------------------------------------------------------------------
	Ogre::Vector3 CBaseObject::getWaypoint()
	{
		Ogre::Vector3 waypoint, waypointNew;

		// Get one element of vector
		waypoint = this->m_waypoints.back();

		// Remove this element from vector
		this->m_waypoints.pop_back();

		// Scale
		waypoint.x *= this->m_scale.x;
		waypoint.y *= this->m_scale.y;
		waypoint.z *= this->m_scale.z;

		// Rotate
		waypointNew = this->m_orientation * waypoint;
		waypoint = waypointNew;

		// put on position
		waypoint.x += this->m_position.x;
		waypoint.x += mLevel->unitToMeter(0.5f);
		waypoint.y += this->m_position.y;
		waypoint.z += this->m_position.z;
		waypoint.z += mLevel->unitToMeter(0.5f);

		return waypoint;
	};



	//--------------------------------------------------------------------------
	bool CBaseObject::hasWaypoints()
	{
		if (this->m_waypoints.empty() == true) return false;
		return true;
	};



	//--------------------------------------------------------------------------
	bool CBaseObject::parseWaypoints(TiXmlElement* node)
	{
		// Check for Entry
		if (node == NULL)
		{
			nrLog.Log(NR_LOG_APP, "CBaseObject::parseWaypoints(): Error, no XML element found!");
			return false;
		};

		// Transform format
		Ogre::Vector3 pos(0.0f, 0.0f, 0.0f);

		float32 x = 0.0f;
		float32 y = 0.0f;
		float32 z = 0.0f;

		nrCDator<float32> _x(x);
		nrCDator<float32> _y(y);
		nrCDator<float32> _z(z);

		_x = std::string(node->Attribute("x"));
		_y = std::string(node->Attribute("y"));
		_z = std::string(node->Attribute("z"));

		pos.x = (Ogre::Real)x;
		pos.y = (Ogre::Real)y;
		pos.z = (Ogre::Real)z;

		// add to vector
		this->m_waypoints.push_back(pos);
		return true;

	}


	//--------------------------------------------------------------------------
	bool CBaseObject::importFromWaypointFile(const char* fileName, const std::string& xmlPath)
	{

		nrLog.Log(NR_LOG_APP, "CBaseObject::importFromWaypointFile(): Read Waypoint definitions from %s", fileName);

		// load the xml document
		shared_ptr<TiXmlDocument> mDoc (new TiXmlDocument(fileName));
		if (!mDoc->LoadFile())
		{
			nrLog.Log(NR_LOG_APP, "CBaseObject::importFromFile(): Can not load the file \"%s\"", fileName);
			return true;
		}

		// Get the root element from the file
		TiXmlElement* rootElem = mDoc->FirstChildElement("waypoints");

		// Get first child & parse it
		TiXmlElement* child = NULL;
		child = rootElem->FirstChildElement("position");
		parseWaypoints(child);

		// Go for other childs
		while (child)
		{
			child = child->NextSiblingElement("position");
			if (child)
				parseWaypoints(child);
		}
	}


	//--------------------------------------------------------------------------
	bool CBaseObject::loadGeometry(TiXmlElement* geomElem, const std::string& xmlPath){

		// Logging
		nrLog.Log(NR_LOG_APP, "CBaseObject::loadGeometry(): Load geometry definition of the object");

		if (geomElem == NULL){
			nrLog.Log(NR_LOG_APP, "CBaseObject::loadGeometry(): Not a valid XML-Element given");
			return true;
		}

		// variables
		TiXmlElement* elem = NULL;

		// get the name of the object
		elem = geomElem->FirstChildElement("file");
		if (elem){
			const char* file = elem->GetText();
			if (file){
				nrLog.Log(NR_LOG_APP, "CBaseObject::loadGeometry(): Load for object %s mesh file \"%s\"", mName.c_str(), file);
				try
				{
					// Create & Load the entity
					mEntity 	= COgreTask::GetSingleton().mSceneMgr->createEntity(mName + "_Entity", std::string(file));
					if (mEntity != NULL && mEntity->getMesh().get() != NULL){
						mEntity->getMesh()->buildEdgeList();
						/*if (mEntity->getMesh()->getNumSubMeshes() > 0){
						Mesh::LodDistanceList dist;
						dist.push_back(200.0f);
						dist.push_back(700.0f);
						dist.push_back(1500.0f);
						dist.push_back(2000.0f);

						mEntity->getMesh()->generateLodLevels(
							dist, ProgressiveMesh::VRQ_CONSTANT, 10);
						}*/
					}
					mObjNode 	= mLevel->mLevelNode->createChildSceneNode(mName);
					mEntity->setCastShadows( true );
					mObjNode->attachObject( mEntity );

					//get raw geometry
					mEntityInformer = new OgreOde::EntityInformer(mEntity);

					mMeshVertexCount = mEntityInformer->getVertexCount();
					mMeshIndexCount = mEntityInformer->getIndexCount();

					mMeshVertices = (Ogre::Vector3*)mEntityInformer->getVertices();
					mMeshIndices = (int*)mEntityInformer->getIndices();
				}
				catch (...)
				{
					mEntity = NULL;
					mObjNode = NULL;
					nrLog.Log(NR_LOG_APP, "CBaseObject::loadGeometry(): An error occurs by loading of the geometry node");
					return true;
				}
			}
		}

		return false;
	}


	//--------------------------------------------------------------------------
	bool CBaseObject::loadAnimation(TiXmlElement* geomElem, const std::string& xmlPath)
	{

		// Logging
		nrLog.Log(NR_LOG_APP, "CBaseObject::loadAnimation(): Load the animation of the object");

		if (geomElem == NULL)
		{
			nrLog.Log(NR_LOG_APP, "CBaseObject::loadAnimation(): Not a valid XML-Element given");
			return true;
		}

		const char* type = geomElem->Attribute("type");
		const char* meshfile = geomElem->Attribute("meshfile");
		const char* axis  = geomElem->Attribute("axis");
		const char* speed = geomElem->Attribute("speed");
		const char* posX = geomElem->Attribute("posX");
		const char* posY = geomElem->Attribute("posY");
		const char* posZ = geomElem->Attribute("posZ");

		//check if all parameters are given
		if ((!type) || (!meshfile) || (!axis) || (!speed))
		{
			nrLog.Log(NR_LOG_APP, "CBaseObject::loadAnimation(): Insufficient parameters, you need type, meshfile, axis and speed");
			return true;
		}

		//check for the right type
		if (strcmp(type, "rotation") != 0)
		{
			nrLog.Log(NR_LOG_APP, "CBaseObject::loadAnimation(): Wrong type specified, supported types are: rotation");
			return true;
		}

		//check if the parent node exists
		if (!mObjNode)
		{
			nrLog.Log(NR_LOG_APP, "CBaseObject::loadAnimation(): The parent object node doesn't exist");
			return true;
		}

		//load the animation geometry
		nrLog.Log(NR_LOG_APP, "CBaseObject::loadAnimation(): Load for object %s the animation mesh file \"%s%s\"", mName.c_str(), "_Animation", meshfile);
		try
		{
			// Create & Load the entity
			mEntityAnim 	= COgreTask::GetSingleton().mSceneMgr->createEntity(mName + "_Animation_" + type, std::string(meshfile));

			//build the edge list for the shadows
			if (mEntityAnim != NULL && mEntityAnim->getMesh().get() != NULL)
			{
				mEntityAnim->getMesh()->buildEdgeList();
			}

			mObjNodeAnim 	= mObjNode->createChildSceneNode(mName + "_Animation");
			mEntityAnim->setCastShadows( true );
			mObjNodeAnim->attachObject( mEntityAnim );

			//get raw geometry
			/*mEntityInformer = new OgreOde::EntityInformer(mEntity);

			mMeshVertexCount = mEntityInformer->getVertexCount();
			mMeshIndexCount = mEntityInformer->getIndexCount();

			mMeshVertices = (Ogre::Vector3*)mEntityInformer->getVertices();
			mMeshIndices = (int*)mEntityInformer->getIndices();*/
		}
		catch (...)
		{
			mEntityAnim = NULL;
			mObjNodeAnim = NULL;
			nrLog.Log(NR_LOG_APP, "CBaseObject::loadGeometry(): An error occurs by loading of the geometry node");
			return true;
		}

		//check for the right axis
		if (strcmp(axis, "x") == 0)
			mAnimAxis = Ogre::Vector3::UNIT_X;
		else if (strcmp(axis, "y") == 0)
			mAnimAxis = Ogre::Vector3::UNIT_Y;
		else if (strcmp(axis, "z") == 0)
			mAnimAxis = Ogre::Vector3::UNIT_Z;
		else
		{
			nrLog.Log(NR_LOG_APP, "CBaseObject::loadAnimation(): Wrong axis specified, supported are: x, y, z");
			return true;
		}

		//get the speed and the position
		mAnimSpeed = 1.0f;
		mEntityAnimPos = Ogre::Vector3(0, 0, 0);

		float32 x, y, z;

		try
		{
			nrCDator<float32> _speed(mAnimSpeed);
			_speed = std::string(geomElem->Attribute("speed"));
		}
		catch(...)
		{
			mAnimSpeed = 1.0f;
		}

		try
		{
			nrCDator<float32> _x(x);
			_x = std::string(geomElem->Attribute("posX"));
		}
		catch(...)
		{
			x = 0.0f;
			nrLog.Log(NR_LOG_APP, "CBaseObject::loadAnimation(): Warning: no x position found");
		}

		try
		{
			nrCDator<float32> _y(y);
			_y = std::string(geomElem->Attribute("posY"));
		}
		catch(...)
		{
			y = 0.0f;
			nrLog.Log(NR_LOG_APP, "CBaseObject::loadAnimation(): Warning: no y position found");
		}

		try
		{
			nrCDator<float32> _z(z);
			_z = std::string(geomElem->Attribute("posZ"));
		}
		catch(...)
		{
			z = 0.0f;
			nrLog.Log(NR_LOG_APP, "CBaseObject::loadAnimation(): Warning: no z position found");
		}

		mEntityAnimPos = Ogre::Vector3(x, y, z);

		//object scale
		mAnimScale = Ogre::Vector3::UNIT_SCALE;
		float32 stretchFactor;

		try
		{
			nrCDator<float32> _stretchFactor(stretchFactor);
			_stretchFactor = std::string(geomElem->Attribute("stretch"));
		}
		catch(...)
		{
			mAnimScale = Ogre::Vector3::UNIT_SCALE;
			nrLog.Log(NR_LOG_APP, "CBaseObject::loadAnimation(): Warning: stretch value");
		}

		mAnimScale = Ogre::Vector3::UNIT_SCALE * stretchFactor;


		return false;
	}


	//--------------------------------------------------------------------------
	bool CBaseObject::importFromFile(const char* fileName, const std::string& xmlPath)
	{

		// load the xml document
		shared_ptr<TiXmlDocument> mDoc (new TiXmlDocument(fileName));
		if (!mDoc->LoadFile())
		{
			nrLog.Log(NR_LOG_APP, "CBaseObject::importFromFile(): Can not load the file \"%s\"", fileName);
			return true;
		}

		// Get the root element from the file
		TiXmlElement* rootElem = mDoc->FirstChildElement(this->getObjectType());

		// get the first root element.
		if (rootElem == NULL)
		{
			nrLog.Log(NR_LOG_APP, "CBaseObject::importFromFile(): Can not find root node \"%s\"", this->getObjectType());
			return true;
		}


		return CBaseObject::parseSettings(rootElem, xmlPath);
	}


	//--------------------------------------------------------------------------
	bool CBaseObject::importFromString(const char* xmlSettings, const std::string& xmlPath)
	{

		// load the xml document
		shared_ptr<TiXmlDocument> mDoc (new TiXmlDocument());
		mDoc->Parse(xmlSettings);
		if (mDoc->Error())
		{
			nrLog.Log(NR_LOG_APP, "CBaseObject::importFromString(): Can not parse string. %s", mDoc->ErrorDesc());
			return true;
		}

		// Get the root element from the file
		TiXmlElement* rootElem = mDoc->FirstChildElement(this->getObjectType());

		// get the first root element.
		if (rootElem == NULL)
		{
			nrLog.Log(NR_LOG_APP, "CBaseObject::importFromString(): Can not find root node \"%s\"", this->getObjectType());
			return true;
		}


		return CBaseObject::parseSettings(rootElem, xmlPath);
	}


	//--------------------------------------------------------------------------
	bool CBaseObject::bindController(const char* name){

		// create an instance of controller object
		mControl = CBaseControl::createInstance(name);

		// is the interface not valid
		if (mControl == NULL)
			return false;

		return true;
	}


	//--------------------------------------------------------------------------
	void CBaseObject::correctObjectsOrigin()
	{
		return;
#if 0
		// only if can access to the geometry
		if (!mObjNode || !mEntity) return;

		// get the AABB
		const AxisAlignedBox& box = mEntity->getBoundingBox();

		// get length along the needed axis
		Real lengthX = box.getMaximum().x - box.getMinimum().x;
		Real lengthY = box.getMaximum().y - box.getMinimum().y;
		Real lengthZ = box.getMaximum().z - box.getMinimum().z;

		// calculate new position of the object
		Vector3 pos;// = mObjNode->getPosition();
		pos.x = lengthX/2.0f;//-(box.getMinimum().x + lengthX / 2.0f);
		pos.y = lengthY/2.0f;//-(box.getMinimum().y + lengthY / 2.0f);
		pos.z = lengthZ/2.0f;//-(box.getMinimum().z + lengthZ / 2.0f);


		//apply this to the scene node
		mObjNode->translate(pos, Ogre::Node::TS_PARENT);
		mObjNode->showBoundingBox(true);
#endif
	}


	//--------------------------------------------------------------------------
	void CBaseObject::scaleObjectProportionaly(char axis, float32 value, bool useGrid)
	{
		// only if can access to the geometry
		if (!mObjNode || !mEntity) return;

		// get the AABB
		const AxisAlignedBox& box = mEntity->getBoundingBox();

		// get length along the needed axis
		Real length = 1.0f;
		if (axis == 'x') length = box.getMaximum().x - box.getMinimum().x;
		if (axis == 'y') length = box.getMaximum().y - box.getMinimum().y;
		if (axis == 'z') length = box.getMaximum().z - box.getMinimum().z;

		// calculate new size
		if (useGrid) value = mLevel->unitToMeter(value);
		Ogre::Real scale = (value / length);

		mObjNode->setScale(scale, scale, scale);
	}


	//--------------------------------------------------------------------------
	void CBaseObject::setName(const char* name)
	{
		this->mName = name;
	}


	//--------------------------------------------------------------------------
	void CBaseObject::setPosition(Vector3 pos)
	{
		this->m_position = pos;
		if (mObjNode)
			mObjNode->setPosition(pos);
	}


	//--------------------------------------------------------------------------
	void CBaseObject::setOrientation(Quaternion orient)
	{
		this->m_orientation = orient;
		if (mObjNode)
			mObjNode->setOrientation(orient);
	}


	//--------------------------------------------------------------------------
	void CBaseObject::setMass (float mass)
	{
		this->m_mass = mass;
	}


	//--------------------------------------------------------------------------
	void CBaseObject::setMassPoint(Vector3 massPoint)
	{
		this->m_masspoint = massPoint;
	}


	//--------------------------------------------------------------------------
	void CBaseObject::setFriction(float coeff)
	{
		this->m_frictionCoefficient = coeff;
	}

	/**
	* Get event from queue
	*
	* @params none
	*
	* @return CEvent, the first item of the queue
	*/
	boost::shared_ptr<CEvent> CBaseObject::getEvent()
	{
		// Check if queue has elements
		if (this->m_eventQueue.empty() == false)
		{
			// Read first element from vector queue
			boost::shared_ptr<CEvent> firstElement = this->m_eventQueue.front();

			// Remove first element from queue
			this->m_eventQueue.pop();

			return firstElement;
		}
	}




	/**
	* Add event to queue
	*
	* @params CEvent Add new item to queue
	*
	* @return nothing
	*/
	void CBaseObject::addEvent(boost::shared_ptr<CEvent> element)
	{
		this->m_eventQueue.push(element);
		return;
	}




	/**
	* Process event from queue
	*
	* @params none
	*
	* @return 0: ok; -1 Error: No further elements in queue;
	*/
	int CBaseObject::process()
	{
		// If no further element @ queue
		if (this->m_eventQueue.empty() == true) return -1;

		// Get first element from queue
		boost::shared_ptr<CEvent> task = this->getEvent();

		// Check what todo
		// TODO: Implementierung der verschiedenen
		// Tasks, die durch das Objekt abgearbeitet
		// werden muessen!

		return true;
	};

};
