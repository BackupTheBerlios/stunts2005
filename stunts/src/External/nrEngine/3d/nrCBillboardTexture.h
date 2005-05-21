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
 **************************************************************************/
#ifndef __NR_CBILLBOARD_TEXTURE_H__
#define __NR_CBILLBOARD_TEXTURE_H__


/**-----------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------------*/
//#include "nrCTexture.h"


/**
 * Same as nrCTexture but provides support to use texture as billboard
 * @see nrCTexture
 * \ingroup 3d
 **/
class nrCBillboardTexture : public nrCTexture {
public:
	
	// Constructors
	nrCBillboardTexture(){nrCTexture();}
	
	nrCBillboardTexture(int width,int height, nrTextureTarget target = NR_TEXTURE_2D,
						nrTextureFlag flag = NR_RGB | NR_LINEAR_MIPMAP_LINEAR){
						nrCTexture(width, height, target, flag);		
						}
			
	// Destructor
  	~nrCBillboardTexture(){
		glDeleteTextures(1, &id);
		_data.reset();		
	};
	

	// billboard part    
    const vec3& getTopLeft(){return billboardTL;}
    const vec3& getTopRight(){return billboardTR;}
    const vec3& getBottomLeft(){return billboardBL;}
    const vec3& getBottomRight(){return billboardBR;}

    // set texture data
    void setTopLeft(const vec3& v){billboardTL = v;}
    void setTopRight(const vec3& v){billboardTR = v;}
    void setBottomLeft(const vec3& v){billboardBL = v;}
    void setBottomRight(const vec3& v){billboardBR = v;}

protected:
		
	// position auf the texture if it is the billboard texture
	// TL-TopLeft Edge of Billboard, ...
	vec3 billboardTL,billboardTR, billboardBR, billboardBL;


};

#endif 
