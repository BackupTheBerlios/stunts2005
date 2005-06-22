#include "CKI.h"


//this is only a dummy
CKI::CKI()
{

}




//--------- CKI

CKI::CKI(CInteractiveObject Object,String Typ)
{
	levelOfSkill = 1;
	levelOfAggressivity = 1;
	levelOfReaction = 1;




	/*
	-> CNeuralNetwork::makeNetwork();
	so nicht, denn es wurde kein Objekt vom Network instanziiert
	*/

	CNeuralNetwork myNetwork(this);

	myNetwork.makeNetwork();




	controlObject=Object;
}

	//--------- ~CKI() ---------
CKI::~CKI()
{
}


// Execution
void CKI::executeKI(float delaySeconds)
{
	computeGear();
	computeStrategy();
	computeDirection();
	computeAcceleration();
}


//    setLevelOfSkill		(int levelSkill);
//	setLevelOfAggressivity	(int levelAggro);
//	setLevelOfReaction	(int levelReac);

int CKI::getLevelOfSkill()
{	return levelOfSkill;	}

int CKI::getLevelOfAggressivity()
{	return levelOfAggressivity;	}

int CKI::getLevelOfReaction()
{	return levelOfReaction;	}


/* getNextWaypoint hast still to be put in !!
	waypoint actualWP 	= scene::getNextWayPoint(startWP,0);

*/

void CKI::computeGear()
{
/*
	if (CMotor::rpm < CMotor::MaxRPM)
		then
		{
			CGearBox::shiftUp();
			//### falsch rum
		}
		else
		{
			CGearBox::shiftDown();
		};
*/
}

void CKI::computeStrategy()
{
}

void CKI::computeDirection()
{

}

void CKI::computeAcceleration()
{
/*
	if (CNeuralNetwork::highsteer() >= 0)
	{
		if (CNeuralNetwork::highspeed() >= 0 )
		{
  			CCarObject::brake( CNeuralNetwork::highsteer()*CNeuralNetwork::highspeed() );
     	}
		else
		{
  			CCarObject::brake( CNeuralNetwork::highsteer()*( 1-CNeuralNetwork::highspeed() ) );
     	}
	}
	else
	{
		if (CNeuralNetwork::highsteer() >= 0)
		{
			CMotor::accellerate( -(CNeuralNetwork::highspeed()) ); //highspeed is negative!
		}
		else
		{
			CMotor::accellerate( CNeuralNetwork::highsteer()*CNeuralNetwork::highspeed() ); // highspeed and highsteer are negative!
		}
	}

//-----------------------------------------
// ---------computeNextWayPoint------------
//-----------------------------------------

/* see above at variable Declaration-----------------------------------
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


----------------------- bis hier hin auskommentiert, da Funktionen fehlen*/

}
