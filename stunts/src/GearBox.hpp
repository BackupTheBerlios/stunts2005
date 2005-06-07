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

#ifndef GEARBOX_H
#define GEARBOX_H

#include <vector>

namespace stunts {
	
	class CGearBox
	{
		public:
			/** 
			* Constructor of GearBox
			*
			* @param numberOfGears Number of Gears
			* @param gearTransmission Vector of transmission for each gear
			*
			* @return nothing
			*/
			
			CGearBox(int numberOfGears, std::vector<float> gearTransmission);
		
	
			/**
			* Deconstructor of Gearbox
			*
			* @param none
			*
			* @return nothing
			*/
			~CGearBox();
			
			/**
			* Function to shift up
			*
			* @param none
			*
			* @return true: success; false: if not
			*/
			bool			shiftUp();
	
			/**
			* Function to shift down
			*
			* @param none
			*
			* @return true: success; false: if not
			*/
			bool			shiftDown();
	
			/**
			* Function to get current gearTransmission
			*
			* @param none
			*
			* @return transmission of current gear
			*/
			float			getTransmission();
			
		protected:
			// Contains current selected gear
			int			m_currentGear;
	
			// Contains number of Gears
			int			m_numberOfGears;
	
			// Contains vector with transmisson of each gear
			std::vector<float>	m_gearTransmission;
	};
};
#endif

