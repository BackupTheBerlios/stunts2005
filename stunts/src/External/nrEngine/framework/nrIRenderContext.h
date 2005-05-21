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
#ifndef _NR_I_RENDER_CONTEXT_H
#define _NR_I_RENDER_CONTEXT_H


/*--------------------------------------------------------------------------
 * What this file provides?
 *------------------------------------------------------------------------*/
class nrIRenderContext;



/*--------------------------------------------------------------------------
 * What this file needs?
 *------------------------------------------------------------------------*/
class nrIFramework;


/*--------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include "nrIFrameWork.h"
#include <string>


/**
 * nrIRenderContext is an interface representing one rendering context in
 * our engine. Each rendering context is a window. So in the future it can be
 * expanded to support more than one rendering windows.
 * This interface representing NULL-Context.
 * There are also other rendering context that depends on the subsystem you
 * want to use.
 * \ingroup fw
 **/
class nrIRenderContext{
protected:
	int32 screenWidth;
	int32 screenHeight;
	int32 screenDepth;

	bool _inFullscreen;

public:

	// constr & destr
	nrIRenderContext(){
		screenWidth = 0;
		screenHeight = 0;
		screenDepth = 0;
		_inFullscreen = false;
	};
	virtual ~nrIRenderContext(){};


	// Window operations
	virtual nrResult	changeWindowTitle(const std::string& title){return NR_OK;}
	virtual nrResult	changeWindowPosition(int32 x, int32 y){return NR_OK;}
	virtual nrResult	changeResolution(int32 width, int32 height, int32 depth){return NR_OK;}
	virtual nrResult	setFullscreen(bool bFull){return NR_OK;}
	bool				isFullscreen(){return _inFullscreen;}
	
	int32	getWindowWidth(){return screenWidth;}
	int32	getWindowHeight(){return screenHeight;}
	int32	getWindowBpp(){return screenDepth;}
		
	// Rendering operation
	virtual nrResult	swapBuffers(){return NR_OK;}

	// More operations
	virtual nrResult	makeScreenshot(const std::string& fileName){return NR_OK;}

	virtual nrResult	setupPixelFormat(){return NR_OK;}

};

 
#endif
