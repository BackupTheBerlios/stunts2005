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


#ifndef _STUNTS_JOINT_HPP_
#define _STUNTS_JOINT_HPP_

#include <OGRE/Ogre.h>
#include <ode/ode.h>

#include <boost/serialization/serialization.hpp>

namespace stunts {
  
  class CPhysics;
  class CPhysicsWorld;
  
  /** A joint connecting 2 bodies.
   */
  class CJoint {
  public:
    /** @name Constructor & Destructor
     */
    //@{
    /** Construct a physics object.
     */
    CJoint() throw();
    
    /** Copy constructor.
     */
    CJoint(const CJoint& other) throw();
    
    /** Virtual copy constructor.
     */
    virtual CJoint* copy() const throw();
    
    /** Virtual destructor.
     */
    virtual ~CJoint();
    //@}
    
    /** Get the world that this joint belongs to.
     */
    CPhysicsWorld* getWorld() const throw() {
      return __world;
    }
    
    /** Get the first body to which this joint is attached.
     */
    CPhysics* getFirstBody() const throw() {
      return __body1;
    }
    
    /** Get the second body to which this joint is attached.
     */
    CPhysics* getSecondBody() const throw() {
      return __body2;
    }
    
    /** Get the internal joint ID.
     */
    dJointID getJointID() const throw() {
      return __joint_id;
    }
    
    /** Attach this joint to a pair of bodies.
     */
    void attach(CPhysics* body1, CPhysics* body2) throw();
    
    /** Get the feedback forces and torques applied to this joint.
     */
    void getFeedback(Ogre::Vector3& f1, Ogre::Vector3& t1,
		     Ogre::Vector3& f2, Ogre::Vector3& t2) const throw();

  private:
    /** Serialization method.
     */
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
      ar & boost::serialization::make_nvp<CPhysicsWorld*>("world", __world);
      ar & boost::serialization::make_nvp<CPhysics*>("body1", __body1);
      ar & boost::serialization::make_nvp<CPhysics*>("body2", __body2);
    }
    
  private:
    CPhysicsWorld* __world;
    
    CPhysics* __body1;
    CPhysics* __body2;
    
    dJointID __joint_id;
  };
  
}

#endif
