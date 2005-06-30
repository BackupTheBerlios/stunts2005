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
		}
		else
		{
			if(debug_ki)  std::cout << ".KI.";
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
		computeStrategy();

	//	waypoint computeNextWayPoint();

		computeGear();
		computeDirection();
		computeAcceleration();
	}


	void CKI::setLevelOfSkill (int levelSkill)
	{	this->levelOfSkill = levelSkill;	}

	void CKI::setLevelOfAggressivity (int levelAggro)
	{	this->levelOfAggressivity = levelAggro;	}

	void CKI::setLevelOfReaction (int levelReac)
	{	this->levelOfReaction = levelReac;	}

	int CKI::getLevelOfSkill()
	{	return levelOfSkill;	}

	int CKI::getLevelOfAggressivity()
	{	return levelOfAggressivity;	}

	int CKI::getLevelOfReaction()
	{	return levelOfReaction;	}




	/**
	* soll mal die KI ver?ndern, verbessern, anpassen k?nnen
	*/
	void CKI::computeStrategy()
	{
	}

	/* getNextWaypoint hast still to be put in !!
		waypoint actualWP 	= scene.getNextWayPoint(startWP,0);
	*/
	/**
	* soll anhand von dem WayPoint vector den bestimmen, der am n?chsten ist
	* oder getNextWayPoint liefert von sich den besten WayPoint
	*/
	/*
	waypoint computeNextWayPoint()
	{
	}
	*/


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

		waypoint = mLevel->getFirstWaypoint();


		if ((waypoint != NULL) && (mLevel->mVehicle != NULL))
		{
		//	Ogre::Vector3 o_pos = controlObject->Position();
			Ogre::Vector3 o_pos = mLevel->mVehicle->getPosition();
			if(debug_ki)  std::cout << "\n  o_pos\n" << o_pos;


			Ogre::Vector3 w_dir = waypoint->getVector() - (o_pos);
			if(debug_ki)  std::cout << "\n  w_dir\n" << w_dir;

			Ogre::Quaternion o_dir = controlObject->Orientation();
			if(debug_ki)  std::cout << "\n  o_dir\n" << o_dir;
		//	Ogre::Quaternion o_dir = mLevel->mVehicle->Orientation();

			Ogre::Vector3* dir = new Vector3(1.f, 0.f, 0.f);
			Ogre::Vector3 d = *dir;

			Ogre::Vector3 my_richtung = (o_dir * d);
			if(debug_ki)  std::cout << "\n  my_richtung\n" << my_richtung;

			msteerAngle = my_richtung.dotProduct(w_dir);


			//mLevel->getNextWaypoint(waypoint);
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
		if(debug_ki)  std::cout << "\n\n   compAcc  \n\n";

		net->makeNetwork();

		//### noch pr?fen ob die funktionen alle Ergebnisse liefern
		if (net->isHighSteer() >= 0)
		{
			if (net->isHighSpeed() >= 0 )
			{
//				controlObject->brake( net->isHighSteer() * net->isHighSpeed() );
				if(debug_ki)  std::cout << "brake " << ( net->isHighSteer() * net->isHighSpeed() ) << std::endl;
			}
			else
			{
				//steer [0..1] * (1+ speed [-1..0[ ) => [0..1[
//				controlObject->brake( net->isHighSteer() * ( 1.f + net->isHighSpeed() ) );
				if(debug_ki)  std::cout << "brake " << ( net->isHighSteer() * ( 1.f + net->isHighSpeed() ) ) << std::endl;
			}
		}
		else
		{
			if (net->isHighSpeed() >= 0)
			{
//				controlObject->accellerate( -1.f * (net->isHighSpeed()) ); //highspeed is negative!
				if(debug_ki)  std::cout << "accellerate " << ( -1.f * (net->isHighSpeed()) ) << std::endl;
			}
			else
			{
				// highspeed and highsteer are negative!
//				controlObject->accellerate( net->isHighSteer() * net->isHighSpeed() );
				if(debug_ki)  std::cout << "accellerate " << ( net->isHighSteer() * net->isHighSpeed() ) << std::endl;
			}
		}

	}
}
