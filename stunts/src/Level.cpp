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

#include "Level.hpp"

namespace stunts
{
	//--------------------------------------------------------------------------
	//--- CLevel()
	//--------------------------------------------------------------------------
	CLevel::CLevel()
	{
	}


	//--------------------------------------------------------------------------
	//--- ~CLevel()
	//--------------------------------------------------------------------------
	CLevel::~CLevel()
	{
	}


	//--------------------------------------------------------------------------
	//--- taskInit()
	//--------------------------------------------------------------------------
	nrResult CLevel::taskInit()
	{
		//get all tasks and set member attributes
		getEngineTasks();

		//create terrain (after the engine tasks have been gotten)
		mTerrain = boost::shared_ptr< CTerrain >
			(new CTerrain(boost::shared_ptr< CLevel >(this)));
			
		//return
		return NR_OK;
	}
	

	//--------------------------------------------------------------------------
	//--- taskStart()
	//--------------------------------------------------------------------------
	nrResult CLevel::taskStart()
	{
		return NR_OK;
	}


	//--------------------------------------------------------------------------
	//--- taskUpdate()
	//--------------------------------------------------------------------------
	nrResult CLevel::taskUpdate()
	{
		return NR_OK;
	}


	//--------------------------------------------------------------------------
	//--- taskStop()
	//--------------------------------------------------------------------------
	nrResult CLevel::taskStop()
	{
		return NR_OK;
	}


	//--------------------------------------------------------------------------
	//--- getEngineTasks()
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






