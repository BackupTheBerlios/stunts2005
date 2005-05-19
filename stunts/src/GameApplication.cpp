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


namespace stunts {
  
	GameApplication::GameApplication()
    : __root(0)
	{}
  
	GameApplication::~GameApplication()
	{
		//shutdown te game engine
		nrEngineDelete(); 	
		
		//delete OGRE root
		if (__root)
    		delete __root;
  	}
  
	void GameApplication::run()
	{
		//run the game engine
		nrKernel.Execute();
	}
  
	void GameApplication::initialize()
	{
		//initialize game engine
		nrEngineInit();
		
		//add tasks
		shared_ptr < nrITask > task (new CUserInput());
		task->setTaskPriority(NR_PRIORITY_VERY_HIGH);
		nrKernel.AddTask(task);
		
		//
		nrFramework.AddToKernel(nrKernel, NR_PRIORITY_LAST);
	}
  
}
