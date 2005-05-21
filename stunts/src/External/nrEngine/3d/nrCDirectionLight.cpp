/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "../nrEngine.h"
#include "nrCLight.h"


/**
 * Constructor
 */
nrCDirectionLight::nrCDirectionLight() : nrCLight(){
}


/**
 * Destructor
 */
nrCDirectionLight::~nrCDirectionLight(){

}

/**
 * Set direction
 */
void nrCDirectionLight::setDirection(const vec3& dir){
	if (isAllowedEnum(lightNum)){
		direction = dir;
		glLightfv (GL_LIGHT[lightNum], GL_POSITION, vec4(direction, 0));
	}
}


/**
 * Set direction
 */
void nrCDirectionLight::setDirection(float x, float y, float z){
	if (isAllowedEnum(lightNum)){
		direction = vec3(x,y,z);
		glLightfv (GL_LIGHT[lightNum], GL_POSITION, vec4(direction, 0));
	}
}

