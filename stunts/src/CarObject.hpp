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

			
			bool 				steer(float gasPedal);


			static	const char* getObjectTypeSt() 
					{ return "car"; }
			const char* 	getObjectType()   
					{ return CCarObject::getObjectTypeSt(); }


			inline float Speed() const
			{
				if(mVehicle != NULL)
				{
					if(mVehicle->getWheel(0) != NULL)
					{
						return  ( mVehicle->getWheel(0)->getRPM()  );
					}
				}
				// was nicht da ist hat auch keine Speed
				return 0.f;
			};
			
			
			inline Quaternion	Orientation() const	{ return mVehicle->getOrientation(); };
			inline Vector3	Position() const		{ return mVehicle->getPosition(); };
			
			
			// Functions for gearbox & engine
			inline void	shiftUp()
					{ this->mVehicle->getEngine()->changeUp();				}
			inline void	shiftDown()
					{ this->mVehicle->getEngine()->changeDown();				}
			inline void	setGear(unsigned int i)
					{ this->mVehicle->getEngine()->setGear(i);				}
			inline void 	setAutoBox(bool automatic)
					{ this->mVehicle->getEngine()->setAutoBox(automatic);			}


			// Funktions for engine
			//inline void	accellerate(float throttle)
			//		{ this->mVehicle->getEngine()->setInputs(throttle, 0.0);		}
			//inline void 	brake(float brake)
			//		{ this->mVehicle->getEngine()->setInputs(0.0, brake);			}
			//inline void 	accellerate_brake(float throttle, float brake)
			//		{ this->mVehicle->setInputs(0.f, throttle, brake); 		}
			inline void 	setInputs(float steer, float throttle, float brake)
					{ this->mVehicle->setInputs(steer, throttle, brake); 		}
			
			
			//ODE object
			inline boost::shared_ptr<OgreOde_Prefab::Vehicle>	ODEVehicle()
					{ return this->mVehicle; 						}


		protected:
			//------------------ Methods --------------------------------------
			//! @see CBaseObject::parseSettings()
			bool parseSettings(TiXmlElement* rootElem, const std::string& xmlPath);

			//------------------ Variables --------------------------------------

			//ODE description of the car
		public:
			boost::shared_ptr<OgreOde_Prefab::Vehicle> 	mVehicle;
	};
};
#endif
