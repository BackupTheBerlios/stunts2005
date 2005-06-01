/* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup,
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Christian Morbach <GameDevelopment@paratronic.de>
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


#ifndef _LEVEL_HPP_
#define _LEVEL_HPP_

//------------------------------------------------------------------------------
//--- predeclare this class
//------------------------------------------------------------------------------
namespace stunts
{
	class CLevel;
}

//------------------------------------------------------------------------------
//--- includes
//------------------------------------------------------------------------------
#include <OGRE/Ogre.h>
#include <OGRE/OgreKeyEvent.h>
#include <OGRE/OgreEventListeners.h>
#include <OGRE/OgreStringConverter.h>
#include <OGRE/OgreException.h>

#include <nrEngine/nrEngine.h>

#include "PhysicsWorld.hpp"
#include "OgreTask.hpp"
#include "UserInput.hpp"
#include "Terrain.h"

//------------------------------------------------------------------------------
//--- namespace stunts
//------------------------------------------------------------------------------
namespace stunts
{
	class CLevel : public nrITask
	{
		public:
			CLevel();
			virtual ~CLevel();
			
			
			/**
			 * Get OgreTask
			 * 
			 * @return smart pointer to OgreTask class
			 */
			boost::shared_ptr< COgreTask > 		OgreTask();

			/**
			 * Get UserInput
			 * 
			 * @return smart pointer to UserInput class
			 */
			boost::shared_ptr< CUserInput > 	UserInput();

			/**
			 * Get PhysicsWorld
			 * 
			 * @return smart pointer to PhysicsWorld class
			 */
			boost::shared_ptr< CPhysicsWorld >	PhysicsWorld();


			/**
			 * Get Terrain
			 * 
			 * @return smart pointer to Terrain class
			 */
			boost::shared_ptr< CTerrain >	Terrain();


		protected:
			virtual nrResult taskInit();
			virtual nrResult taskStart();
			virtual nrResult taskUpdate();
			virtual nrResult taskStop();
			virtual const char* taskGetName() {return "LevelTask";}

			/**
			 * get all tasks to initialize own member variables.
			 */
			void getEngineTasks();

			//member variables
			boost::shared_ptr< COgreTask >			mOgreTask;
			boost::shared_ptr< CUserInput > 		mUserInput;
			boost::shared_ptr< CPhysicsWorld > 		mPhysicsWorld;
			
			boost::shared_ptr< Ogre::InputReader >	mInputDevice;
			boost::shared_ptr< CTerrain > 			mTerrain;
	};

};// namespace stunts

#endif //_LEVEL_HPP_



