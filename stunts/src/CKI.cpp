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

#include "CKI.h"

namespace stunts
{
	//--------------------------------------------------------------------------
	//--- CKI()
	//--------------------------------------------------------------------------
	CKI::CKI(boost::shared_ptr< CLevel > level)
	{
		mLevel = level;

		levelOfSkill = 1;
		levelOfAggressivity = 1;
		levelOfReaction = 1;

//		controlObject.reset(Object);

		net = new CNeuralNetwork(this);
		net->makeNetwork();
		
		carSpeed = 0.f;
		steer = 0.f;
		acc = 0.f;
		brake = 0.f;
		
		waypointTime = 0.f;
	}


	//--------------------------------------------------------------------------
	//--- taskInit()
	//--------------------------------------------------------------------------
	nrResult CKI::taskInit()
	{
		return NR_OK;
	}


	//--------------------------------------------------------------------------
	//--- taskStart()
	//--------------------------------------------------------------------------
	nrResult CKI::taskStart()
	{
		return NR_OK;
	}


	//--------------------------------------------------------------------------
	//--- taskUpdate()
	//--------------------------------------------------------------------------
	nrResult CKI::taskUpdate()
	{
		if (controlObject == NULL)
		{
			controlObject.reset( (CCarObject*) mLevel->getCar(0).get() );
			/*
			mObjects = mLevel->Objects();

			if(mObjects.size() != 0)
			{
				for(int i=0; i<mObjects.size(); i++)
				{
					if (mObjects.at(i)->getName().compare("AICar") == 0)
					{
						controlObject.reset( (CCarObject*)mObjects.at(i).get() );
						break;
					}
				}
			}
			*/
		}
		else
		{
			//react here only in this example application
			//	as no InteractiveObject is running
			this->executeKI(COgreTask::GetSingleton().mTimer->getFrameInterval());
		}

		return NR_OK;
	}


	//--------------------------------------------------------------------------
	//--- taskStop()
	//--------------------------------------------------------------------------
	nrResult CKI::taskStop()
	{
		return NR_OK;
	}








	/**
	* legt eine KI f?r ein interaktives Objekt an
	* setzt Standardwerte
	*/
	CKI::CKI(CCarObject* Object,String Typ)
	{
		levelOfSkill = 1;
		levelOfAggressivity = 1;
		levelOfReaction = 1;

		controlObject.reset(Object);

		net = new CNeuralNetwork(this);
	}


		//--------- ~CKI() ---------
	CKI::~CKI()
	{
		net = NULL;
	}

	/**
	* wird aufgerufen, wenn KI handeln soll
	* die Subfunktionen senden dabei die Signale an das InteractiveObject
	*/
	void CKI::executeKI(float delaySeconds)
	{
		actualizeWaypoint(delaySeconds);
		actualizeCarSpeed();
		net->runNetwork();
		
		
		computeStrategy();

	//	waypoint computeNextWayPoint();

		computeGear();
		computeDirection();
		computeAcceleration();
		
		controlObject->setInputs(steer, acc, brake);
	}
	



