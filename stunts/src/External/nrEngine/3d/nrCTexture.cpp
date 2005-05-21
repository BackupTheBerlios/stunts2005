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


/*------------------------------------------------------------------------------
 * nrEngine - Includes
 *----------------------------------------------------------------------------*/ 
#include "../nrEngine.h"
#include "nrCTexture.h"
#include "nrCTextureLoader.h"


/**
 * Create empty rendering texture. 
 * @param width,height - size of new texture
 * @param target - render target of the texture (2D, 3D, CubeMap, ...)
 * @param flag - texture flags (like NR_RGB, NR_CLAMP, ...)
 * @return either NR_OK or error code
 **/
nrResult nrCTexture::createTexture(int width,int height, nrTextureTarget target, nrTextureFlag flag){

	// set internal variables
	this->width = width;
	this->height = height;
	this->depth = 1;
	this->flag = flag;
	this->target = target;

	// generate OpenGL's texture object	
	glGenTextures(1,&id);
	glBindTexture(target,id);
	type = GL_UNSIGNED_BYTE;
	
	// Check if we have supported format
	if		(flag & NR_LUMINANCE) 			format = GL_LUMINANCE;
	else if	(flag & NR_LUMINANCE_ALPHA) 	format = GL_LUMINANCE_ALPHA;
	else if	(flag & NR_RGB) 				format = GL_RGB;
	else if	(flag & NR_RGBA) 				format = GL_RGBA;
	else if	((flag & NR_DEPTH_TEXTURE_16) || (flag & NR_DEPTH_TEXTURE_24)) format = GL_DEPTH_COMPONENT;
	else {
		nrLog.Log(NR_LOG_ENGINE, "nrCTexture::nrCTexture(): unknown texture format");
		return NR_TEX_ERROR;
	}
	
	// set internal format
	internalformat = format;
	if (flag & NR_INTERNAL_RGB8)
		internalformat = GL_RGB8;
	
	// check if we want to have float format
	if(flag & NR_FLOAT) {
		type = GL_FLOAT;
		/*if		(flag & RGB) 	internalformat = GL_FLOAT_RGB_NV;
		else if	(flag & RGBA) 	internalformat = GL_FLOAT_RGBA_NV;
		else {
			nrLog.Log(NR_LOG_ENGINE, 
					"nrCTexture::nrCTexture(): FLOAT flag is accessible only for RGB or RGBA formats");
			return;
		}*/
		
	// check for compression format
	} else if(flag & NR_DXT_1) {
		if		(flag & NR_RGB) internalformat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
		else if	(flag & NR_RGBA) internalformat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		else {
			nrLog.Log(NR_LOG_ENGINE, 
					"nrCTexture::nrCTexture(): can`t set DXT_1 compression for not RGB or RGBA format");
		}
	} else if(flag & NR_DXT_3) {
		if(flag & NR_RGBA) internalformat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		else{
			nrLog.Log(NR_LOG_ENGINE, 
					"nrCTexture::nrCTexture(): can`t set DXT_3 compression for not RGBA format");
		}
	} else if(flag & NR_DXT_5) {
		if(flag & NR_RGBA) internalformat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		else{
			nrLog.Log(NR_LOG_ENGINE, 
					"nrCTexture::nrCTexture(): can`t set DXT_5 compression for not RGBA format");
		}
	}
	
	// check for clamping modes
	if(flag & NR_CLAMP) {
		glTexParameteri(target,GL_TEXTURE_WRAP_S,GL_CLAMP);
		glTexParameteri(target,GL_TEXTURE_WRAP_T,GL_CLAMP);
		glTexParameteri(target,GL_TEXTURE_WRAP_R,GL_CLAMP);
	} else if(flag & NR_CLAMP_TO_EDGE) {
		glTexParameteri(target,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(target,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		glTexParameteri(target,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
	}else if (flag & NR_MIRRORED){
		glTexParameteri(target,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
		glTexParameteri(target,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
		glTexParameteri(target,GL_TEXTURE_WRAP_R,GL_MIRRORED_REPEAT);	
	}

	// check for mipmapping mode	
	if(flag & NR_NEAREST) {
		glTexParameteri(target,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(target,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	} else if(flag & NR_LINEAR) {
		glTexParameteri(target,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(target,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	} else if(flag & NR_NEAREST_MIPMAP_NEAREST){
		glTexParameteri(target,GL_GENERATE_MIPMAP_SGIS,GL_TRUE);
		glTexParameteri(target,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(target,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST);
	} else if(flag & NR_LINEAR_MIPMAP_NEAREST) {
		glTexParameteri(target,GL_GENERATE_MIPMAP_SGIS,GL_TRUE);
		glTexParameteri(target,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(target,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	} else if(flag & NR_LINEAR_MIPMAP_LINEAR) {
		glTexParameteri(target,GL_GENERATE_MIPMAP_SGIS,GL_TRUE);
		glTexParameteri(target,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(target,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	}


	// check for anisotropy mode   
	if(flag & NR_ANISOTROPY_1) glTexParameterf(target,GL_TEXTURE_MAX_ANISOTROPY_EXT,1.0);
	else if(flag & NR_ANISOTROPY_2) glTexParameterf(target,GL_TEXTURE_MAX_ANISOTROPY_EXT,2.0);
	else if(flag & NR_ANISOTROPY_4) glTexParameterf(target,GL_TEXTURE_MAX_ANISOTROPY_EXT,4.0);
	else if(flag & NR_ANISOTROPY_8) glTexParameterf(target,GL_TEXTURE_MAX_ANISOTROPY_EXT,8.0);
	else if(flag & NR_ANISOTROPY_16) glTexParameterf(target,GL_TEXTURE_MAX_ANISOTROPY_EXT,16.0);
		
	// create empty texture
	if(target == NR_TEXTURE_2D) glTexImage2D(target,0,internalformat,width,height,0,format,type,NULL);
	else if (target == NR_TEXTURE_1D) glTexImage1D(target,0,internalformat,width,0,format,type,NULL);
	else if(target == NR_TEXTURE_RECT) glTexImage2D(target,0,internalformat,width,height,0,format,type,NULL);
	else if(target == NR_TEXTURE_CUBE) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,0,internalformat,width,height,0,format,type,NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,0,internalformat,width,height,0,format,type,NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,0,internalformat,width,height,0,format,type,NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,0,internalformat,width,height,0,format,type,NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,0,internalformat,width,height,0,format,type,NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,0,internalformat,width,height,0,format,type,NULL);
	}
	else if(target == NR_TEXTURE_3D)
			 glTexImage3D(target,0,internalformat,width,height,depth,0,format,type,NULL);
			 
	return NR_OK;
}


/**
 * Create empty rendering texture. 
 * @param width,height - size of new texture
 * @param target - render target of the texture (2D, 3D, CubeMap, ...)
 * @param flag - texture flags
 **/
nrCTexture::nrCTexture(int width,int height, nrTextureTarget target, nrTextureFlag flag){
	createTexture(width,height,target,flag);
}


/**
 * Free texture data.
 **/
nrCTexture::~nrCTexture() {
	glDeleteTextures(1,&id);
	_data.reset();
}


/**
 * Load texture from a file 
 * @param name - file name of the texture file
 * @param target - render target of texture (e.g. 2D,1D,3D, CUBE)
 * @param flag - texture flags (e.g. CLAMP, RGB, ...)
 * @param UpSideDown - if true loaded file will be flipped vertically
 * @param holdData - if true, the file data will be holded back
 * @return either NR_OK or error code
 **/
nrResult nrCTexture::loadFromFile(const string& name, nrTextureTarget target, nrTextureFlag flag, 
							bool UpSideDown, bool holdData) {
  	
	// define and fill variables
	this->name = name;
	this->width = 0;
	this->height = 0;
	this->depth = 1;
	this->target = target;
	this->flag = flag;
	bool mipmap = false;
	
	// generate OpenGL texture units
	glGenTextures(1,&id);
	glBindTexture(target,id);
	
	// check format and set variables
	if(flag & NR_LUMINANCE) format = GL_LUMINANCE;
	else if(flag & NR_LUMINANCE_ALPHA) format = GL_LUMINANCE_ALPHA;
	else if(flag & NR_RGB) format = GL_RGB;
	else if(flag & NR_RGBA) format = GL_RGBA;
	else if(flag & NR_DEPTH_TEXTURE_16 || flag & NR_DEPTH_TEXTURE_24) format = GL_DEPTH_COMPONENT;
	else {
		nrLog.Log(NR_LOG_ENGINE, "nrCTexture::nrCTexture(): unknown texture format");
		return NR_TEX_INVALID_FORMAT;
	}
	
	// detect internal format and set variables
	type = GL_UNSIGNED_BYTE;
	internalformat = format;

	if (flag & NR_INTERNAL_RGB8)
		internalformat = GL_RGB8;
	
	if(flag & NR_FLOAT){
		nrLog.Log(NR_LOG_ENGINE, 
			"nrCTexture::nrCTexture(): FLOAT flag is not accessible for '%s' file",name.c_str());
	}
	
	// determine dxt-level (DDS-Texture) and set variables
	if(flag & NR_DXT_1) {
		if(flag & NR_RGB) internalformat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
		else if(flag & NR_RGBA) internalformat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		else{
			nrLog.Log(NR_LOG_ENGINE, 
						"nrCTexture::nrCTexture(): can`t set DXT_1 compression for not RGB or RGBA format");
		}
	} else if(flag & NR_DXT_3) {
		if(flag & NR_RGBA) internalformat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		else{
			nrLog.Log(NR_LOG_ENGINE,
						"nrCTexture::nrCTexture(): can`t set DXT_3 compression for not RGBA format");
		}
	} else if(flag & NR_DXT_5) {
		if(flag & NR_RGBA) internalformat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		else{
			nrLog.Log(NR_LOG_ENGINE,
						"nrCTexture::nrCTexture(): can`t set DXT_5 compression for not RGBA format");
		}
	}
	
	// detect clamp type and activate clamping for texture
	if(flag & NR_CLAMP) {
		glTexParameteri(target,GL_TEXTURE_WRAP_S,GL_CLAMP);
		glTexParameteri(target,GL_TEXTURE_WRAP_T,GL_CLAMP);
		glTexParameteri(target,GL_TEXTURE_WRAP_R,GL_CLAMP);
	} else if(flag & NR_CLAMP_TO_EDGE) {
		glTexParameteri(target,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(target,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		glTexParameteri(target,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
	}
	
	// detect mipmap-function and activate it
	if(flag & NR_NEAREST) {
		glTexParameteri(target,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(target,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	} else if(flag & NR_LINEAR) {
		glTexParameteri(target,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(target,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	} else if(flag & NR_NEAREST_MIPMAP_NEAREST){
		glTexParameteri(target,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(target,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST);
		mipmap = true;
	} else if(flag & NR_LINEAR_MIPMAP_NEAREST) {
		glTexParameteri(target,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(target,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		mipmap = true;
	} else if(flag & NR_LINEAR_MIPMAP_LINEAR) {
		glTexParameteri(target,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(target,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		mipmap = true;
	}else if (flag & NR_NEAREST_MIPMAP_LINEAR){
		glTexParameteri(target,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(target,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_LINEAR);    
  		mipmap = true;
	}
	
	// determine if we have anistropic mode
	if(flag & NR_ANISOTROPY_1) glTexParameterf(target,GL_TEXTURE_MAX_ANISOTROPY_EXT,1.0);
	else if(flag & NR_ANISOTROPY_2) glTexParameterf(target,GL_TEXTURE_MAX_ANISOTROPY_EXT,2.0);
	else if(flag & NR_ANISOTROPY_4) glTexParameterf(target,GL_TEXTURE_MAX_ANISOTROPY_EXT,4.0);
	else if(flag & NR_ANISOTROPY_8) glTexParameterf(target,GL_TEXTURE_MAX_ANISOTROPY_EXT,8.0);
	else if(flag & NR_ANISOTROPY_16) glTexParameterf(target,GL_TEXTURE_MAX_ANISOTROPY_EXT,16.0);
	
	// determine render target and do things for that type
	if(target == NR_TEXTURE_2D || target == NR_TEXTURE_RECT || target == NR_TEXTURE_1D) {
		
		// load file
		shared_array<byte> data = nrCTextureLoader::load(&nrVFS,name,width,height);
		
		if (data.get() == NULL) return NR_FILE_ERROR;
			
		// Convert into normal map if we need this
		if (data && (flag & NR_CONVERT_TO_NORMAL_MAP)){
			data = nrCTextureLoader::heightmap_to_normalmap(data, width,height);
		}
		
		// convert into other formats from RGBA
		if(data && format != GL_RGBA) {
			if(format == GL_LUMINANCE) data = nrCTextureLoader::rgba2luminance(data,width,height);
			else if(format == GL_LUMINANCE_ALPHA) data = nrCTextureLoader::rgba2luminance_alpha(data,width,height);
			else if(format == GL_RGB) data = nrCTextureLoader::rgba2rgb(data,width,height);
		}
		
		// Flip texture horizontaly (if needed)
    	if (UpSideDown) nrCTextureLoader::flip(data, width,height, format);
		
		// Set default OpenGL - Texture modulation mode
    	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// If we have mipmaps, create this otherwise set textures
		if (mipmap){
			if (flag & NR_MIP_MAP_SGIS){
				glTexParameteri(target,GL_GENERATE_MIPMAP_SGIS,GL_TRUE);
			}else{
				if (target == NR_TEXTURE_1D)
					gluBuild1DMipmaps(target,format,width,format,GL_UNSIGNED_BYTE,data.get());	
				else
					gluBuild2DMipmaps(target, 3, width, height, format, GL_UNSIGNED_BYTE, data.get());
			}
		}else
			if (target == NR_TEXTURE_1D)
				glTexImage1D(target,0,internalformat,height,0,format,GL_UNSIGNED_BYTE,data.get());
			else
				glTexImage2D(target,0,internalformat,width,height,0,format,GL_UNSIGNED_BYTE,data.get());
				
		// check if we want to hold readed data
		if (holdData){
			_data = data;
		}
	
		
	// If we load Cube-Map, so load all parts of it
	}else if(target == NR_TEXTURE_CUBE) {
		GLuint targets[6] = {
			GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		};
		char *suffix[6] = { "px", "nx", "ny", "py", "pz", "nz" };
		for(int i = 0; i < 6; i++) {
			
			char buf[1024];
			sprintf(buf,name.c_str(),suffix[i]);
			shared_array<byte> data = nrCTextureLoader::load(&nrVFS,buf,width,height);
			
			if (data.get() == NULL) return NR_FILE_ERROR;
			
			// Convert from RGBA into other formats
			if(data && format != GL_RGBA) {
				if(format == GL_LUMINANCE) data = nrCTextureLoader::rgba2luminance(data,width,height);
				else if(format == GL_LUMINANCE_ALPHA) data = nrCTextureLoader::rgba2luminance_alpha(data,width,height);
				else if(format == GL_RGB) data = nrCTextureLoader::rgba2rgb(data,width,height);
			}
			
			// Flip horizontaly
    		if (UpSideDown)nrCTextureLoader::flip(data, width,height, format);

    		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			if (mipmap){
				if (flag & NR_MIP_MAP_SGIS){
					glTexParameteri(target,GL_GENERATE_MIPMAP_SGIS,GL_TRUE);
				}else{
					gluBuild2DMipmaps(targets[i], 3, width, height, format, GL_UNSIGNED_BYTE, data.get());
				}
			}else{
				glTexImage2D(targets[i],0,internalformat,width,height,0,format,GL_UNSIGNED_BYTE,data.get());
			}
		}
	}
	
	// Load 3D-Texture
	else if(target == NR_TEXTURE_3D) {
		int fmt;
		shared_array<byte> data = nrCTextureLoader::load_3d(&nrVFS, name,width,height,depth,fmt);
		
		if (data.get() == NULL) return NR_FILE_ERROR;
		
		if(fmt == GL_LUMINANCE) format = GL_LUMINANCE;
		else if(fmt == GL_LUMINANCE_ALPHA) format = GL_LUMINANCE_ALPHA;
		else if(fmt == NR_RGB) format = GL_RGB;
		else format = GL_RGBA;
		glTexImage3D(target,0,internalformat,width,height,depth,0,format,GL_UNSIGNED_BYTE,data.get());
	}

	return NR_OK;
}


/**
 * Enable OpenGL texture target of same type
 * @param unit - OpenGl's texture unit to be activated (0 = GL_TEXTURE0, 1 = GL_TEXTURE1, ...)
 *				Default is -1, which means we are in single texture mode. 
 * @param old - if different from NULL, so old target will be disabled
 *              and new activated (Probably it is not neccessary)
 **/
void nrCTexture::enable(int unit, nrCTexture *old) {
	
	// check if we want to enable already enabled texture
	if(old == this) return;

	// Check for multitexturing
	if (unit > -1){	
		glActiveTexture(GL_TEXTURE0 + unit);
	}
	
	// enable if we need this
	if(old != NULL) {
		
		if(old->target != target) {
			glDisable(old->target);
			glEnable(target);
		}
	} else {
		glEnable(target);
	}
	
	// get back to default texture unit
	if (unit > -1){
		glActiveTexture(GL_TEXTURE0);
	}
	
}


/**
 * Disable OpenGL texture unit 
 * @param unit texture unit to be disabled (0=GL_TEXTURE0, 1=GL_TEXTURE1, ...). 
 *		Let this value default (-1) to stay in single texture mode
 **/
void nrCTexture::disable(int unit) {
	
	// check multitex mode
	if (unit > -1){
		glActiveTexture(GL_TEXTURE0 + unit);
		glDisable(target);	
		glActiveTexture(GL_TEXTURE0);
	}else{
		glDisable(target);	
	}
	
}


/**
 * Bind OpenGL texture unit for using
 * @param unit texture unit to bind (0=GL_TEXTURE0, 1=GL_TEXTURE1, ...). 
 *		Let this value default (-1) to stay in single texture mode 
 * @param old - if different from NULL, so old target will be disabled
 *              and new activated (Probably it is not neccessary)
 **/
void nrCTexture::bind(int unit, nrCTexture *old) {
	
	// return back if we want to bind already binded texture
	if(old == this) return;

	if (unit > -1){	
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(target,id);
		glActiveTexture(GL_TEXTURE0);
	}else{
		glBindTexture(target,id);
	}
}



/**
 * Copy frame buffer content into OpenGL texture unit
 * @param target - can be used to copy content to certain target (f.e. NEGATIVE_X
 *				   if current texture is a cube map)
 * @param x,y - top left position of the frame buffer to be copied
 **/
void nrCTexture::copy(GLuint target, int x, int y) {
	if(target == 0) glCopyTexSubImage2D(this->target,0,0,0,x,y,width,height);
	else glCopyTexSubImage2D(target,0,0,0,x,y,width,height);
}


/**
 * Render 2D textured quad
 * @param x0,y0,x1,y1 - left,top,right,bottom position of quad
 **/
void nrCTexture::render(float x0,float y0,float x1,float y1) {
	glBegin(GL_QUADS);
	if(target == NR_TEXTURE_RECT) {
		glTexCoord2i(0,0);
		glVertex2f(x0,y0);
		glTexCoord2i(width,0);
		glVertex2f(x1,y0);
		glTexCoord2i(width,height);
		glVertex2f(x1,y1);
		glTexCoord2i(0,height);
		glVertex2f(x0,y1);
	} else {
		glTexCoord2f(0,0);
		glVertex2f(x0,y0);
		glTexCoord2f(1,0);
		glVertex2f(x1,y0);
		glTexCoord2f(1,1);
		glVertex2f(x1,y1);
		glTexCoord2f(0,1);
		glVertex2f(x0,y1);
	}
	glEnd();
}


/**
 * renderRect - render 2D textured rect. You can also define position of the texels.
 * @param width,height - size of texture (not size of RECT !!!)
 * @param ofsX,ofsY - texel offset
 * @param x0,y0,x1,y1 - position and size
 *
 * @note : Works only on RECT - Texture units
 **/
void nrCTexture::renderRect (int width, int height, int ofsX, int ofsY, 
						  float x0, float y0, float x1, float y1){
							  
	if (target != NR_TEXTURE_RECT) return;
		
	glBegin(GL_QUADS);
	 glTexCoord2f(ofsX,ofsY);
     glVertex2f(x0, y1);
	glTexCoord2f(width+ofsX,ofsY);
     glVertex2f(x1, y1);	
	glTexCoord2f(width+ofsX,height+ofsY);
     glVertex2f(x1, y0);
    glTexCoord2f(ofsX,height+ofsY);
     glVertex2f(x0, y0);
	glEnd();

}
