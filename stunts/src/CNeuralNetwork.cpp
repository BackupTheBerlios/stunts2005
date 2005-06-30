#include "CNeuralNetwork.h"


namespace stunts
{
	
	CNeuralNetwork::CNeuralNetwork(CKI* KI)
	{
		this->ki = KI;
	}
	
	CNeuralNetwork::~CNeuralNetwork()
	{
	}
		
	void CNeuralNetwork::makeNetwork()
	{
		functionSpeed = new CFunction(ki, SPEED, false);
		functionSpeed->setInputs();
		functionSpeed->makeFunction();
		
		functionSteer = new CFunction(ki, STEER, false);
		functionSteer->setInputs();
		functionSteer->makeFunction();
		
	//	printf("Speed = %f\n", *functionSpeed->getOutput());
	//	printf("Steer = %f\n", *functionSteer->getOutput());
	}
	
	void CNeuralNetwork::runNetwork()
	{	
		functionSteer->runFunction();
		functionSpeed->runFunction();
	}
	
	float CNeuralNetwork::isHighSteer()
	{
		return *functionSteer->getOutput();
	}
	
	float CNeuralNetwork::isHighSpeed()
	{
		return *functionSpeed->getOutput();
	}
}
