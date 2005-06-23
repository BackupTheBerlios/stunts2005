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

#ifndef MOTOR_H
#define MOTOR_H

namespace stunts {
	
	class CEngine
	{
		public:
			/**
			* Constructor of engine
			*
			* @param minRpm Minimum rpm of engine
			* @param maxRpm Maximum rpm of engine
			* 
			* @return nothing
			*/
			CEngine(int minRpm, int maxRpm);
	
	
			
			
			/**
			* Deconstructor of engine
			*
			* @param none
			*
			* @return nothing
			*/
			~CEngine();
	
	
	
			/**
			* Function to increase rpm
			*
			* @param throttle, value of throttle-pedal
			*
			* @return true: accelleration is possible, false if not
			*/
			bool	accellerate(float throttle);
			
	//	protected:
			// Current rpm of engine
			int	m_rpm;
	
			// Minimum rpm of engine
			int	m_minRpm;
	
			// Maximum rpm of engine
			int	m_maxRpm;
	
			// If engine is damaged...
			float	m_damage;
	};
};
#endif
