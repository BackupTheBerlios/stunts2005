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

#ifndef __NR_C_SPOT_LIGHT_H
#define __NR_C_SPOT_LIGHT_H


/*------------------------------------------------------------------------------
 * Forward Declarations
 *----------------------------------------------------------------------------*/
class nrCSpotLight;
	

/*------------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------------*/
#include "nrCLight.h"


/**
 * Spot light class. 
 * @see nrCLight
 * \ingroup 3d
 */
class nrCSpotLight: public nrCLight {
private:
	
	// spotlight properties
	float constAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
	float spotExponent;
	float spotCutoff;
	vec3  direction;
	
  
public:
	
	// Constructor & destructor
	nrCSpotLight();
	~nrCSpotLight();
	

	// get properties
	bool isSpotLight(){return true;}
	bool isDirectionalLight(){return false;}
	bool isPointLight(){return false;}
	bool isPositional(){return true;}
		
	// set spot light properties	
	void setConstantAttenuation(float);
	void setLinearAttenuation(float);
	void setQuadraticAttenuation(float);
	void setSpotExponent(float);
	void setSpotCutoff (float);
	
	void setDirection(const vec3&);
	void setDirection(float, float, float);
	
	
	
	float getConstantAttenuation(){return constAttenuation;}
	float getLinearAttenuation(){return linearAttenuation;}
	float getQuadraticAttenuation(){return quadraticAttenuation;}
	float getSpotExponent(){return spotExponent;}
	float getSpotCutoff(){return spotCutoff;}
		
	const vec3& getDirection(){return direction;}
	
};

#endif
