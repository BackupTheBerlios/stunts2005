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

#ifndef WHEEL_H
#define WHEEL_H

#include "BaseObject.hpp"

namespace stunts {

	class CWheel : public CBaseObject
	{
		public:
			CWheel();
			
			/**
			* Constructor of wheel object
			*
			* @param xmlSettingsString Contains Settings in XML format
			*
			* @return nothing
			*/
			CWheel(char* xmlSettingsString, const std::string& xmlPath);

			/**
			* Deconstructor of wheel object
			*
			* @param none
			*
			* @return nothing
			*/
			~CWheel();

			
			static	const char* getObjectTypeSt() 	{ return "wheel"; }
					const char* getObjectType() 	{ return CWheel::getObjectTypeSt(); }

	
					
		protected:
			// Setting if this wheel is driven (angetrieben)
			bool	m_isDrivewheel;
	
			// Setting if this wheel is steerable
			bool	m_isSteerable;
	
			// Friction Coefficient (Reibungskoeffizient) of this wheel
			float	m_frictionCoefficient;
	};
};
#endif
