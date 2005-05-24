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


#ifndef _STUNTS_TIME_HPP_
#define _STUNTS_TIME_HPP_

#include <climits>
#include <iostream>

#include <boost/serialization/serialization.hpp>

namespace stunts {
  
  /** This class represents an absolute time or a relative time
   * interval in seconds and nanoseconds.  The actual representation
   * is hidden (but methods are provided to access the seconds and
   * nanoseconds part).  The time interval can also be positive or
   * negative infinite.
   */
  class CTime {
  public:
    /** Type of infinity, used for constructing infinite time intevals.
     */
    enum Infinite {
      /** Positive infinite time interval.
       */
      INFINITE,
      
      /** Negative infinite time interval.
       */
      NEGATIVE_INFINITE
    };
    
    /** Number of nanoseconds per second.
     */
    static const int NANOSECONDS_PER_SECOND = 1000000000;
    
  public:
    /** Construct a zero time interval.
     */
    CTime() throw();
    
    /** Construct an infinite time interval.
     */
    CTime(Infinite) throw();
    
    /** Construct a finite time interval from seconds and nanoseconds.
     */
    CTime(int seconds, int nanoseconds) throw();
    
    /** Construct a finite time interval from a real value in seconds.
     */
    explicit CTime(double seconds) throw();
    
    /** Destructor.
     */
    ~CTime() throw() {}
    
    /** Check if this time interval is zero.
     */
    bool isZero() const throw();
    
    /** Check if this time interval is negative.
     */
    bool isNegative() const throw();
    
    /** Check if this time interval is positive.
     */
    bool isPositive() const throw();
    
    /** Check if this time interval is finite.
     */
    bool isFinite() const throw();
    
    /** Check if this time interval is infinite.
     */
    bool isInfinite() const throw();
    
    /** Get seconds part of this time interval.
     */
    int getSeconds() const throw();
    
    /** Get nanoseconds part of this time interval.
     */
    int getNanoseconds() const throw();
    
    /** Convert this time interval to seconds represented as @c float.
     */
    float getFloat() const throw();
    
    /** Convert this time interval to seconds represented as @c double.
     */
    double getDouble() const throw();
    
    /** Set this time interval to zero.
     */
    void reset() throw();
    
    /** Set this time interval to an infinite time interval.
     */
    void reset(Infinite) throw();
    
    /** Set this time interval to a finite time interval from seconds and nanoseconds.
     */
    void reset(int seconds, int nanoseconds) throw();
    
    /** Set this time interval to a finite time interval from a real value in seconds.
     */
    void reset(double seconds) throw();
    
    /** Get the negation of this time interval.
     */
    CTime operator-() const throw();
    
    /** Negate this time interval.
     */
    void negate() throw();
    
    /** Get the product of this time interval and a factor.
     */
    CTime operator*(double factor) const throw();
    
    /** Get the product of this time interval and a factor.
     */
    friend CTime operator*(double factor, const CTime& time) throw();
    
    /** Get the product of this time interval and the inverse of a factor.
     */
    CTime operator/(double factor) const throw();
    
    /** Multiply this time interval with a factor.
     */
    const CTime& operator*=(double factor) throw();
    
    /** Multiply this time interval with the inverse of a factor.
     */
    const CTime& operator/=(double factor) throw();
    
    /** Get the sum of this time interval and another.
     */
    CTime operator+(const CTime& time) const throw();
    
    /** Get the difference of this time interval and another.
     */
    CTime operator-(const CTime& time) const throw();
    
    /** Add another time interval to this time interval.
     */
    const CTime& operator+=(const CTime& time) throw();
    
    /** Subtract another time interval from this time interval.
     */
    const CTime& operator-=(const CTime& time) throw();
    
    /** Display this time interval on an output stream.
     */
    friend std::ostream& operator<<(std::ostream& ostr, const CTime& time);
    
  private:
    /** Serialization method.
     */
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
      ar & make_nvp("infinite", __infinite);
      ar & make_nvp("seconds", __seconds);
      ar & make_nvp("nanoseconds", __nanoseconds);
    }
    
  private:
    bool __infinite;
    int __seconds;
    int __nanoseconds;
  };

  inline bool CTime::isZero() const throw() {
    return !__infinite && __seconds == 0 && __nanoseconds == 0;
  }
  
  inline bool CTime::isNegative() const throw() {
    return __seconds < 0;
  }
  
  inline bool CTime::isPositive() const throw() {
    return __seconds > 0 || (__seconds == 0 && __nanoseconds > 0);
  }
  
  inline bool CTime::isFinite() const throw() {
    return !__infinite;
  }
  
  inline bool CTime::isInfinite() const throw() {
    return __infinite;
  }
  
  inline int CTime::getSeconds() const throw() {
    return __seconds;
  }
  
  inline int CTime::getNanoseconds() const throw() {
    return __nanoseconds;
  }
  
  inline float CTime::getFloat() const throw() {
    return float(__seconds) + float(__nanoseconds) / NANOSECONDS_PER_SECOND;
  }
  
  inline double CTime::getDouble() const throw() {
    return float(__seconds) + float(__nanoseconds) / NANOSECONDS_PER_SECOND;
  }
  
  inline const CTime& CTime::operator*=(double factor) throw() {
    return (*this) = (*this) * factor;
  }
  
  inline const CTime& CTime::operator/=(double factor) throw() {
    return (*this) = (*this) / factor;
  }
  
  inline CTime CTime::operator+(const CTime& time) const throw() {
    CTime temp(*this);
    temp += time;
    return temp;
  }
  
  inline CTime CTime::operator-(const CTime& time) const throw() {
    CTime temp(*this);
    temp -= time;
    return temp;
  }
  
}

#endif
