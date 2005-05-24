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


#ifndef _STUNTS_PHYSICS_WORLD_INFO_HPP_
#define _STUNTS_PHYSICS_WORLD_INFO_HPP_

#include "PhysicsAutoDisable.hpp"
#undef PI
#undef EPSILON
#include <OGRE/Ogre.h>

namespace stunts {
  
  /** A game object which contains information about the global
   * physics of a world. Each world has the following global
   * attributes:
   *  - ERP value (must be between 0 and 1)
   *  - CFM value (constraint-force-mixing value, must be a small number)
   *  - Gravity vector
   */
  class CPhysicsWorldInfo
    : public CPhysicsAutoDisable
  {
  public:
    /** Construct a %PhysicsWorldInfo with default attributes.
     * - The ERP value is 0.2
     * - The CFM value is 10^(-5)
     * - The gravity vector is null
     */
    CPhysicsWorldInfo() throw()
      : __erp(0.2f),
	__cfm(0.00001f),
	__gravity(0,0,0)
    {}
    
    /** Virtual copy constructor.
     */
    virtual CPhysicsWorldInfo* copy() const throw() {
      return new CPhysicsWorldInfo(*this);
    }
    
    /** Destructor.
     */
    ~CPhysicsWorldInfo() throw() {}
    
    /** Get the ERP value.
     */
    float getERP() const throw() {
      return __erp;
    }
    
    /** Get the CFM value.
     */
    float getCFM() const throw() {
      return __cfm;
    }
    
    /** Get the gravity vector.
     */
    const Ogre::Vector3& getGravity() const throw() {
      return __gravity;
    }
    
    /** Set the ERP value.
     */
    void setERP(float erp) throw() {
      __erp = erp;
    }
    
    /** Set the CFM value.
     */
    void setCFM(float cfm) throw() {
      __cfm = cfm;
    }
    
    /** Set the gravity vector.
     */
    void setGravity(const Ogre::Vector3& gravity) throw() {
      __gravity = gravity;
    }
    
  private:
    /** Serialization method.
     */
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
      ar & boost::serialization::base_object<CPhysicsAutoDisable>(make_nvp("auto_disable", *this));
      
      ar & make_nvp("erp", __erp);
      ar & make_nvp("cfm", __cfm);
      ar & make_nvp("gravity", __gravity);
    }
    
  private:
    float __erp;
    float __cfm;
    Ogre::Vector3 __gravity;
  };
  
}

#endif
