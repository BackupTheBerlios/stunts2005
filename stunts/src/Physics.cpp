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


#include "Physics.hpp"

namespace stunts {
  
  CPhysics::CPhysics() throw()
    : __world(0),
      __body_id(0)
  {}
  
  CPhysics::CPhysics(const CPhysics& other) throw()
    : __world(0),
      __body_id(0)
  {}
  
  CPhysics* CPhysics::copy() const throw() {
    return new CPhysics(*this);
  }
  
  CPhysics::~CPhysics() {
    if(__body_id) {
      dBodyDestroy(__body_id);
      __body_id = 0;
    }
  }
  
}
