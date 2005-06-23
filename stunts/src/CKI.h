#ifndef CKI_h
#define CKI_h

class CKI;

#include "CNeuralNetwork.h"

//---------- Includes ----------
#include "CNeuralNetwork.h"
#include "Level.hpp"
//#include "InteractiveObject.hpp"
#include "GearBox.hpp"
#include "Engine.hpp"
#include "CarObject.hpp"
#include "BaseObject.hpp"
//---------- /Includes ---------

//using namespace std;
using namespace stunts;

class CKI {

 public:
	// Default constructor and destructor
     	
	CKI(CCarObject* Object,String Typ);
//	CKI();
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

	CCarObject* controlObject;
	CNeuralNetwork* net;

	int levelOfSkill;
	int levelOfAggressivity;
	int levelOfReaction;
	int SteerAngle;


	void computeStrategy();
	
	shared_ptr<CWawaypoint> waypoint;
	
	void computeGear();
	void computeDirection();
	void computeAcceleration();
};

#endif

