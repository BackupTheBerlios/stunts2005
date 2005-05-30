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


#ifndef _STUNTS_PHYSICS_HPP_
#define _STUNTS_PHYSICS_HPP_

#include "PhysicsAutoDisable.hpp"

#include <ode/ode.h>

namespace stunts {
  
  class CPhysicsWorld;
  
  /** A Physics component of a game object.
   */
  class CPhysics {
  public:
    /** @name Constructor & Destructor
     */
    //@{
    /** Construct a physics object.
     */
    CPhysics() throw();
    
    /** Copy constructor.
     */
    CPhysics(const CPhysics& other) throw();
    
    /** Virtual copy constructor.
     */
    virtual CPhysics* copy() const throw();
    
    /** Virtual destructor.
     */
    virtual ~CPhysics();
    //@}
    
    /** @name Access Methods
     */
    //@{
    /** Get the physics world which owns this object.
     */
    CPhysicsWorld* getPhysicsWorld() const throw() {
      return __world;
    };
    
    /** Get the internal body ID.
     */
    dBodyID getBodyID() const throw() {
      return __body_id;
    }
    //@}
    
  private:
    /** Serialization method.
     */
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
      ar & boost::serialization::base_object<CPhysicsAutoDisable>(make_nvp("auto_disable", *this));
      
      ar & make_nvp("world", __world);
    }
    
  private:
    CPhysicsWorld* __world;     ///< Physics world owning this object.
    dBodyID __body_id;          ///< ODE Body ID.
  };
  
}

#endif
