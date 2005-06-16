#include "CNeuralNetwork.h"


CNeuralNetwork::CNeuralNetwork(CKI* KI)
{
	if(KI != NULL)
		this->ki = KI;
	else
		this->ki = new CKI;
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
	
	printf("Speed = %f\n", *functionSpeed->getOutput());
	printf("Steer = %f\n", *functionSteer->getOutput());
}

float CNeuralNetwork::isHighSteer()
{
	return *functionSpeed->getOutput();
}

float CNeuralNetwork::isHighSpeed()
{
	return *functionSteer->getOutput();
}