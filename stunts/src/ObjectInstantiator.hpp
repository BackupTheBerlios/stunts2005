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


#ifndef _C_BASEOBJECT_INSTANTIATOR_H
#define _C_BASEOBJECT_INSTANTIATOR_H

#include "BaseObject.hpp"
#include "CarObject.hpp"
#include "Wheel.hpp"

#include <string>

namespace stunts {

	/** 
	 * Creates an instance of an object, by given it's object type name
	 **/
	class CObjectInstantiator
	{
		public:
		
			/**
			* This will create an object by its type name. So we provide here a function with which one 
			* you do not have to know which class name you should use to create an object. 
			* Simply give me the name of the object type and I will return you the interface.
			* @param objType Unique object type name
			* @return Interfacve of the object according to the given name or NULL if no such
			* object type is supported
			* @note You have to delete the object later to prevent memory leaks
			**/
			static CBaseObject*	createInstance(std::string objType){
				
				// check whenever we have valid parameter
				if (objType.length() == 0) return NULL;
					
				// check whenever we do support this kind of object type
				if (objType == CBaseObject::getObjectTypeSt())
					return new CBaseObject();
				else if (objType == CCarObject::getObjectTypeSt())
					return new CCarObject();
				else if (objType == CWheel::getObjectTypeSt())
					return new CWheel();
				
				return NULL;
			}
			
	};
};

#endif
