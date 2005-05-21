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
 ***************************************************************************
 *                                                                         *
 * Original code was written by  Alexander Zaprjagaev <frustum@frustum.org>*
 *                                                                         *
 **************************************************************************/ 
#ifndef __CTEXTURE_H__
#define __CTEXTURE_H__


/*------------------------------------------------------------------------------
 Forward declarations
 -----------------------------------------------------------------------------*/
class nrCTexture;


/*------------------------------------------------------------------------------
 Includes
 -----------------------------------------------------------------------------*/
#include <string>
#include "../nrEngine.h"
#include "nrCFont.h"

#ifdef _WIN32
#include <windows.h>
#endif

/*------------------------------------------------------------------------------
 Enums
 -----------------------------------------------------------------------------*/

typedef int nrTextureTarget;
typedef int nrTextureFlag;

enum {
	NR_TEXTURE_1D 		= GL_TEXTURE_1D,
	NR_TEXTURE_2D 		= GL_TEXTURE_2D,
	NR_TEXTURE_RECT 	= GL_TEXTURE_RECTANGLE_NV,
	NR_TEXTURE_CUBE 	= GL_TEXTURE_CUBE_MAP,
	NR_TEXTURE_3D 		= GL_TEXTURE_3D
};


enum {
	// format
	NR_LUMINANCE 		= 1 << 0,
	NR_LUMINANCE_ALPHA 	= 1 << 1,
	NR_RGB 				= 1 << 2,
	NR_RGBA 			= 1 << 3,

	// flags
	NR_FLOAT 			= 1 << 4,

	// compression
	NR_DXT_1 			= 1 << 5,
	NR_DXT_3 			= 1 << 6,
	NR_DXT_5 			= 1 << 7,

	// clamp
	NR_CLAMP 			= 1 << 8,
	NR_CLAMP_TO_EDGE 	= 1 << 9,

	// filter
	NR_NEAREST	 				= 1 << 10,
	NR_LINEAR 					= 1 << 11,
	NR_NEAREST_MIPMAP_NEAREST 	= 1 << 12,
	NR_LINEAR_MIPMAP_NEAREST 	= 1 << 13,
	NR_LINEAR_MIPMAP_LINEAR 	= 1 << 14,

	// anisotropy
	NR_ANISOTROPY_1 	= 1 << 15,
	NR_ANISOTROPY_2 	= 1 << 16,
	NR_ANISOTROPY_4 	= 1 << 17,
	NR_ANISOTROPY_8 	= 1 << 18,
	NR_ANISOTROPY_16 	= 1 << 19,

	//Zusatz
	NR_NEAREST_MIPMAP_LINEAR 	= 1 << 20,
	
	// Internal Formats
	NR_INTERNAL_RGB8 			= 1 << 21,
	
	NR_MIP_MAP_SGIS 			= 1 << 22,
	
	// Should we convert our loaded image into normal map
	NR_CONVERT_TO_NORMAL_MAP 	= 1 << 23,
	
	NR_DEPTH_TEXTURE_16 		= 1 << 24,
	NR_DEPTH_TEXTURE_24 		= 1 << 25,
	
	NR_MIRRORED 				= 1 << 26
	
};


//! Texture unit in nrEngine
/**
 * This class represents texture unit in our engine.
 * Texture unit is derived from resource class, so you can use the texture
 * object as a resource. You can also use it without resource manager
 * system.
 *
 * \ingroup 3d
 **/
class nrCTexture : public nrIResource{
public:

	/**
	 * Simple constructor
	 **/
	nrCTexture():width(0),height(0),id(0){}
	
	
	nrCTexture(int width,int height, nrTextureTarget target = NR_TEXTURE_2D,
				nrTextureFlag flag = NR_RGB | NR_LINEAR_MIPMAP_LINEAR);
			
	// Destructor
  	virtual ~nrCTexture();
	

	// Load texture from a file		
	nrResult loadFromFile ( const string& name, nrTextureTarget target = NR_TEXTURE_2D, 
							nrTextureFlag flag = NR_RGB | NR_LINEAR_MIPMAP_LINEAR, 
							bool upSideDown = false, bool holdData = false);

						
	nrResult createTexture (int width,int height, nrTextureTarget target = NR_TEXTURE_2D,
							nrTextureFlag flag = NR_RGB | NR_LINEAR_MIPMAP_LINEAR);
	
	
	// enable/disable/bind texture (single texture mode)
	void enable	(int unit = -1, nrCTexture *old = NULL);
	void disable(int unit = -1);
	void bind	(int unit = -1, nrCTexture *old = NULL);
	
	// copy and render function for texture	
	void copy		(GLuint target = 0, int x=0, int y=0);
	void render		(float x0 = -1.0,float y0 = -1.0,float x1 = 1.0,float y1 = 1.0);
	
	void renderRect (int width, int height, int ofsX=0, int ofsY=0, 
						  float x0=0, float y0=0, float x1=1, float y1=1);

	/**
	 * Get the width of texture unit
	 **/
    int getWidth(){return width;}
    
	/**
	 * Get height of the texture
	 **/
	int getHeight() {return height;}
    
	/**
	 * Get ID of the texture
	 **/
	GLuint getId() {return id;}
    
	/**
	 * Get name of the texture. Name is a file name from which one texture was loaded
	 **/
	const string& getName() {return name;}
	
	/**
	 * If you have load texture with wish to hold the data, you can get here access to the data
	 * of the texture file
	 **/
	shared_array<byte> getData(){return _data;}

	//--------------------------------
	// Resource interface
	//--------------------------------
	const string getResTypeName(){return "nrCTexture";}
	nrResult unloadRes(){return NR_OK;}
	nrResult reloadRes(){return NR_OK;}
	
protected:
	
	// if we want to store image data, we can do it here
	shared_array<byte> _data;

	// Texture properties
	int 			width,height,depth;
  	string			name;
	nrTextureFlag 	flag;

	// Some OpenGL-specific properties
	GLuint target;
	GLuint type;
	GLuint internalformat;
	GLuint format;
	GLuint id;

};

#endif
