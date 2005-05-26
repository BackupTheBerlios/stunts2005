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


namespace stunts
{
	GameApplication::GameApplication()
	{
	
	}


	GameApplication::~GameApplication()
	{
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

		//add tasks
		boost::shared_ptr<CLevel>
			level_task(new CLevel());

		boost::shared_ptr<CUserInput>
			user_input(new CUserInput(level_task));
			
		boost::shared_ptr<CPhysicsWorld>
			physics_world(new CPhysicsWorld(level_task));
			
		boost::shared_ptr<COgreTask>
			ogre_task(new COgreTask(level_task));


		//set priorities
		user_input->setTaskPriority(NR_PRIORITY_VERY_HIGH);
		level_task->setTaskPriority(NR_PRIORITY_VERY_HIGH);
		ogre_task->setTaskPriority(NR_PRIORITY_FIRST);
		
		//add tasks to the kernel
		nrKernel.AddTask(user_input);
		nrKernel.AddTask(physics_world);
		nrKernel.AddTask(ogre_task);
		nrKernel.AddTask(level_task);

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
