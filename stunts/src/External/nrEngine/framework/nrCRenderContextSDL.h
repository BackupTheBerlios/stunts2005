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
#ifndef _NR_C_RENDER_CONTEXT_SDL_H
#define _NR_C_RENDER_CONTEXT_SDL_H


/*--------------------------------------------------------------------------
 * What this file provides?
 *------------------------------------------------------------------------*/
class nrCRenderContextSDL;



/*--------------------------------------------------------------------------
 * What this file needs?
 *------------------------------------------------------------------------*/
class nrCFrameworkSDL;


/*--------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include "nrIRenderContext.h"
#include "nrCFrameWorkSDL.h"

#include <string>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>


/**
 * nrCRenderContextSDL is our rendering context with SDL support. So here we
 * use SDL to create window or to change their properties and so on.
 * \ingroup fw
 **/
class nrCRenderContextSDL: public nrIRenderContext{
private:
	nrCFrameworkSDL*	framework;
	
	// neccessary things for graphics
	int32 					_videoFlags;
	SDL_Surface* 			_mainWindow;

public:

	// constr & destr
	nrCRenderContextSDL(nrCFrameworkSDL* fw);
	~nrCRenderContextSDL();


	// Window operations
	nrResult	changeWindowTitle(const std::string& title);
	nrResult	changeWindowPosition(int32 x, int32 y);
	nrResult	changeResolution(int32 width, int32 height, int32 depth);
	nrResult	setFullscreen(bool bFull);
	
	// Rendering operation
	nrResult	swapBuffers();

	// More operations
	nrResult	makeScreenshot(const std::string& fileName);

	nrResult	setupPixelFormat();

};

 
#endif
