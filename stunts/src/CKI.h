/* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup,
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Peter Kaczmarczyk
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
			CKI(CLevel* level);

			virtual ~CKI();

			// Execution
			void executeKI (float delaySeconds);

			// setter
			void CKI::setLevelOfSkill (float levelSkill)		{this->levelOfSkill = levelSkill;}
			void CKI::setLevelOfAggressivity (float levelAggro)	{this->levelOfAggressivity = levelAggro;}
			void CKI::setLevelOfReaction (float levelReac)		{this->levelOfReaction = levelReac;}

			// getter
			float* getLevelOfSkill()			{	return &levelOfSkill;	}
			float* getLevelOfAggressivity()	{	return &levelOfAggressivity;	}
			float* getLevelOfReaction()		{	return &levelOfReaction;	}


			float* steerAngle()				{	return &msteerAngle;	};
			float* Speed()						{	return &carSpeed;	}


			virtual const char* taskGetName() {return "KITask";}

		protected:

			static const bool debug_ki = false;
			
			//sums the time
			float l_time;
			
			float steer;
			float acc;
			float brake;

			float waypointTime;
			float stayTime;


			virtual nrResult taskInit();
			virtual nrResult taskStart();
			virtual nrResult taskUpdate();
			virtual nrResult taskStop();


			CLevel*											mLevel;
			std::vector<boost::shared_ptr<CBaseObject> >	mObjects;
			boost::shared_ptr<CCarObject>					controlObject;
			boost::shared_ptr<CWaypoint>					waypoint;

			CNeuralNetwork* net;

			float msteerAngle;
			float carSpeed;


			float levelOfSkill;
			float levelOfAggressivity;
			float levelOfReaction;


			void actualizeWaypoint(float delaySeconds);
			void actualizeCarSpeed();


			void computeStrategy();

			
			//void lowrider(float delaySeconds);
	

			void computeGear();
			void computeDirection();
			void computeAcceleration();
	};
}


#endif

