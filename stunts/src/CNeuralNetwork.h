#ifndef CNeuralNetwork_h
#define CNeuralNetwork_h

namespace stunts
{
	class CNeuralNetwork;
}

#include "CFunction.h"
#include "CKI.h"


using namespace std;
using std::vector;

namespace stunts
{
	
	class CNeuralNetwork {
	  
	 private:
		float* highSteer;
		float* highSpeed;
		
		CFunction* functionSpeed;
		CFunction* functionSteer;
		
		CKI* ki;
		float* outputSteer;
		float* outputSpeed;
		
	 public:
		CNeuralNetwork(CKI* ki);
		~CNeuralNetwork();
		
		void makeNetwork();
		float isHighSteer();
		float isHighSpeed();
	};
}

#endif

