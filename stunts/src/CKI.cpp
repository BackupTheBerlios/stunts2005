#include "CKI.h"

	//--------- CKI 
/**
* legt eine KI für ein interaktives Objekt an
* setzt Standardwerte
*/
CKI::CKI(CCarObject* Object,String Typ)
{
	levelOfSkill = 1;
	levelOfAggressivity = 1;
	levelOfReaction = 1;
	
	controlObject = Object;
	
	net = new CNeuralNetwork(this);
}

/*
CKI::CKI()
{
	levelOfSkill = 1;
	levelOfAggressivity = 1;
	levelOfReaction = 1;
	
	CNeuralNetwork::makeNeuralNetwork(Typ);
//	controlObject=Object;
}
*/
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
* soll mal die KI verändern, verbessern, anpassen können
*/
void CKI::computeStrategy()
{
}

/* getNextWaypoint hast still to be put in !!
	waypoint actualWP 	= scene.getNextWayPoint(startWP,0);
*/
/**
* soll anhand von dem WayPoint vector den bestimmen, der am nächsten ist
* oder getNextWayPoint liefert von sich den besten WayPoint
*/
/*
waypoint computeNextWayPoint()
{
}
*/


void CKI::computeGear()
{

	//### muss man debuggen aber müsste sonst klappen
	if (controlObject->getEngine()->getRpm() < controlObject->getEngine()->getMaxRpm())
	{
		controlObject->shiftDown();
	}
	else
	{
		controlObject->shiftUp();
	};

}

/**
* soll anhand der aktuellen Position und der Position des NextWaypoints
* einen Winkel berechnen, der angibt, ob man links oder rechts muss.
* dieser wird dann in "SteerAngle" gespeichert.
*/
void CKI::computeDirection()
{
	

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

	//### noch prüfen ob die funktionen alle Ergebnisse liefern
	if (net->isHighSteer() >= 0)
	{
		if (net->isHighSpeed() >= 0 )
		{
  			controlObject->brake( net->isHighSteer() * net->isHighSpeed() );
     		}
		else
		{
  			controlObject->brake( net->isHighSteer() * ( 1.f - net->isHighSpeed() ) );
     		}
	}
	else
	{
		if (net->isHighSpeed() >= 0)
		{
			controlObject->accellerate( -1.f * (net->isHighSpeed()) ); //highspeed is negative!
		}
		else
		{
			// highspeed and highsteer are negative!
			controlObject->accellerate( net->isHighSteer() * net->isHighSpeed() );
		}
	}

}

