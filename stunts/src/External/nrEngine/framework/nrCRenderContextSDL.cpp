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

#include "nrCRenderContextSDL.h"
#include "../nrEngine.h"

/**
 * Constructor
 **/
nrCRenderContextSDL::nrCRenderContextSDL(nrCFrameworkSDL* fw){
	framework = fw;

	_inFullscreen = false;
	
	_videoFlags = 0;
	_mainWindow = NULL;

}


/**
 * Destructor
 **/
nrCRenderContextSDL::~nrCRenderContextSDL(){
	framework = NULL;
}


/**
 * Swap the buffers (front to back buffer and vice versa).
 * @return Always NR_OK
 **/
nrResult nrCRenderContextSDL::swapBuffers(){
	SDL_GL_SwapBuffers();

	return NR_OK;
}


/**
 * This will change the title of the rendering window.
 * @param title string containing new title of the window
 * @return NR_OK if no error occurs, otherwise error code
 **/	
nrResult nrCRenderContextSDL::changeWindowTitle(const std::string& title){

	SDL_WM_SetCaption(title.c_str(), title.c_str());

	return NR_OK;
}


/**
 * This will set your rendering window on new position.
 * @param x,y new position of the window
 * @return either NR_OK or error code
 **/
nrResult nrCRenderContextSDL::changeWindowPosition(int32 x, int32 y){

	return NR_OK;
}


/**
 * Changes the resolution/the size of the rendering window. You use this
 * function to set the depth of the renderer too.
 * @param width,height new size of the window
 * @param depth Depth of the rendering resolution
 * @return either NR_OK or error code
 **/
nrResult nrCRenderContextSDL::changeResolution(int32 width, int32 height, int32 depth){

	nrLog.Log(NR_LOG_ENGINE, "nrCRenderContextSDL: Change window resolution to width=%d, height=%d, bpp=%d", width, height, depth);
	
	// create main SDL-Window	
	_mainWindow = SDL_SetVideoMode(width, height,
								depth, _videoFlags );
	
	if(_mainWindow == NULL){
		nrLog.Log(NR_LOG_ENGINE, "nrCRenderContextSDL: Failed resizing window : %s",
					SDL_GetError());
		return NR_FW_FAILED_TO_RESIZE;
	}else{
		// set new variables	
		screenWidth = width;
		screenHeight = height;
		screenDepth = depth;
	}

	return NR_OK;

#if 0
	// Set OpenGL-Variables
	//SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE,  screenDepth);         // size of depth buffer
    //SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );      // tell SDL that the GL drawing is going to be double buffered
    //SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8);       // we are going to use the stencil buffer
    //SDL_GL_SetAttribute( SDL_GL_ACCUM_RED_SIZE, 0);     // this and the next three lines set the bits allocated per pixel -
    //SDL_GL_SetAttribute( SDL_GL_ACCUM_GREEN_SIZE, 0);   // - for the accumulation buffer to 0
    //SDL_GL_SetAttribute( SDL_GL_ACCUM_BLUE_SIZE, 0);
    //SDL_GL_SetAttribute( SDL_GL_ACCUM_ALPHA_SIZE, 0);
#endif

}


/**
 * Toggle between fullscreen and window mode
 * @return either NR_OK or NR_UNKNOWN_ERROR
 **/
nrResult nrCRenderContextSDL::setFullscreen(bool bFull){

	if (!_inFullscreen && bFull){
		if(SDL_WM_ToggleFullScreen(_mainWindow) == 0){
			nrLog.Log(NR_LOG_ENGINE, "nrCRenderContextSDL: Failed to go into fullscreen mode : %s" ,SDL_GetError());
			return NR_UNKNOWN_ERROR;
		}
	}else if (_inFullscreen && !bFull){
		if(SDL_WM_ToggleFullScreen(_mainWindow) == 0){
			nrLog.Log(NR_LOG_ENGINE, "nrCRenderContextSDL: Failed to go into windowed mode : %s" ,SDL_GetError());
			return NR_UNKNOWN_ERROR;
		}
	}

	return NR_OK;
}



/**
 * Make a screenshot from current framebuffer and store it under the given filename
 * @todo This function also uses OpenGL's feature to read out the content of the
 * backbuffer. Please rewrite the whole screenshot features to be independent
 * of the underlying framework (OpenGL, DirectX, ...)
 **/
nrResult nrCRenderContextSDL::makeScreenshot(const std::string& fileName){

	// get properties and calculate size
	int w = screenWidth;
	int h = screenHeight;
	int size = w*h*4;
		
	// allocate memory for our data
	shared_array<unsigned char> p (new unsigned char[size]);

	// read pixels and store it in our array
	glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, p.get());		
		
	// store screenshot
	nrCTextureLoader::save(fileName, p, w, h);
	
	p.reset();

	return NR_OK;
}


/**
 * Initialize pixelformat.
 * @todo This is the single place in the whole Framework implementation
 * where we directly specify that we want to use OpenGL.
 * So correct this in the future to be independent of the underlying system.
 **/
nrResult nrCRenderContextSDL::setupPixelFormat(){
 
	// setup or video flags for SDL
	_videoFlags    = SDL_OPENGL;     // it's an openGL window
	_videoFlags   |= SDL_HWPALETTE;  // exclusive access to hardware colour palette
	_videoFlags   |= SDL_RESIZABLE; // the window must be resizeable
	
	const SDL_VideoInfo * VideoInfo = SDL_GetVideoInfo();
	
	if(VideoInfo == NULL){
		nrLog.Log(NR_LOG_ENGINE, "nrCRenderContextSDL: Failed getting Video Info : %s",
				SDL_GetError());
		return NR_RC_CANNOT_SETUP_PIXEL_FORMAT;
	}
	
	// if we have hardware support, so choose it for using
	if(VideoInfo -> hw_available){
		_videoFlags |= SDL_HWSURFACE;
	}else{
		_videoFlags |= SDL_SWSURFACE;
	}
	
	// Blitting is fast copying / moving /swapping of contiguous sections of memory
	if(VideoInfo -> blit_hw){
		_videoFlags |= SDL_HWACCEL;
	}
	
	return NR_OK;
}

