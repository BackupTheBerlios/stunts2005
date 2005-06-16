#include "CNeurode.h"
#include <iostream>

	CNeurode::CNeurode(int n)
	{
		this->function = neurode_EXP;

		weights.clear();
		float zero = 0.f;
		weights = vector<float*>((size_t)n+1, &zero);
		Inputs = vector<CNeurode*>((size_t)n, NULL);
	
		num = n;
		output = 0;
	}

	CNeurode::CNeurode(int n, int Function)
	{
		this->function = Function;

		weights.clear();
		float zero = 0.f;
		weights = vector<float*>((size_t)n+1, &zero);
		Inputs = vector<CNeurode*>((size_t)n, NULL);
	
		num = n;
		output = 0;
	}

	CNeurode::~CNeurode()
	{
		weights.clear();
		Inputs.clear();
	}

	void CNeurode::addInput(int n, CNeurode* Input, float* weight){
		if(n == 0){
			//printf("\tbias added\t");
 			weights.at(weights.size()-1) = weight;
			//printf("addInput %f an Stelle %i\n",*weight ,(weights.size()-1) );
		}
		else{
			//einfügen der Inputs
 			weights.at(n-1) = weight;
		
			Inputs.at(n-1) = Input;
			//printf("addInput %f an Stelle %i\n",*weight ,n-1 );
		}
	}

	float CNeurode::getOutput(){
		return output;
	}

	void CNeurode::runNeurode()
	{
		float inp = 0;
		int end = weights.size()-1;
		float dump = *weights.at(end);
		//printf("\ndump    %f\n", dump);
	
		for(int i=0; i<end; i++)
		{
			inp = Inputs.at(i)->getOutput();
			//printf("inp     %f\n", inp);
			//printf("weights[%i] %f\n",i, *weights.at(i));
 			dump = dump + *weights.at(i) * inp;
			//printf("dump + weights*inp =  %f\n\n", dump);
  		}
  		float f_x;
  		switch(function)
   		{
   			case neurode_CONST :
     			f_x = (dump < 0) ? -1 : 1; break;
   			case neurode_LINEAR :
     			f_x = dump; break;
			case neurode_EXP :
  				f_x = (dump < 0) ? (-(1-exp(dump))) : (1-exp(-dump));
     			break;
		}
  	
   		output = f_x;
	}

