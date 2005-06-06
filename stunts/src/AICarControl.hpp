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


#ifndef _STUNTS_CONTROL_AI_CAR_OBJECT_HPP_
#define _STUNTS_CONTROL_AI_CAR_OBJECT_HPP_

#include "BaseObject.hpp"

namespace stunts {

	/**
	 * This is a class representing artificial inteligence controller for the cars.
	 **/ 
	class CAICarControl: public CBaseControl{
		public:
			//! Default constructor
			CAICarControl();
			
			//! Virtual default destructor to allow deriving from this class
			virtual ~CAICarControl();
				
			/**
			 * Get the controller type name.
			 */
			static const char* getControlType() { return "AICar"; }
		 
	};
}

#endif
