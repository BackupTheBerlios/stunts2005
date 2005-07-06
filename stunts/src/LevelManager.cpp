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


//------------------------------------------------------------------------------
//--- includes
//------------------------------------------------------------------------------
#include "LevelManager.hpp"
#include "Utils.hpp"

#include <boost/shared_ptr.hpp>

using boost::shared_ptr;
using namespace std;
using namespace Ogre;

//------------------------------------------------------------------------------
//--- namespace stunts
//------------------------------------------------------------------------------
namespace stunts
{

	//--------------------------------------------------------------------------
	CLevelManager::CLevelManager():mLevel(0)
	{

		mShouldLoadLevel = false;
		mIsCurrentLoaded = false;

		mUseShadows = 0;
		mUseShadowsInWater = false;
		mGameMode = NOT_STARTED;
		mShadowTechnique = COgreTask::GetSingleton().mSceneMgr->getShadowTechnique();

		registerAllVars();
	}

	//--------------------------------------------------------------------------
	CLevelManager::~CLevelManager()
	{
		unload();

		mLevel.reset();

		deregisterAllVars();
	}


	//--------------------------------------------------------------------------
	int32 CLevelManager::loadLevel(const std::string& name)
	{
		// check now, if we does support such a level
		if (!hasGotLevel(name))
		{
			nrLog.Log(NR_LOG_APP, "CLevelManager: Level %s was not found. So can not load it!", name.c_str());
			return NO_SUCH_LEVEL_FOUND;
		}

		// first we check whenever the level is already loaded
		if (mIsCurrentLoaded && mCurrentLevel == name){
			nrLog.Log(NR_LOG_APP, "CLevelManager: Level %s is already loaded. Please unload it first", name.c_str());
			return LEVEL_ALREADY_LOADED;
		}

		// if another level is loaded, so error code
		if (mIsCurrentLoaded && mCurrentLevel != name){
			nrLog.Log(NR_LOG_APP, "CLevelManager: Another level is currently loaded. Please unload it first");
			return ANOTHER_LEVEL_IS_LOADED;
		}

		// create new level object
		mLevel.reset(new CLevel());

		// get the filename of the level file
		std::string fileName = mLevelPath + getLevelData(name)->path + "/level.xml";

		nrLog.Log(NR_LOG_APP, "CLevelManager: Load level from \"%s\"", fileName.c_str());

		// load the level
		if (mLevel->loadLevel(fileName)) return LEVEL_LOAD_ERROR;
		mLevel->start();

		mIsCurrentLoaded = true;
		mCurrentLevel = name;

		// OK
		return OK;
	}


	//--------------------------------------------------------------------------
	int32 CLevelManager::unloadLevel(const std::string& name)
	{
		// check now, if we does support such a level
		if (!hasGotLevel(name)) return NO_SUCH_LEVEL_FOUND;

		// first we check whenever the level is already loaded
		if (!mIsCurrentLoaded && mCurrentLevel == name) return LEVEL_NOT_LOADED;

		// if another level is loaded, so error code
		if (mIsCurrentLoaded && mCurrentLevel != name) return ANOTHER_LEVEL_IS_LOADED;

		nrLog.Log(NR_LOG_APP, "CLevelManager: Unload level \"%s\"", name.c_str());

		// create new level object
		mLevel->stop();
		mLevel.reset(0);

		mIsCurrentLoaded = false;
		mCurrentLevel = name;

		// OK
		return OK;
	}

	//--------------------------------------------------------------------------
	int32 CLevelManager::reloadLevel(const std::string& name)
	{
		// First unload the level
		int32 ret = unloadLevel(name);
		if (ret != OK) return ret;

		// now load the level
		return loadLevel(name);
	}

	//--------------------------------------------------------------------------
	CLevelManager::LevelData* CLevelManager::getLevelData(const std::string& name)
	{
		std::vector<LevelData>::iterator it = mLevelList.begin();

		while (it != mLevelList.end())
		{
			// Level was found
			if (it->name == name) return &(*it);

			it++;
		}

		// no such level name was found
		return NULL;
	}

	//--------------------------------------------------------------------------
	bool CLevelManager::hasGotLevel(const std::string& name)
	{
		return getLevelData(name) != NULL;
	}


	//--------------------------------------------------------------------------
	void CLevelManager::registerAllVars()
	{
		nrSettingsRegisterString(mLevelToLoad, 	"level_file");

		nrSettingsRegister(bool, mShouldLoadLevel, 	"load_level");
		nrSettingsRegister(bool, mUseShadows, 		"use_shadows");
		nrSettingsRegister(bool, mUseShadowsInWater,"use_shadows_in_water");

		nrSettingsRegister(int, mGameMode, "game_mode");

	}


