/* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup, 
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Art Tevs <tevs@mpi-sb.mpg.de>
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


#ifndef _STUNTS_CONTROL_HUMAN_OBJECT_HPP_
#define _STUNTS_CONTROL_HUMAN_OBJECT_HPP_

#include "BaseControl.hpp"

namespace stunts {

	/**
	 * This is a class of a human controller used in the game. Human controller
	 * is used to allow user to interact within the game.
	 **/ 
	class CHumanControl: public CBaseControl{
		public:
			//! Default constructor
			CHumanControl();
			
			//! Virtual default destructor to allow deriving from this class
			virtual ~CHumanControl();
			
			
			/**
			 * Get the controller type name.
			 */
			static const char* getControlType() { return "Human"; }
			 
	};
}

#endif
