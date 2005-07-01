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
		CFunction* functionHighSteer;
		
		CKI* ki;
		float* outputSteer;
		float* outputSpeed;
		
	 public:
		CNeuralNetwork(CKI* ki);
		~CNeuralNetwork();
		
		void makeNetwork();
		void runNetwork();
		float Steer();
		float isHighSteer();
		float isHighSpeed();
	};
}

#endif

