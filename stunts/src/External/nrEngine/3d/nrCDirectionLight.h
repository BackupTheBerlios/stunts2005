/***************************************************************************
 *                                                                         *
 *   (c) Art Tevs, MPI Informatik Saarbruecken                             *
 *       mailto: <tevs@mpi-sb.mpg.de>                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __NR_C_DIRECTION_LIGHT_H
#define __NR_C_DIRECTION_LIGHT_H


/*------------------------------------------------------------------------------
 * Forward Declarations
 *----------------------------------------------------------------------------*/
class nrCDirectionLight;
	

/*------------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------------*/
#include "nrCLight.h"


/**
 * Directional light source. 
 * @see nrCLight
 * \ingroup 3d
 */
class nrCDirectionLight: public nrCLight {
private:
	vec3  direction;
	
public:
	
	// Constructor & destructor
	nrCDirectionLight();
	~nrCDirectionLight();
	
	// get properties
	bool isSpotLight(){return false;}
	bool isDirectionalLight(){return true;}
	bool isPointLight(){return false;}
	bool isPositional(){return false;}
		
	// Directional light properties	
	void setDirection(const vec3&);
	void setDirection(float, float, float);
	
	const vec3& getDirection(){return direction;}
	
};

#endif
