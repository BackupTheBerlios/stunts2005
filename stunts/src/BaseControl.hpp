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


#ifndef _STUNTS_CONTROL_BASE_OBJECT_HPP_
#define _STUNTS_CONTROL_BASE_OBJECT_HPP_

#include <nrEngine/nrEngine.h>
#include <string>

namespace stunts {

	/**
	 * Base class for all controller used in the game. Each controller should be derived
	 * from this one to implement functions of controling interface. There are more types
	 * of controlers in the current version: Human/Interactive and AI.
	 * Human controller is used by the player, to control any kind of object in the game. 
	 * This one can also be subdivided for mouse, keyboard, joystick controller.
	 * AI controller is used by underlying artificial inteligence to control objects.
	 **/ 
	class CBaseControl{
		public:
			//! Default constructor
			CBaseControl();
			
			//! Virtual default destructor to allow deriving from this class
			virtual ~CBaseControl();
			
			/**
			 * This method is used to create a appropriate controller type by only
			 * knowing the name of the controller. In this way you are able to bind
			 * controllers to objects only by defining the name of the type of controller
			 * to be bound. This allows us simple definition of level data and scripting.
			 * @param ctrlType Unique type name of the controller
			 * @return either instance of the controller object or NULL if such one does not supported.
			 **/
			static CBaseControl* createInstance(std::string ctrlType);
			
			/**
			 * Get the controller type name.
			 */
			static const char* getControlType() { return "BaseControl"; }
		 
	};
}

#endif
