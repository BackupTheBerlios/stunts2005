/* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup,
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Florian Winter <fw@graphics.cs.uni-sb.de>
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


#include "GameApplication.hpp"

#include "OgreTimer.hpp"
#include "CGuiTask.h"

using boost::shared_ptr;


namespace stunts
{
	GameApplication::GameApplication()
	{
	
	}


	GameApplication::~GameApplication()
	{
		CGuiTask::Release();
		COgreTask::Release();
		
		// kill all tasks if there are any
		nrKernel.KillAllTasks();

		//delete nrEngine singleton
		nrEngineDelete();
	}

	
	void GameApplication::run()
	{
		//go into rendering loop
		nrKernel.Execute();
	}


	/**
	 * Initialize the application
	 **/
	bool GameApplication::initialize()
	{
		//create nrEngine singleton
		nrEngineInit();

		//Logging
		nrLog.Init("../log/", "");
		nrLog.Log(NR_LOG_APP, "Application started");

		// Initialize virtual file system
		nrVFS.setPathToVFS("../");
		if (nrVFS.openFileSystem() != NR_OK){
			printf("VFS Error !!!\n");
			return false;
		}

		
		//add tasks
		COgreTask::Instantiate();
		
		shared_ptr<CLevel> 		level_task(new CLevel());
		shared_ptr<CUserInput> 	user_input(new CUserInput(level_task));
		shared_ptr<CPhysicsWorld>physics_world(new CPhysicsWorld(level_task));
			
		
		//set priorities
		user_input->setTaskPriority(NR_PRIORITY_VERY_HIGH);
		level_task->setTaskPriority(NR_PRIORITY_VERY_LOW);
		
		// Add singletons to the kernel		
		COgreTask::GetSingleton().AddToKernel(nrKernel, NR_PRIORITY_NORMAL);
				
		//add tasks to the kernel
		nrKernel.AddTask(user_input);
		nrKernel.AddTask(physics_world);
		nrKernel.AddTask(level_task);
				
		// set level variables. JUST FOR TESTING, this should be done by GUI
//		nrSettings.get("level_file") = std::string("../media/level/Level.xml");
//		nrSettings.get("load_level") = std::string("1");
		
		// Setup InGame-Clock
		shared_ptr<nrITimeSource> timer (new COgreTimer());
		nrCClock::GetSingleton().setTimeSource(timer);
		
		// Add clock to the kernel. The priority must be first, because we
		// want use timer before Ogre
		nrCClock::GetSingleton().AddToKernel(nrKernel, NR_PRIORITY_FIRST);


		// add GUI task
		CGuiTask::Instantiate();
		CGuiTask::GetSingleton().AddToKernel(nrKernel, NR_PRIORITY_ULTRA_LOW);

		// this is how you load a page!
		CGuiTask::GetSingleton().addPage( "dummy", "" );
		CGuiTask::GetSingleton().selectPage( "dummy" );		
		CGuiTask::GetSingleton().rActive() = false;
 		
        return true;
	}



	/**
	 * Quit the application
	 **/
	void GameApplication::quit(void* p)
	{
		nrLog.Log(NR_LOG_APP, "Stunts::quit(): Quit the application");
		nrKernel.KillAllTasks();
	}

}
