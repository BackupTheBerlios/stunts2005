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
#include "PhysicsWorld.hpp"

#include <nrEngine/nrEngine.h>

namespace stunts {
  
  GameApplication::GameApplication()
    : __root(0)
  {}
  
  GameApplication::~GameApplication() {
	// kill all tasks if there is one 
	nrKernel.KillAllTasks();
    
	/* delete nrFramework singleton */
    nrFrameworkDelete();
    
    /* delete nrEngine singleton */
    nrEngineDelete();
    
    /* delete OGRE root */
    delete __root;
  }
  
  void GameApplication::run() {
    /* run the game engine */
    nrKernel.Execute();
  }
  
  void GameApplication::initialize() {
    /* create nrEngine singleton */
    nrEngineInit();

	/* Logging */    
	nrLog.Init("../log/","");
	nrLog.Log(NR_LOG_APP, "Application started");

    /* create nrFramework singleton */
    nrFrameworkInit();
	int handle = 0;
	nrResult ret = nrFramework.createRenderContext (&handle,800,600,32);
	if (ret != NR_OK){
		nrLog.Log(NR_LOG_APP, "Can not create rendering window");
		return;
	}
	nrFramework.getRC()->changeWindowTitle("Stunts2005");
   	nrFramework.setQuitCallback(GameApplication::quit, NULL);

    /* add tasks */
    boost::shared_ptr<CUserInput> user_input(new CUserInput());
    boost::shared_ptr<CPhysicsWorld> physics_world(new CPhysicsWorld());
    user_input->setTaskPriority(NR_PRIORITY_VERY_HIGH);
    nrKernel.AddTask(user_input);
    nrKernel.AddTask(physics_world);
    
    /* whatever */
    nrFramework.AddToKernel(nrKernel, NR_PRIORITY_LAST);
  }
  
  void GameApplication::quit(void* p)
  {
	nrLog.Log(NR_LOG_APP, "Stunts::quit(): Quit the application");
	nrKernel.KillAllTasks();	
  }

}
