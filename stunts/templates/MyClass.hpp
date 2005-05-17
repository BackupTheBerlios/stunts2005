/* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup, 
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Firstname Lastname <email@domain.org>
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


#ifndef __STUNTS_MY_CLASS_HPP_
#define __STUNTS_MY_CLASS_HPP_

namespace stunts {
  
  /** An example class. This file is a template for header files in
   * this project. Note that the GPL header above contains a
   * maintainer name.  If you create a new file, you might also want
   * to enter your name as maintainer.
   */
  class MyClass {
  public:
    /** Default constructor.
     */
    MyClass();
    
    /** Get Something.
     */
    int getSomething() const throw() { return __something; }
    
  private:
    int __something;          ///< Something.
  };
  
}

#endif
