#ifndef CKI_h
#define CKI_h

class CKI;

#include "CNeuralNetwork.h"

//---------- Includes ----------
#include "Level.hpp"
#include "InteractiveObject.hpp"
#include "CNeuralNetwork.h"
#include "GearBox.hpp"
#include "Engine.hpp"
#include "CarObject.hpp"
#include "BaseObject.hpp"
//---------- /Includes ---------

using namespace std;
using namespace stunts;

class CKI {

 public:
	// Default constructor and destructor

	CKI();
	CKI(CInteractiveObject Object, String Typ);
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
	//waypoint computeNextWayPoint();

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

