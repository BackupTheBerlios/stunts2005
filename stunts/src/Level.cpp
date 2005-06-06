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

using boost::shared_ptr;

namespace stunts
{
	//--------------------------------------------------------------------------
	CLevel::CLevel()
	{
		mIsLoaded = false;
		mShouldLoadLevel = false;
		mLevelFileName = "";
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

		mLevelFileName = levelFile;

        // Get the gravity for this level
		elem = rootElem->FirstChildElement("gravity");
		if (elem)
			readGravity(elem);

		// check for terrain file
		elem = rootElem->FirstChildElement("terrain");
		if (elem)
			readTerrain(elem);

		// check now for object in the file
		elem = rootElem->FirstChildElement("objects");
		if (elem)
			readObjects(elem);

		// check for the atmosphere
		elem = rootElem->FirstChildElement("atmosphere");
		if (elem)
			readAtmosphere(elem);

		mIsLoaded = true;
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
	void CLevel::readAtmosphere(TiXmlElement* elem)
	{
		nrLog.Log(NR_LOG_APP, "CLevel::readAtmosphere(): Read atmosphere node form level file");

		// check for importing of a file
		TiXmlElement*	smElem = elem->FirstChildElement("import");
		if (smElem)
		{
			mAtmosphere.reset(new CAtmosphere());
			mAtmosphere->importFromFile(smElem->Attribute("file"), smElem->Attribute("root"));
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
			mTerrain.reset(new CTerrain(mOgreTask->mSceneMgr));
			mTerrain->importFromFile(smElem->Attribute("file"), smElem->Attribute("root"));
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
			// Element
			TiXmlElement*	subElem = NULL;

			// if it is an object node, so load data from it
			char* type = (char*)smElem->Attribute("type");
			shared_ptr<CBaseObject>	obj(CBaseObject::createInstance(type));
			if (obj == NULL){
				type = type == NULL ? (char*)"NULL" : type;
				nrLog.Log(NR_LOG_APP, "CLevel::readObjects(): Non valid object type \"%s\"", type);
				break;
			}

			// read the name and set it for the object
			subElem = smElem->FirstChildElement("name");
			if (subElem){
				TiXmlElement* name = subElem->FirstChildElement();
				if (name)
					obj->setName(name->ToText()->Value());
			}

			// find if we want to import a file
			subElem = smElem->FirstChildElement("import");
			if (subElem)
				obj->importFromFile(subElem->Attribute("file"));


			// if we found a controller, so bind it
			subElem = smElem->FirstChildElement("control");
			if (subElem)
				obj->bindController(subElem->Attribute("name"));

			// store the object in a vector
			this->mObjects.push_back(obj);
		}

		// now import a file if one was specified
		TiXmlElement*	smElem = elem->FirstChildElement("import");
		if (smElem)
			importTrackFile (smElem->Attribute("file"), smElem->Attribute("root"));

	}

	//--------------------------------------------------------------------------
	void CLevel::importTrackFile(const char* fileName, const char* root)
	{
		// open the document for loading
		shared_ptr<TiXmlDocument>	mDoc (new TiXmlDocument());
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

		// check now for object in the file
		elem = rootElem->FirstChildElement("objects");
		if (elem)
			readObjects(elem);

	}
	//--------------------------------------------------------------------------
	nrResult CLevel::taskInit()
	{
		//get all tasks and set member attributes
		getEngineTasks();

		//create terrain (after the engine tasks have been gotten)
		mTerrain = boost::shared_ptr< CTerrain >
			(new CTerrain(mOgreTask->mSceneMgr));

		//return
		return NR_OK;
	}


	//--------------------------------------------------------------------------
	nrResult CLevel::taskStart()
	{
		return NR_OK;
	}


	//--------------------------------------------------------------------------
	nrResult CLevel::taskUpdate()
	{
		// if we are forced to load the level file
		if (mShouldLoadLevel && !mIsLoaded && mLevelFileName.length() > 0){
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
		mPhysicsWorld = boost::dynamic_pointer_cast<CPhysicsWorld, nrITask>(nrKernel.getTaskByName("PhysicWorld"));

		//check for errors
		if (!mOgreTask || !mUserInput || !mPhysicsWorld)
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
	//--- OgreTask()
	//--------------------------------------------------------------------------
	boost::shared_ptr< COgreTask >  CLevel::OgreTask()
	{
		return mOgreTask;
	}


	//--------------------------------------------------------------------------
	//--- UserInput()
	//--------------------------------------------------------------------------
	boost::shared_ptr< CUserInput >  CLevel::UserInput()
	{
		return mUserInput;
	}


	//--------------------------------------------------------------------------
	//--- PhysicsWorld()
	//--------------------------------------------------------------------------
	boost::shared_ptr< CPhysicsWorld >  CLevel::PhysicsWorld()
	{
		return mPhysicsWorld;
	}


	//--------------------------------------------------------------------------
	//--- Terrain()
	//--------------------------------------------------------------------------
	boost::shared_ptr< CTerrain >  CLevel::Terrain()
	{
		return mTerrain;
	}
};






