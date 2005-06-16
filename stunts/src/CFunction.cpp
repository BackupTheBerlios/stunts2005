#include "CFunction.h"

CFunction::CFunction(CKI* KI, int Function, bool func_new = false)
{
	//to make new Files with weights
	FUNCTION_NEW = true;
	//FUNCTION_NEW = func_new;
	
	this->ki = KI;
	this->function = Function;
	
    steerAnd11 = new float[3];
    steerAndnot12 = new float[3];
    steerOr21 = new float[3];
    
    speedAnd11 = new float[3];
    speedAndnot12 = new float[3];
    speedOr21 = new float[3];
	/*
    steerAnd11.clear();
    steerAndnot12.clear();
    steerOr21.clear();
    
    speedAnd11.clear();
    speedAndnot12.clear();
    speedOr21.clear();
    */
}

CFunction::~CFunction()
{
    delete [] steerAnd11;
    delete [] steerAndnot12;
    delete [] steerOr21;
    
    delete [] speedAnd11;
    delete [] speedAndnot12;
    delete [] speedOr21;
	/*
    steerAnd11.clear();
    steerAndnot12.clear();
    steerOr21.clear();
    
    speedAnd11.clear();
    speedAndnot12.clear();
    speedOr21.clear();*/
}

void CFunction::makeFunction()
{
	if(function == STEER)
	{
			/*
			steerAnd11.push_back(-1.f);
			steerAnd11.push_back(1.f);
			steerAnd11.push_back(1.f);
		
			steerAndnot12.push_back(0.f);
			steerAndnot12.push_back(2.f);
			steerAndnot12.push_back(-1.f);
			
			steerOr21.push_back(0.f);
			steerOr21.push_back(1.f);
			steerOr21.push_back(1.f);
			*/
		//Inputs
		CNeurode* reactN = new CNeurode(0, neurode_LINEAR);
		float react = 1.f;
		reactN->addInput(0, NULL, &react);
		reactN->runNeurode();
		
		CNeurode* steerangelN = new CNeurode(0, neurode_LINEAR);
		float steerangel = 1.f;
		steerangelN->addInput(0, NULL, &steerangel);
		steerangelN->runNeurode();
		
		CNeurode* aggresN = new CNeurode(0, neurode_LINEAR);
		float aggres = 1.f;
		aggresN->addInput(0, NULL, &aggres);
		aggresN->runNeurode();
		
		//Verrechnung
		steerNeurodeAnd11 = new CNeurode(2);
		steerNeurodeAnd11->addInput(0, NULL, &(steerAnd11[0]));
		steerNeurodeAnd11->addInput(1, reactN, &(steerAnd11[1]));
		steerNeurodeAnd11->addInput(2, steerangelN, &(steerAnd11[2]));
		steerNeurodeAnd11->runNeurode();
    	printf("and11 = %f\n", (steerNeurodeAnd11->getOutput()));
		
		steerNeurodeAndnot12 = new CNeurode(2);
		steerNeurodeAndnot12->addInput(0, NULL, &(steerAndnot12[0]));
		steerNeurodeAndnot12->addInput(1, steerangelN, &(steerAndnot12[1]));
		steerNeurodeAndnot12->addInput(2, aggresN, &(steerAndnot12[2]));
		steerNeurodeAndnot12->runNeurode();
    	printf("andnot12 = %f\n", (steerNeurodeAndnot12->getOutput()));
		
		steerNeurodeOr21 = new CNeurode(2, neurode_EXP);
		steerNeurodeOr21->addInput(0, NULL, &(steerOr21[0]));
		steerNeurodeOr21->addInput(1, steerNeurodeAnd11, &(steerOr21[1]));
		steerNeurodeOr21->addInput(2, steerNeurodeAndnot12, &(steerOr21[2]));
		steerNeurodeOr21->runNeurode();
    	printf("or21 = %f\n", (steerNeurodeOr21->getOutput()));
  	}
  	
	if(function == SPEED)
	{
		// reagiert bei Geschwindigkeit ~50 
		/*
		speedAnd11.push_back(-50.f);
		speedAnd11.push_back(50.f);
		speedAnd11.push_back(1.f);
		
		speedAndnot12.push_back(-25.f);
		speedAndnot12.push_back(1.f);
		speedAndnot12.push_back(-25.f);
		
		speedOr21.push_back(0.f);
		speedOr21.push_back(1.f);
		speedOr21.push_back(1.f);
		*/
		//Inputs
		CNeurode* reactN = new CNeurode(0, neurode_LINEAR);
		float react = 1.f;
		reactN->addInput(0, NULL, &react);
		reactN->runNeurode();
		
		CNeurode* speedN = new CNeurode(0, neurode_LINEAR);
		float speed = 50.f;
		speedN->addInput(0, NULL, &speed);
		speedN->runNeurode();
		
		CNeurode* aggresN = new CNeurode(0, neurode_LINEAR);
		float aggres = 1.f;
		aggresN->addInput(0, NULL, &aggres);
		aggresN->runNeurode();
		
		//Verrechnung, Aufstellung des Netzes
		speedNeurodeAnd11 = new CNeurode(2);
		speedNeurodeAnd11->addInput(0, NULL, &(speedAnd11[0]));
		speedNeurodeAnd11->addInput(1, reactN, &(speedAnd11[1]));
		speedNeurodeAnd11->addInput(2, speedN, &(speedAnd11[2]));
		speedNeurodeAnd11->runNeurode();
    	printf("and11 = %f\n", (speedNeurodeAnd11->getOutput()));
		
		speedNeurodeAndnot12 = new CNeurode(2);
		speedNeurodeAndnot12->addInput(0, NULL, &(speedAndnot12[0]));
		speedNeurodeAndnot12->addInput(1, speedN, &(speedAndnot12[1]));
		speedNeurodeAndnot12->addInput(2, aggresN, &(speedAndnot12[2]));
		speedNeurodeAndnot12->runNeurode();
    	printf("andnot12 = %f\n", (speedNeurodeAndnot12->getOutput()));
		
		speedNeurodeOr21 = new CNeurode(2, neurode_EXP);
		speedNeurodeOr21->addInput(0, NULL, &(speedOr21[0]));
		speedNeurodeOr21->addInput(1, speedNeurodeAnd11, &(speedOr21[1]));
		speedNeurodeOr21->addInput(2, speedNeurodeAndnot12, &(speedOr21[2]));
		speedNeurodeOr21->runNeurode();
    	printf("or21 = %f\n", (speedNeurodeOr21->getOutput()));
    	
  	}
}

