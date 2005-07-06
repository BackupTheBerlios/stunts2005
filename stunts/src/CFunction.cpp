#include "CFunction.h"

namespace stunts
{
	CFunction::CFunction(CKI* KI, int Function, bool func_new = true)
	{
		//to make new Files with weights
		//FUNCTION_NEW = true;
		FUNCTION_NEW = func_new;
	
		this->ki = KI;
		this->function = Function;
	
		
		steer = new float[3];
		
		
		highsteerAnd11 = new float[3];
		highsteerAndnot12 = new float[3];
		highsteerOr21 = new float[3];
	
		
		speedAnd11 = new float[3];
		speedAndnot12 = new float[3];
		speedOr21 = new float[3];
	}
	
	CFunction::~CFunction()
	{
		delete [] steer;
		
		
		delete [] highsteerAnd11;
		delete [] highsteerAndnot12;
		delete [] highsteerOr21;
	
		
		delete [] speedAnd11;
		delete [] speedAndnot12;
		delete [] speedOr21;
		
	}
	
	void CFunction::makeFunction()
	{
		if(function == STEER)
		{
				/*
				steer.push_back(0.f);
				steer.push_back(4.f);
				steer.push_back(0.f);
				*/
			//Inputs
			steerSteerangelN = new CNeurode(0, neurode_LINEAR);
			steerSteerangelN->addInput(0, NULL, ( ki->steerAngle() ));
			steerSteerangelN->runNeurode();
			
			steerReactN = new CNeurode(0);
			steerReactN->addInput(0, NULL, ki->getLevelOfReaction() );
			steerReactN->runNeurode();
			
			steer6timesN = new CNeurode(1, neurode_LINEAR);
			steer6timesN->addInput(0, NULL, &steer[0]);
			steer6timesN->addInput(1, steerReactN, &steer[1]);
			steer6timesN->runNeurode();
	
			//Verrechnung
			steerNeurodeMulti = new CNeurode(1);
			steerNeurodeMulti->addInput(0, NULL, &(steer[2]));
			steerNeurodeMulti->addInput(1, steerSteerangelN, &(steer6timesN->output ) );
			steerNeurodeMulti->runNeurode();
			//printf("multi = %f\n", (steerNeurodeMulti->getOutput()));
		}
	
		if(function == HIGHSTEER)
		{
				/*
				highsteerAnd11.push_back(-1.f);
				highsteerAnd11.push_back(1.f);
				highsteerAnd11.push_back(1.f);
	
				highsteerAndnot12.push_back(0.f);
				highsteerAndnot12.push_back(2.f);
				highsteerAndnot12.push_back(-1.f);
	
				highsteerOr21.push_back(0.f);
				highsteerOr21.push_back(2.f);
				highsteerOr21.push_back(2.f);
				*/
			//Inputs
			highsteerReactN = new CNeurode(0, neurode_LINEAR);
			highsteerReactN->addInput(0, NULL, ki->getLevelOfReaction() );
			highsteerReactN->runNeurode();
	
			highsteerSteerangelN = new CNeurode(0, neurode_LINEAR);
			highsteerSteerangelN->addInput(0, NULL, ( ki->steerAngle() ));
			highsteerSteerangelN->runNeurode();
	
			highsteerAggresN = new CNeurode(0, neurode_LINEAR);
			highsteerAggresN->addInput(0, NULL, ki->getLevelOfAggressivity());
			highsteerAggresN->runNeurode();
	
			//Verrechnung
			highsteerNeurodeAnd11 = new CNeurode(2);
			highsteerNeurodeAnd11->addInput(0, NULL, &(highsteerAnd11[0]));
			highsteerNeurodeAnd11->addInput(1, highsteerReactN, &(highsteerAnd11[1]));
			highsteerNeurodeAnd11->addInput(2, highsteerSteerangelN, &(highsteerAnd11[2]));
			highsteerNeurodeAnd11->runNeurode();
			//printf("and11 = %f\n", (highsteerNeurodeAnd11->getOutput()));
	
			highsteerNeurodeAndnot12 = new CNeurode(2);
			highsteerNeurodeAndnot12->addInput(0, NULL, &(highsteerAndnot12[0]));
			highsteerNeurodeAndnot12->addInput(1, highsteerSteerangelN, &(highsteerAndnot12[1]));
			highsteerNeurodeAndnot12->addInput(2, highsteerAggresN, &(highsteerAndnot12[2]));
			highsteerNeurodeAndnot12->runNeurode();
			//printf("andnot12 = %f\n", (highsteerNeurodeAndnot12->getOutput()));
	
			highsteerNeurodeOr21 = new CNeurode(2, neurode_EXP);
			highsteerNeurodeOr21->addInput(0, NULL, &(highsteerOr21[0]));
			highsteerNeurodeOr21->addInput(1, highsteerNeurodeAnd11, &(highsteerOr21[1]));
			highsteerNeurodeOr21->addInput(2, highsteerNeurodeAndnot12, &(highsteerOr21[2]));
			highsteerNeurodeOr21->runNeurode();
			//printf("or21 = %f\n", (highsteerNeurodeOr21->getOutput()));
		}
	
		if(function == SPEED)
		{
			// reagiert bei Geschwindigkeit ~50
			/*
			speedAnd11.push_back(-50.f);
			speedAnd11.push_back(10.f);
			speedAnd11.push_back(1.f);
	
			speedAndnot12.push_back(-25.f);
			speedAndnot12.push_back(1.f);
			speedAndnot12.push_back(-10.f);
	
			speedOr21.push_back(0.f);
			speedOr21.push_back(1.f);
			speedOr21.push_back(1.f);
			*/
			//Inputs
			speedReactN = new CNeurode(0, neurode_LINEAR);
			speedReactN->addInput(0, NULL, ki->getLevelOfReaction());
			speedReactN->runNeurode();
	
			speedSpeedN = new CNeurode(0, neurode_LINEAR);
			speedSpeedN->addInput(0, NULL, ki->Speed());
			speedSpeedN->runNeurode();
	
			speedAggresN = new CNeurode(0, neurode_LINEAR);
			speedAggresN->addInput(0, NULL, ki->getLevelOfAggressivity());
			speedAggresN->runNeurode();
	
			/*
			* Verrechnung, Aufstellung des Netzes
			*/
			
			//schlaegt aus, wenn speed ~40 bei react=1
			speedNeurodeAnd11 = new CNeurode(2);
			speedNeurodeAnd11->addInput(0, NULL, &(speedAnd11[0]));
			speedNeurodeAnd11->addInput(1, speedReactN, &(speedAnd11[1]));
			speedNeurodeAnd11->addInput(2, speedSpeedN, &(speedAnd11[2]));
			speedNeurodeAnd11->runNeurode();
			//printf("and11 = %f\n", (speedNeurodeAnd11->getOutput()));
	
			//schlaegt aus, wenn speed ~40 bei aggres=1 sonst viel spaeter
			speedNeurodeAndnot12 = new CNeurode(2);
			speedNeurodeAndnot12->addInput(0, NULL, &(speedAndnot12[0]));
			speedNeurodeAndnot12->addInput(1, speedSpeedN, &(speedAndnot12[1]));
			speedNeurodeAndnot12->addInput(2, speedAggresN, &(speedAndnot12[2]));
			speedNeurodeAndnot12->runNeurode();
			//printf("andnot12 = %f\n", (speedNeurodeAndnot12->getOutput()));
	
			speedNeurodeOr21 = new CNeurode(2, neurode_EXP);
			speedNeurodeOr21->addInput(0, NULL, &(speedOr21[0]));
			speedNeurodeOr21->addInput(1, speedNeurodeAnd11, &(speedOr21[1]));
			speedNeurodeOr21->addInput(2, speedNeurodeAndnot12, &(speedOr21[2]));
			speedNeurodeOr21->runNeurode();
			//printf("or21 = %f\n", (speedNeurodeOr21->getOutput()));
	
		}
	}
	
	
	void CFunction::runFunction()
	{
		if(function == STEER)
		{
			steerSteerangelN->runNeurode();
			steerReactN->runNeurode();
			steer6timesN->runNeurode();
			steerNeurodeMulti->runNeurode();
			/*
			printf("steer = %f\n", (steerSteerangelN->output));
			printf("steer = %f\n", (steerReactN->output));
			printf("steer = %f\n", (steer6timesN->output));
			printf("steer = %f\n", (steerNeurodeMulti->output));
			*/
		}
		
		if(function == HIGHSTEER)
		{
			highsteerReactN->runNeurode();
			highsteerSteerangelN->runNeurode();
			highsteerAggresN->runNeurode();
			highsteerNeurodeAnd11->runNeurode();
			highsteerNeurodeAndnot12->runNeurode();
			highsteerNeurodeOr21->runNeurode();
			/*
			printf("highsteer = %f\n", (highsteerReactN->output));
			printf("highsteer = %f\n", (highsteerSteerangelN->output));
			printf("highsteer = %f\n", (highsteerAggresN->output));
			printf("highsteer = %f\n", (highsteerNeurodeAnd11->output));
			printf("highsteer = %f\n", (highsteerNeurodeAndnot12->output));
			printf("highsteer = %f\n", (highsteerNeurodeOr21->output));
			*/
		}
	
		if(function == SPEED)
		{
			speedReactN->runNeurode();
			speedSpeedN->runNeurode();
			speedAggresN->runNeurode();
			speedNeurodeAnd11->runNeurode();
			speedNeurodeAndnot12->runNeurode();
			speedNeurodeOr21->runNeurode();
		/*	
			printf("speed = %f\n", (speedReactN->output));
			printf("speed = %f\n", (speedSpeedN->output));
			printf("speed = %f\n", (speedAggresN->output));
			printf("speed = %f\n", (speedNeurodeAnd11->output));
			printf("speed = %f\n", (speedNeurodeAndnot12->output));
			printf("speed = %f\n", (speedNeurodeOr21->output));
		*/
		}
	}
	