	/**
	* nimmt, wenn es sein muss den naechsten Waypoint
	*/
	void CKI::actualizeWaypoint(float delaySeconds = 0.f)
	{	
		if(waypoint == NULL)
		{
			waypoint = mLevel->getFirstWaypoint();
			return;
		}
		else
		{
			
			waypointTime += delaySeconds;
			
			
			if(debug_ki)  std::cout << "\nwaypoint " << waypoint->getVector() << std::endl; 
			
			
			if( waypoint->getNext() != NULL )
			{
				//falls du zu lange brauchst deinen Punkt zu finden fahr den n?chsten Punkt an
				if(waypointTime > 5.f)
				{
					waypoint = waypoint->getNext();
					/*
					std::cout << "waypointTime \n" << waypointTime << "\n";
					std::cout << "delaySeconds \n" << delaySeconds << "\n\n";
					*/
					waypointTime = 0.f;
					return;
				}
				
				//falls du 180 Grad turn machen musst, nimm naechsten Punkt
				if( (waypointTime > 0.5f) && (abs(steer) > 0.95f))
				{
					
					waypoint = waypoint->getNext();
					/*
					std::cout << "abs(steer) \n" << abs(steer) << "\n";
					*/
					waypointTime = 0.f;
					return;
				}
				
				
				float w_dis = ( waypoint->getVector() - waypoint->getNext()->getVector() ).length();
				
				float dis_1 = ( waypoint->getVector() - controlObject->Position() ).length();
				float dis_2 = ( waypoint->getNext()->getVector() - controlObject->Position() ).length();
				float dis_3 = ( waypoint->getNext()->getNext()->getVector() - controlObject->Position() ).length();
				float dis_4 = ( waypoint->getNext()->getNext()->getNext()->getVector() - controlObject->Position() ).length();
				float dis_5 = ( waypoint->getNext()->getNext()->getNext()->getNext()->getVector() - controlObject->Position() ).length();
				
				//falls nah genug am Waypoint, schau auf naechsten
				if(dis_1 < w_dis)
				{
					waypoint = waypoint->getNext();
					
					if(debug_ki)  std::cout << "\n\nwaypoint changed: was too near\n";
					
					actualizeWaypoint();
					waypointTime = 0.f;
					return;
				}
				
				//falls nextWaypoint naeher liegt als der jetzige, nimm naechsten
				if(dis_2 < dis_1)
				{
					waypoint = waypoint->getNext();
					
					if(debug_ki)  std::cout << "\n\nwaypoint changed: next is nearer\n";
					
					actualizeWaypoint();
					waypointTime = 0.f;
					return;
				}
				if(dis_3 < dis_1)
				{
					waypoint = waypoint->getNext()->getNext();
					
					if(debug_ki)  std::cout << "\n\nwaypoint changed: next is nearer\n";
					
					actualizeWaypoint();
					waypointTime = 0.f;
					return;
				}
				
				if(dis_4 < dis_1)
				{
					waypoint = waypoint->getNext()->getNext()->getNext();
					
					if(debug_ki)  std::cout << "\n\nwaypoint changed: next is nearer\n";
					
					actualizeWaypoint();
					waypointTime = 0.f;
					return;
				}
				
				if(dis_5 < dis_1)
				{
					waypoint = waypoint->getNext()->getNext()->getNext()->getNext();
					
					if(debug_ki)  std::cout << "waypoint changed: next is nearer\n";
					
					actualizeWaypoint();
					waypointTime = 0.f;
					return;
				}
				return;
			}
			return;
		}
		return;
	}
	
	
	void CKI::actualizeCarSpeed()
	{
		if(controlObject != NULL)
		{
			carSpeed = controlObject->Speed();
			return;
		}
		// was nicht da ist hat auch keine Speed
		carSpeed = 0.f;
	}
			
	
	
	/**
	* soll mal die KI veraendern, verbessern, anpassen koennen
	*/
	void CKI::computeStrategy()
	{
	}
	
	
	
	void CKI::computeGear()
	{
	/*
		//### muss man debuggen aber m?sste sonst klappen
		if (controlObject->getEngine()->getRpm() < controlObject->getEngine()->getMaxRpm())
		{
			controlObject->shiftDown();
		}
		else
		{
			controlObject->shiftUp();
		};
	*/
	}

