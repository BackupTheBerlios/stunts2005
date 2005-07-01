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


#ifndef CNeurode_h
#define CNeurode_h

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#if defined(WIN32) || defined(__WATCOMC__) || defined(_WIN64)
    #include <windows.h>
#endif

using namespace std;
using std::vector;

const int neurode_CONST = 100;
const int neurode_LINEAR = 200;
const int neurode_EXP = 300;


namespace stunts
{
	class CNeurode {
	  
	 private:
	 public:
		vector<float*> weights;
		vector<CNeurode*> Inputs;
		int num;
		int function;
		
	// public:
		CNeurode(int number);
		CNeurode(int number, int Function);
		~CNeurode();
		
		float output;
		void addInput(int n, CNeurode* Input, float* weight);
		float getOutput();
		void runNeurode();
		
	};
}
#endif

