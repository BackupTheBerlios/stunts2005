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
		/*
		vector<float> steerAnd11;
		vector<float> steerAndnot12;
		vector<float> steerOr21;*/
		float* steerAnd11;
		float* steerAndnot12;
		float* steerOr21;
		CNeurode* steerNeurodeAnd11;
		CNeurode* steerNeurodeAndnot12;
		CNeurode* steerNeurodeOr21;
		
		//SPEED
		/*
		vector<float> speedAnd11;
		vector<float> speedAndnot12;
		vector<float> speedOr21;*/
		float* speedAnd11;
		float* speedAndnot12;
		float* speedOr21;
		CNeurode* speedNeurodeAnd11;
		CNeurode* speedNeurodeAndnot12;
		CNeurode* speedNeurodeOr21;
		
	 public:
		CFunction(CKI* ki, int function, bool func_new);
		~CFunction();
		
		void makeFunction();
		float* getOutput();
		void setInputs();
	};
}
#endif

