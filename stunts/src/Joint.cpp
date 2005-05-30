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


#include "Joint.hpp"
#include "Physics.hpp"

namespace stunts {
  
  inline Ogre::Vector3 makeVector3(const dVector3& v) {
    return Ogre::Vector3(v[0], v[1], v[2]);
  }
  
  
  CJoint::CJoint() throw()
    : __world(0),
      __body1(0),
      __body2(0)
  {}
  
  CJoint::CJoint(const CJoint& other) throw()
    : __world(other.__world),
      __body1(other.__body1),
      __body2(other.__body2)
  {}
  
  CJoint* CJoint::copy() const throw() {
    return new CJoint(*this);
  }
  
  CJoint::~CJoint() {
    if(__joint_id) {
      dJointDestroy(__joint_id);
      __joint_id = 0;
    }
  }
  
  void CJoint::attach(CPhysics* body1, CPhysics* body2) throw() {
    /* set first and second body */
    __body1 = body1;
    __body2 = body2;
    
    /* attach internally */
    if(__joint_id) {
      dBodyID body_id1 = getFirstBody() ? getFirstBody()->getBodyID() : 0;
      dBodyID body_id2 = getSecondBody() ? getSecondBody()->getBodyID() : 0;
      
      dJointAttach(__joint_id, body_id1, body_id2);
    }
  }
  
  void CJoint::getFeedback(Ogre::Vector3& f1, Ogre::Vector3& t1,
			   Ogre::Vector3& f2, Ogre::Vector3& t2) const throw()
  {
    /* joint must be attached */
    assert(__body1);
    assert(__body1);
    
    /* joint must be live */
    assert(__joint_id);
    
    const dJointFeedback* feedback = dJointGetFeedback(__joint_id);
    f1 = makeVector3(feedback->f1);
    t1 = makeVector3(feedback->t1);
    f2 = makeVector3(feedback->f2);
    t2 = makeVector3(feedback->t2);
  }
  
}