	/**
	* soll anhand der aktuellen Position und der Position des NextWaypoints
	* einen Winkel berechnen, der angibt, ob man links oder rechts muss.
	* dieser wird dann in "steerAngle" gespeichert.
	*/
	void CKI::computeDirection()
	{
		
		if (waypoint != NULL)
		{
			
			Ogre::Vector3 o_pos = controlObject->Position();
			if(debug_ki)  std::cout << "o_pos " << o_pos << std::endl;


			Ogre::Vector3 w_dir = waypoint->getVector() - (o_pos);
//			if(debug_ki)  std::cout << "\n  w_dir\n" << w_dir;
		
		
			Ogre::Quaternion o_dir = controlObject->Orientation();
//			if(debug_ki)  std::cout << "\n  o_dir\n" << o_dir;

			Ogre::Vector3* dir = new Vector3(-1.f, 0.f, 0.f);
			Ogre::Vector3 d = *dir;

			Ogre::Vector3 my_richtung = (o_dir * d);
//			if(debug_ki)  std::cout << "\n  my_richtung\n" << my_richtung << std::endl;
			
			my_richtung.normalise();
			w_dir.normalise();
			
			msteerAngle = my_richtung.dotProduct(w_dir);
//			if(debug_ki)  std::cout << "\n  msteerAngle\n" << msteerAngle << endl;
			
			
			steer = net->Steer();
			if(debug_ki)  std::cout << "lenkung " << net->Steer() << std::endl;
		
		
		}


	/*
	//see above at variable Declaration-----------------------------------
		waypoint computeNextWayPoint()
		{

	//check if WayPoint reached
		if (controlObject.getDistance(actualWP)<1)
			then
			{
				actualWP = scene::getNextWayPoint(actualWP,1);
			};

	// Check for Waypoint BEHIND Car (due to jump/bad wayfinding)
		waypoint WP = actualWP;
		int i = 1;
		while ((DotProduct(controlObject.CBaseObject::position - WP.CBaseObject::position , controlObject.CBaseObject::direction) < 0)
			&& WP <> scene::getNextWayPoint(last,0) && WP <> scene::getNextWayPoint(actualWP,4))
	// solange der WP hinter dem Auto liegt, springe einen Wegpunkt weiter, maximal aber 4, bzw zum letzten Wegpunkt.
		do
			{
				WP = getNextWaypoint(actualWP,i);
				i++;
			};
		if (WP == scene::getNextWayPoint(last,0)) then actualWP = WP;
		}
	----------------------- bis hier hin auskommentiert, da Funktionen fehlen
	*/
	}



	/**
	*
	*/
	void CKI::computeAcceleration()
	{
		/*
		if(debug_ki)  std::cout << "\n\n   compAcc  \n\n";
		
		if(debug_ki)  std::cout << "steer  "<< net->isHighSteer()  << std::endl;
		if(debug_ki)  std::cout << "speed  "<< net->isHighSpeed()  << std::endl;
		*/
		
		if (net->isHighSteer() >= 0)
		{
			if (net->isHighSpeed() >= 0 )
			{
				brake = ( net->isHighSteer() * net->isHighSpeed() );
				acc = 0.f;
				
				//controlObject->accellerate_brake(0.f,  net->isHighSteer() * net->isHighSpeed() );
				if(debug_ki)  std::cout << "brake (danger)" << ( net->isHighSteer() * net->isHighSpeed() ) << std::endl;
			}
			else
			{
				//steer [0..1]  speed [-1..0[ 
				brake = 0.f;//( net->isHighSteer() * ( 1.f + net->isHighSpeed() ) );
				acc = ( net->isHighSteer() * ( -1.f * net->isHighSpeed() ) );
				
				//controlObject->accellerate_brake(0.f,  net->isHighSteer() * ( 1.f + net->isHighSpeed() ) );
				if(debug_ki)  std::cout << "accellerate (safe)" << ( net->isHighSteer() * ( -1.f * net->isHighSpeed() ) ) << std::endl;
			}
		}
		else
		{
			if (net->isHighSpeed() >= 0)
			{
				//steer ]-1..0[  speed [0..1[
				acc = (1.f +  (-1.f * (net->isHighSpeed())) );
				brake = 0.f;
				
				//controlObject->accellerate_brake( 1.f + -1.f * (net->isHighSpeed()) , 0.f); //highspeed is negative!
				if(debug_ki)  std::cout << "accellerate (danger) " << (1.f +  -1.f * (net->isHighSpeed()) ) << std::endl;
			}
			else
			{
				// highspeed and highsteer are negative!
				acc = ( net->isHighSteer() * net->isHighSpeed());
				brake = 0.f;
				
				//controlObject->accellerate_brake( (1.f - net->isHighSteer()) * net->isHighSpeed() , 0.f);
				if(debug_ki)  std::cout << "accellerate (turbo) " << ( net->isHighSteer() * net->isHighSpeed() ) << std::endl;
			}
		}
	}
}
