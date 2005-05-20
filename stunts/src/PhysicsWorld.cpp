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

namespace stunts {
  
  CPhysicsWorld::CPhysicsWorld()
    : __world_id(0)
  {}
  
  CPhysicsWorld::~CPhysicsWorld() {}
  
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
    nrLog.Log(NR_LOG_CONSOLE, "Init physics...");
    return 0;
  }
  
  nrResult CPhysicsWorld::taskStart() {
    nrLog.Log(NR_LOG_CONSOLE, "Start physics...");
    return 0;
  }
  
  nrResult CPhysicsWorld::taskUpdate() {
    return 0;
  }
  
  nrResult CPhysicsWorld::taskStop() {
    nrLog.Log(NR_LOG_CONSOLE, "Stop physics...");
    return 0;
  }
  
}
