/* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup,
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Christian Morbach <GameDevelopment@paratronic.de>
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


#ifndef CKI_h
#define CKI_h


//------------------------------------------------------------------------------
//--- predeclare this class
//------------------------------------------------------------------------------
namespace stunts
{
	class CKI;
}


//------------------------------------------------------------------------------
//--- includes
//------------------------------------------------------------------------------
#include "CNeuralNetwork.h"
#include "Level.hpp"
//#include "InteractiveObject.hpp"
#include "GearBox.hpp"
#include "Engine.hpp"
#include "CarObject.hpp"
#include "BaseObject.hpp"
 
 
namespace stunts
{ 

	class CKI : public nrITask
	{
	
		 public:
			// Default constructor and destructor
			CKI(CCarObject* Object, String Typ);
			CKI(boost::shared_ptr< CLevel > level);
		
			virtual ~CKI();
		
			// Execution
			void executeKI (float delaySeconds);
			
			// setter
			void setLevelOfSkill		(int levelSkill);
			void setLevelOfAggressivity	(int levelAggro);
			void setLevelOfReaction	(int levelReac);
		 
			// getter
			int getLevelOfSkill();
			int getLevelOfAggressivity();
			int getLevelOfReaction();
		 
			virtual const char* taskGetName() {return "KITask";}
							
		protected:
			virtual nrResult taskInit();
			virtual nrResult taskStart();
			virtual nrResult taskUpdate();
			virtual nrResult taskStop();


			boost::shared_ptr< CLevel >						mLevel;
			std::vector<boost::shared_ptr<CBaseObject> >	mObjects;
			boost::shared_ptr<CCarObject>					controlObject;

			CNeuralNetwork* net;
		
			int levelOfSkill;
			int levelOfAggressivity;
			int levelOfReaction;
			int SteerAngle;
		
		
			void computeStrategy();
			
			boost::shared_ptr<CWaypoint> waypoint;
			
			void computeGear();
			void computeDirection();
			void computeAcceleration();
	};
}
	
	
#endif

