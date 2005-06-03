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


#ifndef _STUNTS_PHYSICS_AUTO_DISABLE_HPP_
#define _STUNTS_PHYSICS_AUTO_DISABLE_HPP_

#include "Time.hpp"

#include <boost/serialization/serialization.hpp>

namespace stunts {
  
  /** Auto-disable policies for physical objects.  This structure
   * describes policies for automatically disabling physical objects
   * that are motionless.  An object is considered motionless if
   * certain conditions are met whose parameters are defined by this
   * structure.  An object must also be motionless for a certain time
   * or a certain number of steps before it is disabled automatically.
   * It is also possible to enable or disable the auto-disable
   * feature.
   *
   * An object is considered motionless if its absolute velocity is
   * below the linear threshold and its angular velocity is below the
   * angular threshold given by this structure. An object is
   * automatically disabled if the auto-disable flag is set and the
   * object has been motionless for either at least the number of
   * steps or the amount of time given by this structure.
   */
  class CPhysicsAutoDisable {
  public:
    /** Construct default auto-disable policies.
     * - The auto-disable flag is @c false
     * - The linear threshold is 0.01
     * - The angular threshold is 0.01
     * - The number of steps is 10
     * - The time is zero
     */
    CPhysicsAutoDisable() throw()
      : __auto_disable(false),
	__linear_threshold(0.01f),
	__angular_threshold(0.01f),
	__steps(10),
	__time(0)
    {}
    
    /** Virtual copy constructor.
     */
    virtual CPhysicsAutoDisable* copy() const throw() {
      return new CPhysicsAutoDisable(*this);
    }
    
    /** Virtual destructor.
     */
    virtual ~CPhysicsAutoDisable() throw() {}
    
    /** Get the auto-disable flag.
     */
    bool getAutoDisableFlag() const throw() {
      return __auto_disable;
    }
    
    /** Get the auto-disable linear threshold.
     */
    float getAutoDisableLinearThreshold() const throw() {
      return __linear_threshold;
    }
    
    /** Get the auto-disable angular threshold.
     */
    float getAutoDisableAngularThreshold() const throw() {
      return __angular_threshold;
    }
    
    /** Get the auto-disable number of steps.
     */
    unsigned int getAutoDisableSteps() const throw() {
      return __steps;
    }
    
    /** Get the auto-disable time.
     */
    const CTime& getAutoDisableTime() const throw() {
      return __time;
    }
    
    /** Set the auto-disable flag.
     */
    void setAutoDisableFlag(bool auto_disable) throw() {
      __auto_disable = auto_disable;
    }
    
    /** Get the auto-disable linear threshold.
     */
    void setAutoDisableLinearThreshold(float linear_threshold) throw() {
      __linear_threshold = linear_threshold;
    }
    
    /** Get the auto-disable angular threshold.
     */
    void setAutoDisableAngularThreshold(float angular_threshold) throw() {
      __angular_threshold = angular_threshold;
    }

    /** Set the auto-disable number of steps.
     */
    void setAutoDisableSteps(unsigned int steps) throw() {
      __steps = steps;
    }
    
    /** Set the auto-disable time.
     */
    void setAutoDisableTime(const CTime& time) throw() {
      __time = time;
    }
    
  private:    
    /** Serialization method.
     */
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
      ar & boost::serialization::make_nvp<bool>("auto_disable", __auto_disable);
      ar & boost::serialization::make_nvp<float>("auto_disable_linear_threshold", __linear_threshold);
      ar & boost::serialization::make_nvp<float>("auto_disable_angular_threshold", __angular_threshold);
      ar & boost::serialization::make_nvp<unsigned int>("auto_disable_steps", __steps);
      ar & boost::serialization::make_nvp<CTime>("auto_disable_time", __time);
    }
    
  private:
    bool __auto_disable;
    float __linear_threshold;
    float __angular_threshold;
    unsigned int __steps;
    CTime __time;
  };
  
}

#endif
