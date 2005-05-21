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
nrCLight::nrCLight(){
  memset(ambientColor, 0, 4 * sizeof(float));
  memset(diffuseColor, 0, 4 * sizeof(float));
  memset(specularColor, 0, 4 * sizeof(float));

  GL_LIGHT[0] = GL_LIGHT0;
  GL_LIGHT[1] = GL_LIGHT1;
  GL_LIGHT[2] = GL_LIGHT2;
  GL_LIGHT[3] = GL_LIGHT3;
  GL_LIGHT[4] = GL_LIGHT4;
  GL_LIGHT[5] = GL_LIGHT5;
  GL_LIGHT[6] = GL_LIGHT6;
  GL_LIGHT[7] = GL_LIGHT7;
  
  isOn = false;
}


/**
 * Destructor
 */
nrCLight::~nrCLight(){

}


/**
 * Set light id. Two light sources with same id have same behaivor because
 * of using hardware light sources. 
 * @note Only if the id is between 0 and 7 the light will be used otherwise
 * there will be no effect by rendering 3D-World with this light on. 
 * @todo Implement other architecture to allow much more light sources than
 * OpenGL allows (There is only 8 Lights).
 **/
void nrCLight::setLightID(int id){
	lightNum = id;
}


/**
 * Set ambient color of the light source
 * @param color - Light color to be set as ambient color
 **/
void nrCLight::setAmbientColor (const vec4& color){
  if (isAllowedEnum(lightNum)){
    glLightfv (GL_LIGHT[lightNum], GL_AMBIENT, color);
    ambientColor[0] = color[0];
    ambientColor[1] = color[1];
    ambientColor[2] = color[2];
    ambientColor[3] = color[3];
  }
}


/**
 * Set ambient color of the light source
 * @param r - red component
 * @param g - green component
 * @param b - blue component
 * @param a - alpha component of the light color
 **/
void nrCLight::setAmbientColor (float r, float g, float b, float a){
  float color[4] = {r, g, b, a};
  setAmbientColor (color);
}


/**
 * Set diffuse color of the light
 * @param color - Light color to be set as diffuse component
 **/
void nrCLight::setDiffuseColor (const vec4& color){
  if (isAllowedEnum(lightNum)){
    glLightfv (GL_LIGHT[lightNum], GL_DIFFUSE, color);
    diffuseColor[0] = color[0];
    diffuseColor[1] = color[1];
    diffuseColor[2] = color[2];
    diffuseColor[3] = color[3];
  }
}


/**
 * Set diffuse color of the light source
 * @param r - red component
 * @param g - green component
 * @param b - blue component
 * @param a - alpha component of the light color
 **/
void nrCLight::setDiffuseColor (float r, float g, float b, float a){
  float color[4] = {r, g, b, a};
  setDiffuseColor (color);
}


/**
 * Set specular color of the light source
 * @param color - color
 **/
void nrCLight::setSpecularColor (const vec4& color){
  if (isAllowedEnum(lightNum)){
    glLightfv (GL_LIGHT[lightNum], GL_SPECULAR, color);
    specularColor[0] = color[0];
    specularColor[1] = color[1];
    specularColor[2] = color[2];
    specularColor[3] = color[3];
  }
}

/**
 * set specular color of the light source
 * @param r - red component
 * @param g - green component
 * @param b - blue component
 * @param a - alpha component of the light color
 **/
void nrCLight::setSpecularColor (float r, float g, float b, float a){
  float color[4] = {r, g, b, a};
  setSpecularColor (color);
}


/**
 * Set position of the light source
 * @param newPos - new position of the light source
 */
void nrCLight::setPosition (const vec3& newPos){
  
	// position
	if (isPositional()){
		float pos[4] = {newPos.x, newPos.y, newPos.z, (GLfloat)isPositional()};
		glLightfv (GL_LIGHT[lightNum], GL_POSITION, pos);
	}
	this->pos = newPos;
  
}


/**
 * Set position of the light source
 * @param x,y,z - new position of the light source
 */
void nrCLight::setPosition (float x, float y, float z){
	
	// position
	if (isPositional()){
		float pos[4] = {x, y, z, (GLfloat)isPositional()};
		glLightfv (GL_LIGHT[lightNum], GL_POSITION, pos);
	}
	this->pos = vec3(x,y,z);
	
}
	

  
/**
 * Check whenever we have valid light. There is now support for only 8 Hardware
 * lights.
 **/
bool nrCLight::isAllowedEnum(int l){

  if (l >= 0 && l < 8)
      return true;

  return false;            
    
}



/**
 * Turn this light source on
 */
void nrCLight::On(){
	if (isAllowedEnum(lightNum) && !isOn){
		glEnable (GL_LIGHT[lightNum]);
		isOn = true;
	}
}

/**
 * Turn this light source off
 */
void nrCLight::Off(){
	if (isAllowedEnum(lightNum) && isOn)
		glDisable (GL_LIGHT[lightNum]);
	
	isOn = false;
}


/**
 * Update the light source
 */
void nrCLight::Update(){
	// check if the light is on and OpenGL's state is off,
	// so turn the light source on
	if (isOn && isAllowedEnum(lightNum)){
		bool isEnabled = glIsEnabled(GL_LIGHT[lightNum]);
		
		if (!isEnabled){
			glEnable(GL_LIGHT[lightNum]);
		}
	}
}


/**
 * Initialize light source.
 **/
void nrCLight::Init(){

  // prüfe zuerst, ob Licht erlaubt ist
  if (!isAllowedEnum (GL_LIGHT[lightNum])) return;
  
  // aktiviere die farben
  setAmbientColor (ambientColor);
  setDiffuseColor (diffuseColor);
  setSpecularColor (specularColor);

  // setze die Position des Lichtes
  setPosition (pos);
  
}
