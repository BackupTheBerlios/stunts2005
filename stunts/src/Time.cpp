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


#include "Time.hpp"

namespace stunts {
  
  CTime::CTime() throw() {
    reset();
  }
  
  CTime::CTime(Infinite inf) throw() {
    reset(inf);
  }
  
  CTime::CTime(int seconds, int nanoseconds) throw() {
    reset(seconds, nanoseconds);
  }
  
  CTime::CTime(double seconds) throw() {
    reset(seconds);
  }
  
  void CTime::reset() throw() {
    __infinite = false;
    __seconds = 0;
    __nanoseconds = 0;
  }
  
  void CTime::reset(Infinite inf) throw() {
    __infinite = true;
    __nanoseconds = 0;
    if(inf == INFINITE) {
      __seconds = 1;
    } else {
      __seconds = -1;
    }
  }
  
  void CTime::reset(int seconds, int nanoseconds) throw() {
    while(nanoseconds < 0) {
      nanoseconds += NANOSECONDS_PER_SECOND;
      --seconds;
    }
    while(nanoseconds >= NANOSECONDS_PER_SECOND) {
      nanoseconds -= NANOSECONDS_PER_SECOND;
      ++seconds;
    }
    
    __infinite = false;
    __seconds = seconds;
    __nanoseconds = nanoseconds;
  }
  
  void CTime::reset(double seconds) throw() {
    __seconds = static_cast<int>(seconds);
    __nanoseconds = static_cast<int>((seconds - __seconds) * NANOSECONDS_PER_SECOND);
  }
  
  CTime CTime::operator-() const throw() {
    CTime temp(*this);
    temp.negate();
    return temp;
  }
  
  void CTime::negate() throw() {
    __seconds = -__seconds;
    if(__nanoseconds > 0) {
      __nanoseconds = NANOSECONDS_PER_SECOND - __nanoseconds;
      --__seconds;
    }
  }
  
  CTime CTime::operator*(double factor) const throw() {
    if(isFinite()) {
      double new_seconds = getDouble() * factor;
      return CTime(new_seconds);
    } else if(factor == 0.0) {
      return CTime();
    } else if(isPositive() == (factor >= 0)) {
      return *this;
    } else {
      return -(*this);
    }
  }
  
  CTime CTime::operator/(double factor) const throw() {
    if(isFinite()) {
      double new_seconds = getDouble() / factor;
      return CTime(new_seconds);
    } else if(factor == 0.0) {
      return CTime();
    } else if(isPositive() == (factor >= 0)) {
      return *this;
    } else {
      return -(*this);
    }
  }
  
  const CTime& CTime::operator+=(const CTime& time) throw() {
    if(!isInfinite() && !time.isInfinite()) {
      __seconds += time.__seconds;
      __nanoseconds += time.__nanoseconds;
      if(__nanoseconds < 0) {
	__nanoseconds += NANOSECONDS_PER_SECOND;
	--__seconds;
      } else if (__nanoseconds >= NANOSECONDS_PER_SECOND) {
	__nanoseconds -= NANOSECONDS_PER_SECOND;
	++__seconds;
      }
    } else if(isInfinite() && time.isInfinite()) {
      if(isPositive() != time.isPositive()) {
	reset();
      }
    } else if(time.isInfinite()) {
      *this = time;
    }
    return *this;
  }
  
  const CTime& CTime::operator-=(const CTime& time) throw() {
    if(!isInfinite() && !time.isInfinite()) {
      __seconds += time.__seconds;
      __nanoseconds += time.__nanoseconds;
      if(__nanoseconds < 0) {
	__nanoseconds += NANOSECONDS_PER_SECOND;
	--__seconds;
      } else if (__nanoseconds >= NANOSECONDS_PER_SECOND) {
	__nanoseconds -= NANOSECONDS_PER_SECOND;
	++__seconds;
      }
    } else if(isInfinite() && time.isInfinite()) {
      if(isPositive() == time.isPositive()) {
	reset();
      }
    } else if(time.isInfinite()) {
      *this = time;
      negate();
    }
    return *this;
  }
  
  std::ostream& operator<<(std::ostream& ostr, const CTime& time) {
    if(time.__infinite) {
      ostr << "INFINITE";
    } else {
	ostr << time.__seconds << "sec " << time.__nanoseconds << "nsec";
    }
  }
  
}
