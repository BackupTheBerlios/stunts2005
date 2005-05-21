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
#ifndef __NR_C_TEXTURE_LOADER_H__
#define __NR_C_TEXTURE_LOADER_H__


/*------------------------------------------------------------------------------
 Forward declarations
 -----------------------------------------------------------------------------*/
class nrCTextureLoader;


/*------------------------------------------------------------------------------
 Includes
 -----------------------------------------------------------------------------*/
#include <string>
#include "../nrEngine.h"


/*------------------------------------------------------------------------------
 Enums
 -----------------------------------------------------------------------------*/

/**
 * This class represents texture loader in our engine. It supports loading
 * from a lot of image formats (JPG, PNG, TGA, DDS). There is also a method for 
 * storing data array into some of this formats.
 * @todo Implement better functions for loading of images.
 * @todo There are some bugs in loading of corrupted PNG-Files. Please correct this.
 * \ingroup 3d
 **/
class nrCTextureLoader : public nrIResourceLoader{
public:

	// Cnstr & Destr
	nrCTextureLoader();
	~nrCTextureLoader();

	// Resource loader interface
	nrIResource* 	createEmptyResource (){return NULL;}
	nrResult 		loadResource (nrIResource *res){return NR_OK;}

	
	//--------------------------------------------------------------------------
	//	Static Functions
	//--------------------------------------------------------------------------
	
	// 2d textures
	static shared_array<byte> load(nrCFileSystem* vfs, const string&name,int &width,int &height);
	static int save(const string&name,const shared_array<byte>& data,int width,int height);

	static shared_array<byte> load_tga(nrCFileSystem* vfs, const string&name,int &width,int &height);
	static shared_array<byte> load_png(nrCFileSystem* vfs, const string&name,int &width,int &height);
	static shared_array<byte> load_jpeg(nrCFileSystem* vfs, const string&name,int &width,int &height);
	static shared_array<byte> load_dds(nrCFileSystem* vfs, const string&name,int &width,int &height);
	
	static int save_tga(const string&name,const shared_array<byte>& data,int width,int height);
	static int save_jpeg(const string&name,const shared_array<byte>& data,int width,int height,int quality);
	
	static shared_array<byte> rgba2luminance(shared_array<byte>& data,int width,int height);
	static shared_array<byte> rgba2luminance_alpha(shared_array<byte>& data,int width,int height);
	static shared_array<byte> rgba2rgb(shared_array<byte>& data,int width,int height);
	static void	flip (shared_array<byte>& data, int width, int height, GLuint format);

	static shared_array<byte> heightmap_to_normalmap (shared_array<byte>& data, int width, int height);
		
	// 3d textures
	static shared_array<byte> load_3d(nrCFileSystem* vfs, const string&name,int &width,int &height,int &depth,int &format);
	static int save_3d(const string&name,const shared_array<byte>& data,int width,int height,int depth,int format);
	
};

#endif