	float* CFunction::getOutput()
	{
		if(function == STEER)
		{
			if(steerNeurodeMulti != NULL)
				return &(steerNeurodeMulti->output);
			return NULL;
		}
		
		if(function == HIGHSTEER)
		{
			if(highsteerNeurodeOr21 != NULL)
				return &(highsteerNeurodeOr21->output);
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
			//	printf("\n###   make highsteer.ai\n");
				fIO->Open(dat, FILE_WRITE_MODE);
				fIO->PushHeader("steer");
				steer[0] = 0.f;
				steer[1] = 4.f;
				steer[2] = 0.f;
				fIO->PushRecord(steer, sizeof(float)*3);
			}
			else
			{
				//printf("\n###   read highsteer.ai\n");
				fIO->Open(dat, FILE_READ_MODE);
	
				fIO->GotoHeader("steer");
				fIO->PopRecord(steer);
			}
			//Test
			/*
			printf("steer[0]=%f  [1]=%f  [2]=%f\n",steer[0],steer[1],steer[2]);
			*/
		}
		
		if(function == HIGHSTEER)
		{
			char* dat = "highsteer.ai";
			FileIO* fIO = new FileIO();
	
			if(FUNCTION_NEW || !fIO->Open(dat, FILE_READ_MODE))
			{
			//	printf("\n###   make highsteer.ai\n");
				fIO->Open(dat, FILE_WRITE_MODE);
				fIO->PushHeader("highsteerAnd11");
				highsteerAnd11[0] = -1.f;
				highsteerAnd11[1] = 1.f;
				highsteerAnd11[2] = 1.f;
				fIO->PushRecord(highsteerAnd11, sizeof(float)*3);
	
				fIO->PushHeader("highsteerAndnot12");
				highsteerAndnot12[0] = 0.f;
				highsteerAndnot12[1] = 2.f;
				highsteerAndnot12[2] = -1.f;
				fIO->PushRecord(highsteerAndnot12, sizeof(float)*3);
	
				fIO->PushHeader("highsteerOr21");
				highsteerOr21[0] = 0.f;
				highsteerOr21[1] = 3.f;
				highsteerOr21[2] = 3.f;
				fIO->PushRecord(highsteerOr21, sizeof(float)*3);
			}
			else
			{
				//printf("\n###   read highsteer.ai\n");
				fIO->Open(dat, FILE_READ_MODE);
	
				fIO->GotoHeader("highsteerAnd11");
				fIO->PopRecord(highsteerAnd11);
	
				fIO->GotoHeader("highsteerAndnot12");
				fIO->PopRecord(highsteerAndnot12);
	
				fIO->GotoHeader("highsteerOr21");
				fIO->PopRecord(highsteerOr21);
			}
			//Test
			/*
			printf("highsteerAnd[0]=%f  [1]=%f  [2]=%f\n",highsteerAnd11[0],highsteerAnd11[1],highsteerAnd11[2]);
			printf("highsteerAndnot[0]=%f  [1]=%f  [2]=%f\n",highsteerAndnot12[0],highsteerAndnot12[1],highsteerAndnot12[2]);
			printf("highsteerOr[0]=%f  [1]=%f  [2]=%f\n",highsteerOr21[0],highsteerOr21[1],highsteerOr21[2]);
			*/
		}
		
