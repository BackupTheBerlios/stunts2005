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

#include "Engine.hpp"

namespace stunts {
	
	/**
	* Constructor of engine
	*
	* @param minRpm Mimimum rpm of engine
	* @param maxRpm Maximum rpm of engine
	*
	* @return nothing
	*/
	CEngine::CEngine(int minRpm=800, int maxRpm=6200)
	{
		// Set value of started engine to minRpm
		this->m_rpm	= minRpm;
	
		// Save minRpm
		this->m_minRpm	= minRpm;
	
		// Save maxRpm
		this->m_maxRpm	= maxRpm;
	
		// Set damage to 0
		this->m_damage	= 0;
	};
	
	
	
	
	/**
	* Deconstructor of engine
	*
	* @param none
	*
	* @return nothing
	*/
	CEngine::~CEngine()
	{
	
	};
	
	
	
	/**
	* accellerate, increase motorRPM
	*
	* @args trottle, value how much throttle is given
	* 	0=0% ; 0.5=50 ; 1=100%
	* 	and so on...
	*
	* @return bool, if accelleration is possible true, else false
	*/
	
	bool CEngine::accellerate(float throttle)
	{
		if (this->m_rpm <= this->m_minRpm) return false;
		if (this->m_rpm >= this->m_maxRpm)  return false;
		this->m_rpm +=25;
	};
};

