#ifndef CKI_h
#define CKI_h

class CKI;

#include "CNeuralNetwork.h"

//---------- Includes ----------
#include "Level.hpp"
#include "InteractiveObject.hpp"
#include "CNeutralNetwork.hpp"
#include "CGearbox.hpp"
#include "CMotor.hpp"
#include "CCarObject.hpp"
#include "CBaseObject.hpp"
//---------- /Includes ---------

using namespace std;

class CKI {

 public:
	// Default constructor and destructor
     	
	CKI(CInteractiveObject Object,String Typ);
	virtual ~CKI();

	// Execution

	void executeKI (float delaySeconds);
//    setLevelOfSkill		(int levelSkill);
//	setLevelOfAggressivity	(int levelAggro);
//	setLevelOfReaction	(int levelReac);

	int getLevelOfSkill();
	int getLevelOfAggressivity();
	int getLevelOfReaction();


 private:

	CInteractiveObject controlObject;
	waypoint computeNextWayPoint();

	int levelOfSkill;
	int levelOfAggressivity;
	int levelOfReaction;
	int SteerAngle;

	void computeGear();
	void computeStrategy();
	void computeDirection();
	void computeAcceleration();
};

#endif

