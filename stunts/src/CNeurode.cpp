/* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup,
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Peter Kaczmarczyk
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
 * USA.
 */


#include "CNeurode.h"
#include <iostream>

namespace stunts
{
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
			//einf?gen der Inputs
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
}
