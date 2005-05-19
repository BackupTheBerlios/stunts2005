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

#include <OGRE/Ogre.h>
#include <../../nrEngine/nrEngine.h>

#ifndef _STUNTS_GAME_APPLICATION_HPP_
#define _STUNTS_GAME_APPLICATION_HPP_

#include "UserInput.hpp"

namespace stunts {
  
  /** The main application of the game.
   */
  class GameApplication {
  public:
    /** Construct a new %GameApplication.
     */
    GameApplication();
    
    /** Destructor.
     */
    virtual ~GameApplication();
    
    /** Run the application.
     */
    virtual void run();
    
    /** Initialize the application.
     */
    virtual void initialize();
    
  private:
    Ogre::Root* __root;
  };
  
}

#endif
