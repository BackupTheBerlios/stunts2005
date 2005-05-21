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

#ifndef __NR_C_LIGHT_H
#define __NR_C_LIGHT_H


/*------------------------------------------------------------------------------
 * Forward Declarations
 *----------------------------------------------------------------------------*/
class nrCLight;
class nrCPointLight;
class nrCSpotLight;
class nrCDirectionalLight;


/**
 * Light class represents any light source in our 3D-World. This is a simple class
 * storing light position and light properties. 
 * @todo Expand this class to have more flexible light definition system. That means that
 * user can define also per pixel lighting here or something else. It would be also nice
 * if this class is not dependent on OpenGL but defined for using with each rendering system.
 * \ingroup 3d
 */
class nrCLight {
protected:	

	// methods
	bool isAllowedEnum(int);
	GLenum GL_LIGHT[8];
	
	// light properties
	int lightNum;
	vec3 pos;
	bool isOn;
	
	// light color
	float ambientColor[4];
	float diffuseColor[4];
	float specularColor[4];
  
public:
	
	// Constructor & destructor
	nrCLight();
	virtual ~nrCLight();
	
	// set/get id of the light source
	void setLightID(int id);
	int getLightID(){return lightNum;}
	
	// methoden
	virtual void On();
	virtual void Off();
	virtual void Init();
	virtual void Update();
	
	// get properties
	virtual bool isSpotLight() = 0;
	virtual bool isDirectionalLight() = 0;
	virtual bool isPointLight() = 0;
	virtual bool isPositional() = 0;
	
	// set methods
	void setPosition (const vec3& newPos);
	void setPosition (float, float, float);
	
	void setAmbientColor (const vec4&);
	void setAmbientColor (float, float , float , float);
	
	void setDiffuseColor (const vec4&);
	void setDiffuseColor (float, float, float, float);
	
	void setSpecularColor (const vec4&);
	void setSpecularColor (float, float, float, float);
	
	// get light properties
	const vec3& getPosition() 		{return pos;}
	vec4 getAmbientColor()	{return vec4(ambientColor);}
	vec4 getDiffuseColor()	{return vec4(diffuseColor);}
	vec4 getSpecularColor()	{return vec4(specularColor);}
	
};


/*------------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------------*/
#include "nrCPointLight.h"
#include "nrCDirectionLight.h"
#include "nrCSpotLight.h"

#endif
