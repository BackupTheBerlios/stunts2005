#ifndef CFunction_h
#define CFunction_h


namespace stunts
{
	class CFunction;
}


#include "FileIO.h"
#include "CNeurode.h"
#include "CKI.h"


using namespace std;
using std::vector;

const int SPEED = 1000;
const int STEER = 2000;
const int HIGHSTEER = 3000;

namespace stunts
{

	class CFunction {
	  
	 private:
		CKI* ki;
		int function;
		vector<CNeurode*> neurodes;
		//to make new weight files
		bool FUNCTION_NEW;
		
		//STEER
		float* steer;
		
		CNeurode* steerSteerangelN;
		CNeurode* steerReactN;
		
		CNeurode* steer6timesN;
		CNeurode* steerNeurodeMulti;
		
		//HIGHSTEER
		float* highsteerAnd11;
		float* highsteerAndnot12;
		float* highsteerOr21;
		
		CNeurode* highsteerReactN;
		CNeurode* highsteerSteerangelN;
		CNeurode* highsteerAggresN;
		
		CNeurode* highsteerNeurodeAnd11;
		CNeurode* highsteerNeurodeAndnot12;
		CNeurode* highsteerNeurodeOr21;
		
		//SPEED
		float* speedAnd11;
		float* speedAndnot12;
		float* speedOr21;
		
		CNeurode* speedReactN;
		CNeurode* speedSpeedN;
		CNeurode* speedAggresN;
	
		CNeurode* speedNeurodeAnd11;
		CNeurode* speedNeurodeAndnot12;
		CNeurode* speedNeurodeOr21;
		
	 public:
		CFunction(CKI* ki, int function, bool func_new);
		~CFunction();
		
		void makeFunction();
		void runFunction();
		float* getOutput();
		void setInputs();
	};
}
#endif