	//--------------------------------------------------------------------------
	void CLevelManager::deregisterAllVars()
	{
		nrSettings.deregisterVariable("level_file");
		nrSettings.deregisterVariable("load_level");
		nrSettings.deregisterVariable("use_shadows");
		nrSettings.deregisterVariable("use_shadows_in_water");
		nrSettings.deregisterVariable("game_mode");
	}


	//--------------------------------------------------------------------------
	int32 CLevelManager::loadLevelDescriptions(const std::string& fileName)
	{
		nrLog.Log(NR_LOG_APP, "CLevelManager: Load level definitions from \"%s\"", fileName.c_str());

		// we open the file for parsing.
		// Later we can open the file through the virtual file system if we had got more time
		// for development
		shared_ptr<TiXmlDocument> mLevelDoc (new TiXmlDocument(fileName.c_str()));
		if (!mLevelDoc->LoadFile())
		{
			nrLog.Log(NR_LOG_APP, "CLevelManager: There is an error occurs by loading of level description \"%s\"", fileName.c_str());
			return CONTENT_FILE_NOT_FOUND;
		}

		// Load elements form the level file and handle with them in according way
		TiXmlElement* rootElem = mLevelDoc->FirstChildElement("levels");

		// check if the root element is correct one
		if (!rootElem)
		{
			nrLog.Log(NR_LOG_APP, "CLevelManager: Content file does not contain valid data");
			return CONTENT_FILE_CORRUPTED;
		}

		// get the path from the file name
		mLevelPath = getPathFromFileName(fileName);

		// scan through all levels we can found here
		for (TiXmlNode* elem = rootElem->FirstChild("level"); elem != 0;
					elem = rootElem->IterateChildren(elem))
        {

			TiXmlElement* sElem = NULL;

			// Level data
			LevelData data;

			// get the name
			sElem = elem->FirstChildElement("name");
			if (sElem)
			{
				data.name = sElem->Attribute("name");
				data.path = sElem->Attribute("path");
			}

			// get creator data
			sElem = elem->FirstChildElement("author");
			if (sElem)
			{
				data.author = sElem->Attribute("name");
				data.datestr = sElem->Attribute("datestr");
			}
			
			// now read the textures
			sElem = elem->FirstChildElement("thumbnail");
			if (sElem)
			{
				int count = 0;
				nrCDator<int> dcount(count);
				
				const char* str = sElem->Attribute("count");
				const char* files = sElem->Attribute("file");
				
				dcount = std::string(str);
				
				// load all screenshots as textures
				for (int i=0; i < count; i++)
				{
					char filename[256];
					sprintf (filename, "%s/%s", data.path.c_str(), files);
					sprintf (filename, filename, i);
					
					Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().load(filename, "General");
				
					data.screenshots.push_back(tex);
				}
				
			}
			
			// store the data in a supported list of levels
			mLevelList.push_back(data);

			// Some log info
			nrLog.Log(NR_LOG_APP, "CLevelManager: Found level \"%s\" (path \"%s%s\") created by %s",
					data.name.c_str(), mLevelPath.c_str(), data.path.c_str(), data.author.c_str());
		}

		// OK
		return OK;
	}


	//--------------------------------------------------------------------------
	nrResult CLevelManager::taskInit()
	{
		// OK
		return NR_OK;

	}

	//--------------------------------------------------------------------------
	nrResult CLevelManager::taskStart()
	{
		// OK
		return NR_OK;

	}

	//--------------------------------------------------------------------------
	nrResult CLevelManager::taskUpdate()
	{

		// check whenever we have shadows acitvated
		static bool oldShadowMode = true;
		
		if (mUseShadows != oldShadowMode){
			oldShadowMode = mUseShadows;
			if (mUseShadows){
				COgreTask::GetSingleton().mSceneMgr->setShadowTechnique(mShadowTechnique);
			}else{
				mShadowTechnique = COgreTask::GetSingleton().mSceneMgr->getShadowTechnique();
				COgreTask::GetSingleton().mSceneMgr->setShadowTechnique(SHADOWTYPE_NONE);
			}
		}
		
		// if we are forced to load the level file
		if (mShouldLoadLevel && (mLevelToLoad.length() > 0) && (COgreTask::GetSingleton().mSceneMgr != NULL)){
			mShouldLoadLevel = false;

			nrLog.Log(NR_LOG_APP, "CLevelManager: Loading of level %s is initiated !", mLevelToLoad.c_str());

			//load the level and its physics
			//unload();
			loadLevel(mLevelToLoad);
		}

		// update the level
		if (mLevel)
			mLevel->update();

		// OK
		return NR_OK;

	}

	//--------------------------------------------------------------------------
	nrResult CLevelManager::taskStop()
	{
		// Unload the level
		unload();

		// OK
		return NR_OK;
	}


};
// namespace stunts




