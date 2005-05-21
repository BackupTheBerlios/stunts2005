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

#ifndef _NR_C_FRAMEWORKSDL_H_
#define _NR_C_FRAMEWORKSDL_H_


/*--------------------------------------------------------------------------
 * What this file provides?
 *------------------------------------------------------------------------*/
class nrCFrameworkSDL;


/*--------------------------------------------------------------------------
 * What this file need?
 *------------------------------------------------------------------------*/
class nrIFramework;


/*--------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include "sdl/sdl.h"
#include "nrIFrameWork.h"


/**
 * Framework derived from nrIFramework - Interface. This class implements
 * framework by using <a href="http://www.libsdl.org">libSDL</a>
 * \ingroup fw
 **/
class nrCFrameworkSDL : public nrIFramework{		
public: 
	
	// Constructor & Destructor
	nrCFrameworkSDL();
	virtual ~nrCFrameworkSDL();


	// Framework's functions
	nrResult		init();
	nrResult		update();
	nrResult		stop();

	// Input cursor functions
	void	showCursor(bool bShow);
	void	getCursorPosition(int32* x, int32* y);
	void	setCursorPosition(int32 x, int32 y);


	// Render context operations
	nrResult	createRenderContext(int32* returnID, int32 width, int32 height, int32 bpp);

	
	/**
	 * Just return the time in milliseconds.
	 **/
	uint32	getTicks(){return SDL_GetTicks();}

};

#endif
