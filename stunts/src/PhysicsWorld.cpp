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


#include "PhysicsWorld.hpp"

#include <iostream>

namespace stunts {
  
  CPhysicsWorld::CPhysicsWorld()
    : __world_id(0)
  {}
  
  CPhysicsWorld::~CPhysicsWorld() {
    /* destroy world */
    if(__world_id) {
      std::cout << "destroy world" << std::endl;
      dWorldDestroy(__world_id);
      __world_id = 0;
    }
  }
  
  CPhysicsWorld::CPhysicsWorld(const CPhysicsWorld&)
    : __world_id(0)
  {
    /* never called */
  }
  
  const CPhysicsWorld& CPhysicsWorld::operator=(const CPhysicsWorld&) {
    /* never called */
    return *this;
  }
  
  nrResult CPhysicsWorld::taskInit() {
    nrLog.Log(NR_LOG_APP, "Init physics...");
    
    /* create a world */
    std::cout << "create world" << std::endl;
    __world_id = dWorldCreate();
    
    return NR_OK;
  }
  
  nrResult CPhysicsWorld::taskStart() {
    nrLog.Log(NR_LOG_APP, "Start physics...");
    return NR_OK;
  }
  
  nrResult CPhysicsWorld::taskUpdate() {
    /* just a test */
    dWorldStep(__world_id,0.05);
    
    return NR_OK;
  }
  
  nrResult CPhysicsWorld::taskStop() {
    nrLog.Log(NR_LOG_APP, "Stop physics...");
    return NR_OK;
  }
  
}