		if(function == SPEED)
		{
			char* dat = "speed.ai";
			FileIO* fIO = new FileIO();
	
			if(FUNCTION_NEW || !fIO->Open(dat, FILE_READ_MODE))
			{
				//printf("\n###   make speed.ai\n");
				fIO->Open(dat, FILE_WRITE_MODE);
				fIO->PushHeader("speedAnd11");
				speedAnd11[0] = -50.f;
				speedAnd11[1] = 10.f;
				speedAnd11[2] = 1.f;
				fIO->PushRecord(speedAnd11, sizeof(float)*3);
	
				fIO->PushHeader("speedAndnot12");
				speedAndnot12[0] = -25.f;
				speedAndnot12[1] = 1.f;
				speedAndnot12[2] = -10.f;
				fIO->PushRecord(speedAndnot12, sizeof(float)*3);
	
				fIO->PushHeader("speedOr21");
				speedOr21[0] = 0.f;
				speedOr21[1] = 1.f;
				speedOr21[2] = 1.f;
				fIO->PushRecord(speedOr21, sizeof(float)*3);
			}
			else
			{
				//printf("\n###   read speed.ai\n");
				fIO->Open(dat, FILE_READ_MODE);
	
				fIO->GotoHeader("speedAnd11");
				fIO->PopRecord(speedAnd11);
				//printf("speedAnd[0]=%f  [1]=%f  [2]=%f\n",speedAnd11[0],speedAnd11[1],speedAnd11[2]);
	
				fIO->GotoHeader("speedAndnot12");
				fIO->PopRecord(speedAndnot12);
	
				fIO->GotoHeader("speedOr21");
				fIO->PopRecord(speedOr21);
			}
		}
	}
}
