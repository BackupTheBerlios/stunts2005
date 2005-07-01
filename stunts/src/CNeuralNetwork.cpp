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
		
		functionHighSteer = new CFunction(ki, HIGHSTEER, false);
		functionHighSteer->setInputs();
		functionHighSteer->makeFunction();
		
		functionSteer = new CFunction(ki, STEER, false);
		functionSteer->setInputs();
		functionSteer->makeFunction();
		
	//	printf("Speed = %f\n", *functionSpeed->getOutput());
	//	printf("Steer = %f\n", *functionSteer->getOutput());
	}
	
	void CNeuralNetwork::runNetwork()
	{	
		functionSteer->runFunction();
		functionHighSteer->runFunction();
		functionSpeed->runFunction();
	}
	
	float CNeuralNetwork::Steer()
	{
		return *functionSteer->getOutput();
	}
	
	float CNeuralNetwork::isHighSteer()
	{
		return *functionHighSteer->getOutput();
	}
	
	float CNeuralNetwork::isHighSpeed()
	{
		return *functionSpeed->getOutput();
	}
}
