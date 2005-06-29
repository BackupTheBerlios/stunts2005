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

#ifndef _C_CAROBJECT_H
#define _C_CAROBJECT_H

#include "InteractiveObject.hpp"
#include "Engine.hpp"
#include "GearBox.hpp"
#include "Wheel.hpp"

#include <vector>
#include <boost/shared_ptr.hpp>

namespace stunts {

	class CCarObject : public CBaseObject
	{
		public:
			CCarObject();

			/**
			* Constructor of Car Object
			*
			* @params xmlSettingsString, String with Settings in XML Format
			*
			* @return none
			*/
			CCarObject(char* xmlSettingsString, const std::string& xmlPath);


			/**
			* Deconstructor of Car Object
			*
			* @params none
			*
			* @return none
			*/                  
			~CCarObject();                

			bool 		brake(float brakePedal);
			bool 		steer(float gasPedal);


			static	const char* getObjectTypeSt() 		{ return "car"; }
			const char* getObjectType() 			{ return CCarObject::getObjectTypeSt(); }
       
			// Pointer to gearbox and engine
			boost::shared_ptr<CGearBox>	getGearbox() 	{ return m_Gear; }
			boost::shared_ptr<CEngine>	getEngine()	{ return m_Engine; }

			// Functions for gearbox
			bool				shiftUp()	{ return this->m_Gear->shiftUp(); }
			bool				shiftDown()	{ return this->m_Gear->shiftDown(); }


			// Funktions for engine
			inline	bool				accellerate(float throttle)
				{ return this->m_Engine->accellerate(throttle); }
			inline	int	getRpm()	{ return this->m_Engine->getRpm(); }
			inline	int	getMinRpm()	{ return this->m_Engine->getMinRpm(); }
			inline	int	getMaxRpm()	{ return this->m_Engine->getMaxRpm(); }

			//ODE object
			inline	boost::shared_ptr<OgreOde_Prefab::Vehicle>	ODEVehicle()
				{ return this->mVehicle; }


		protected:
			//------------------ Methods --------------------------------------
			//! @see CBaseObject::parseSettings()
			bool parseSettings(TiXmlElement* rootElem, const std::string& xmlPath);

			//------------------ Variables --------------------------------------

			// Gearbox of car
			boost::shared_ptr<CGearBox>	m_Gear;

			// Engine of car
			boost::shared_ptr<CEngine>	m_Engine;

			// Vector with carwheels
			std::vector<boost::shared_ptr<CWheel> >	wheels;

			// brakepedal value
			float		m_brake;

			//ODE description of the car
			boost::shared_ptr<OgreOde_Prefab::Vehicle> mVehicle;
	};
};
#endif
