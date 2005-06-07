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
#include "Engine.hpp"
#include "GearBox.hpp"
#include "Wheel.hpp"


#ifndef CCAROBJECT_H
#define CCAROBJECT_H

namespace stunts {
	
	class CCarObject : public CInteractiveObject
	{
		public:
			/**
			* Constructor of Car Object
			*
			* @params xmlSettingsString, String with Settings in XML Format
			*
			* @return none
			*/
			CCarObject(char* xmlSettingsString);
	
	
			/**
			* Deconstructor of Car Object
			*
			* @params none
			*
			* @return none
			*/
			~CCarObject();
	
			bool 		brake();
			bool 		steer();




			/**
			 * Import the file which has got declaration of the car object.
			 * @param fileName Name of the file containing object data
			 * @return false if the reading was successfull
			 **/
			bool importFromFile(const char* fileName);	



			
			/**
			 * Get the object type name.
			 */
			static const char* getObjectType() { return "car"; }


		protected:
			// Gearbox of car
			CGearBox	m_Gear(int numberOfGears, vector<float>* gearTransmission);

			// Engine of car
			CEngine		m_Engine(int minRpm, int maxRpm);

			// Vector with car wheels
			vector<CWheel>	wheels(char* xmlSettingsString);

			// brakepedal value
			bool		m_brake(float brakepedal);
	};
};	
#endif
