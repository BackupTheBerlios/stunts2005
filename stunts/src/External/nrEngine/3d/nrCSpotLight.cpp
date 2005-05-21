/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "../nrEngine.h"
#include "nrCSpotLight.h"


/**
 * Constructor
 */
nrCSpotLight::nrCSpotLight() : nrCLight(){
	constAttenuation = 0;
	
}


/**
 * Destructor
 */
nrCSpotLight::~nrCSpotLight(){

}


/**
 * Set constant spot light intensity attenuation
 */
void nrCSpotLight::setConstantAttenuation(float f){
	if (isAllowedEnum(lightNum)){
		this->constAttenuation = f;
	  	glLightf (GL_LIGHT[lightNum], GL_CONSTANT_ATTENUATION, f);
	}
}


/**
 * Set linear spot light intensity attenuation
 */
void nrCSpotLight::setLinearAttenuation(float f){
	if (isAllowedEnum(lightNum)){
		this->linearAttenuation = f;
	  	glLightf (GL_LIGHT[lightNum], GL_LINEAR_ATTENUATION, f);
	}
}


/**
 * Set quadratic spot light intensity attenuation
 */
void nrCSpotLight::setQuadraticAttenuation(float f){
	if (isAllowedEnum(lightNum)){
		this->quadraticAttenuation = f;
	  	glLightf (GL_LIGHT[lightNum], GL_QUADRATIC_ATTENUATION, f);
	}
}


/**
 * Set spot light exponent
 */
void nrCSpotLight::setSpotExponent(float f){
	if (isAllowedEnum(lightNum)){
		this->spotExponent = f;
	  	glLightf (GL_LIGHT[lightNum], GL_SPOT_EXPONENT, f);
	}
}

/**
 * Set spot light exponent
 */
void nrCSpotLight::setSpotCutoff(float f){
	if (isAllowedEnum(lightNum)){
		this->spotCutoff = f;
	  	glLightf (GL_LIGHT[lightNum], GL_SPOT_CUTOFF, f);
	}
}


/**
 * Set direction
 */
void nrCSpotLight::setDirection(const vec3& dir){
	if (isAllowedEnum(lightNum)){
		direction = dir;
	  	glLightfv (GL_LIGHT[lightNum], GL_SPOT_DIRECTION, dir);
	}
}


/**
 * Set direction
 */
void nrCSpotLight::setDirection(float x, float y, float z){
	if (isAllowedEnum(lightNum)){
		direction = vec3(x,y,z);
	  	glLightfv (GL_LIGHT[lightNum], GL_SPOT_DIRECTION, direction);
	}
}

