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
		//get task list
		list< boost::shared_ptr< nrITask > > taskList = nrKernel.getTaskList();
		list< boost::shared_ptr< nrITask > >::const_iterator iter;

		//iterate through this list
		for (iter=taskList.begin(); iter != taskList.end(); iter++)
		{
			if (!strcmp((*iter)->taskGetName(), "OgreTask"))
			{
				mOgreTask = *((boost::shared_ptr<COgreTask>*)&(*iter));
			}
			else if (!strcmp((*iter)->taskGetName(), "UserInput"))
			{
				mUserInput = *(boost::shared_ptr< CUserInput >*)&(*iter);
			}
			else if (!strcmp((*iter)->taskGetName(), "PhysicWorld"))
			{
				mPhysicsWorld = *(boost::shared_ptr< CPhysicsWorld >*)&(*iter);
			}			
		}
		
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

};






