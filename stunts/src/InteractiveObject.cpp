/* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup, 
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Andreas Maurer <andi@andile.de
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

#include "InteractiveObject.hpp"

namespace stunts {
	
	CInteractiveObject::CInteractiveObject()
	{
	
	}
	
	
	/**
	* Constructor of BaseObject
	*
	* @param char* xmlSettingsString, String with settings in XML format
	* 
	* @return nothing
	* 
	*/
	CInteractiveObject::CInteractiveObject(char* xmlSettingsString) : CBaseObject(xmlSettingsString) 
	{
		// We do not need to specify here more, bacause the base object will call the 
		// appropriate overrided parseSettings mehtod, where we are defining how to
		// parse the settings
	}
	
	
	
	
	/**
	* Deconstrubtor of BaseObject
	*
	* @param none
	*
	* @return nothing
	*/
	CInteractiveObject::~CInteractiveObject()
	{
	
	}
};

