/* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup,
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Art Tevs <tevs@mpi-sb.mpg.de>
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


#ifndef _LEVEL_MANAGER_HPP_
#define _LEVEL_MANAGER_HPP_


//------------------------------------------------------------------------------
//--- predeclare this class
//------------------------------------------------------------------------------

namespace stunts
{
	class CLevelManager;
}

//------------------------------------------------------------------------------
//--- includes
//------------------------------------------------------------------------------
#include <OGRE/Ogre.h>

#include <nrEngine/nrEngine.h>

#include "External/tinyxml/tinyxml.h"

#include "Level.hpp"

#include <boost/scoped_ptr.hpp>

//------------------------------------------------------------------------------
//--- namespace stunts
//------------------------------------------------------------------------------
namespace stunts
{
	/**
	 * Manager that manages level loading and unloading. Also it can read
	 * files containing information about all available levels, so you can
	 * load levels by just giving their names.
	 *
	 * @note At now there is only support for one level loaded at the same time.
	 * 		In the future the manager can be rewritten to support more than one
	 *		level simultaneously.
	 **/
	class CLevelManager : public nrISingletonTask<CLevelManager>, public nrIChangeable
	{
		public:
			//! this structure holds information about a level
			struct LevelData
			{
				std::string		name;
				std::string 	path;
				std::string		author;
				std::string		datestr;
			};

			// Error codes
			enum{
				OK = 0,
				NO_SUCH_LEVEL_FOUND,
				LEVEL_ALREADY_LOADED,
				LEVEL_NOT_LOADED,
				ANOTHER_LEVEL_IS_LOADED,
				LEVEL_LOAD_ERROR,
				CONTENT_FILE_NOT_FOUND,
				CONTENT_FILE_CORRUPTED
			};

			/** Get a vector containing all levels data */
			std::vector<LevelData>&	getAllLevelData() { return mLevelList; }


			// Constructor & Desturctor
			CLevelManager ();
			~CLevelManager();

			/** Unload all currently opened levels
			*/
			int32 unload()
			{
				if (mIsCurrentLoaded)
					return unloadLevel(mCurrentLevel);

				return OK;
			}

			/** Load the level with specfied name
			*/
			int32 loadLevel(const std::string& name);

			/** Unload the level with specfied name
			*/
			int32 unloadLevel(const std::string& name);

			/** Reload the level with specfied name
			*/
			int32 reloadLevel(const std::string& name);


			/** Load the file containg level list, which show us all levels
			 *  we can use in the game
			 *  @param fileName FileName of the XML-File containing level descriptions
			 *  @return 0 if load was successful
			 */
			int32 loadLevelDescriptions(const std::string& fileName);


			/** Get the list containing all levels we can load
			*/
			//std::vector<std::string> getLevelList();

			/** Returns true if such a level names exists in our database.
			*/
			bool hasGotLevel(const std::string& name);


			//-----------------------------------------
			// Task Interface
			//-----------------------------------------
			nrResult taskInit();
			nrResult taskStart();
			nrResult taskStop();
			nrResult taskUpdate();

			const char* taskGetName() { return "CLevelManager"; }

			//----------------------------------------
			// Changeable Interface
			//----------------------------------------

			//! Register all used variables by the settings manager
			void	registerAllVars();

			//! Remove all used variables from the settings manager
			void	deregisterAllVars();


		private:

			//! Here we store the level object
			boost::scoped_ptr<CLevel>	mLevel;

			//! Vector containing all levels we are using in the game
			std::vector<LevelData>		mLevelList;

			//! Name of the level, that is currently loaded
			std::string					mCurrentLevel;

			//! If true, so we have a level that is currently loaded
			bool						mIsCurrentLoaded;

			//! String containg information about the path containg levels
			std::string					mLevelPath;

			//! Store here the name of the level to be loaded at the next cycle (setted by the settings manager)
			std::string					mLevelToLoad;

			//! If 1 so the level will be loaded at the next cycle
			bool						mShouldLoadLevel;

			//----------------------------------------------------
			// Methods
			//----------------------------------------------------
			LevelData*		getLevelData(const std::string& name);

	};

};
// namespace stunts

#endif //_LEVEL_HPP_



