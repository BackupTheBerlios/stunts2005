#ifndef CKI_h
#define CKI_h

class CKI;

#include "CNeuralNetwork.h"

//---------- Includes ----------
#include "CNeuralNetwork.h"
//#include "Level.hpp"
//#include "InteractiveObject.hpp"
//#include "Gearbox.hpp"
//#include "Engine.hpp"
//#include "CCarObject.hpp"
//#include "BaseObject.hpp"
//---------- /Includes ---------

using namespace std;

class CKI {

 public:
	// Default constructor and destructor
     	
	CKI(CInteractiveObject* Object,String Typ);
	CKI();
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


 private:

	CInteractiveObject* controlObject;
	CNeuralNetwork* net;

	int levelOfSkill;
	int levelOfAggressivity;
	int levelOfReaction;
	int SteerAngle;


	void computeStrategy();
	
	waypoint computeNextWayPoint();
	
	void computeGear();
	void computeDirection();
	void computeAcceleration();
};

#endif

