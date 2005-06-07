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

#include "GearBox.hpp"

namespace stunts {
	
	/**
	* Constructor
	*
	* @args numberOfGears, gearTransmission
	*
	* @return none
	*
	*/
	CGearBox::CGearBox(int numberOfGears, std::vector<float> gearTransmission)
	{
		this->m_numberOfGears 		=	numberOfGears;
		this->m_gearTransmission 	=	gearTransmission;
	}
	
	
	/**
	* Deconstructor
	*
	* @args none
	*
	* @return none
	*
	*/
	
	CGearBox::~CGearBox()
	{
		
	}
	
	
	
	/**
	* shiftUp, if possible increase m_currentGear
	*
	* @args none
	*
	* @return true if possible, false if not
	*
	*/
	
	bool CGearBox::shiftUp()
	{
		if (m_currentGear == m_numberOfGears) return false;
		m_currentGear++;
		return true;
	}
	
	
	
	/**
	* shiftDown, if possible decrease m_currentGear
	*
	* @args none
	*
	* @return true if possible, false if not
	*
	*/
	
	bool CGearBox::shiftDown()
	{
		if (m_currentGear == 1) return false;
		m_currentGear--;
		return true;
	}
	
	
	
	/**
	* getTransmission, return transmission of current gear
	*
	* @args none
	*
	* @return float value of current gear-transmission
	*
	*/
	
	float CGearBox::getTransmission()
	{
		return (this->m_gearTransmission)[this->m_currentGear-1];	
	}
};