float* CFunction::getOutput()
{
	if(function == STEER)
	{
		if(steerNeurodeOr21 != NULL)
			return &(steerNeurodeOr21->output);
		return NULL;
	}
	if(function == SPEED)
	{
		if(speedNeurodeOr21 != NULL)
			return &(speedNeurodeOr21->output);
		return NULL;
	}
}

/**
* liest aus der betreffenden Datei die Gewichte raus,
* falls diese nicht existiert wird eine Datei mit Startwerten erzeugt
*/
void CFunction::setInputs()
{
	if(function == STEER)
	{
		char* dat = "steer.ai";
		FileIO* fIO = new FileIO();
		
		if(FUNCTION_NEW || !fIO->Open(dat, FILE_READ_MODE))
		{
			printf("\n###   make steer.ai\n");
			fIO->Open(dat, FILE_WRITE_MODE);
			fIO->PushHeader("steerAnd11");
			steerAnd11[0] = -1.f;
			steerAnd11[1] = 1.f;
			steerAnd11[2] = 1.f;
			fIO->PushRecord(steerAnd11, sizeof(float)*3);
			
			fIO->PushHeader("steerAndnot12");
			steerAndnot12[0] = 0.f;
			steerAndnot12[1] = 2.f;
			steerAndnot12[2] = -1.f;
			fIO->PushRecord(steerAndnot12, sizeof(float)*3);
			
			fIO->PushHeader("steerOr21");
			steerOr21[0] = 0.f;
			steerOr21[1] = 1.f;
			steerOr21[2] = 1.f;
			fIO->PushRecord(steerOr21, sizeof(float)*3);
		}
		else
		{
			printf("\n###   read steer.ai\n");
			fIO->Open(dat, FILE_READ_MODE);
			
			fIO->GotoHeader("steerAnd11");
   			fIO->PopRecord(steerAnd11);
   			
			fIO->GotoHeader("steerAndnot12");
   			fIO->PopRecord(steerAndnot12);
   			
			fIO->GotoHeader("steerOr21");
   			fIO->PopRecord(steerOr21);
		}
		//Test
		/*
		printf("steerAnd[0]=%f  [1]=%f  [2]=%f\n",steerAnd11[0],steerAnd11[1],steerAnd11[2]);
		printf("steerAndnot[0]=%f  [1]=%f  [2]=%f\n",steerAndnot12[0],steerAndnot12[1],steerAndnot12[2]);
		printf("steerOr[0]=%f  [1]=%f  [2]=%f\n",steerOr21[0],steerOr21[1],steerOr21[2]);
		*/
	}
	if(function == SPEED)
	{
		char* dat = "speed.ai";
		FileIO* fIO = new FileIO();
		
		if(FUNCTION_NEW || !fIO->Open(dat, FILE_READ_MODE))
		{
			printf("\n###   make speed.ai\n");
			fIO->Open(dat, FILE_WRITE_MODE);
			fIO->PushHeader("speedAnd11");
			speedAnd11[0] = -50.f;
			speedAnd11[1] = 50.f;
			speedAnd11[2] = 1.f;
			fIO->PushRecord(speedAnd11, sizeof(float)*3);
			
			fIO->PushHeader("speedAndnot12");
			speedAndnot12[0] = -25.f;
			speedAndnot12[1] = 1.f;
			speedAndnot12[2] = -25.f;
			fIO->PushRecord(speedAndnot12, sizeof(float)*3);
			
			fIO->PushHeader("speedOr21");
			speedOr21[0] = 0.f;
			speedOr21[1] = 1.f;
			speedOr21[2] = 1.f;
			fIO->PushRecord(speedOr21, sizeof(float)*3);
		}
		else
		{
			printf("\n###   read speed.ai\n");
			fIO->Open(dat, FILE_READ_MODE);
			
			fIO->GotoHeader("speedAnd11");
   			fIO->PopRecord(speedAnd11);
			printf("speedAnd[0]=%f  [1]=%f  [2]=%f\n",speedAnd11[0],speedAnd11[1],speedAnd11[2]);
   			
			fIO->GotoHeader("speedAndnot12");
   			fIO->PopRecord(speedAndnot12);
   			
			fIO->GotoHeader("speedOr21");
   			fIO->PopRecord(speedOr21);
		}
	}
}

