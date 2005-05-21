/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


/*--------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include "nrCFrameWorkSDL.h"
#include "../nrEngine.h"

#define DEFAULT_WINDOW_SIZE 320
#define DEFAULT_BPP 16

/**
 * Set values to default
 **/
nrCFrameworkSDL::nrCFrameworkSDL():nrIFramework(){
}


/**
 * Release reserved memory .
 **/
nrCFrameworkSDL::~nrCFrameworkSDL(){
	
	// kill SDL-subsystem
	SDL_Quit();


}


/**
 * Initialize the SDL-FrameWork. Initialize SDL-Video and set some properties.
 * @return either NR_OK or:
 *		- NR_FW_CANNOT_INITIALIZE
 *		- NR_FW_ALREADY_INIT if already initialized
 **/
nrResult nrCFrameworkSDL::init(){

	if (isInitialized()){
		nrLog.Log(NR_LOG_ENGINE, "nrCFrameworkSDL: WARNING: Framework already initialized");
		return NR_FW_ALREADY_INIT;
	}
	
	// Log it
	nrLog.Log(NR_LOG_ENGINE, "nrCFrameworkSDL: Initialize SDL framework");
	
	// initialize SDL-Video Interface
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		nrLog.Log(NR_LOG_ENGINE, "nrCFrameworkSDL: Failed initializing SDL Video : %s"
				,SDL_GetError());
		return NR_FW_CANNOT_INITIALIZE;
	}

	// activate SDL-events
	SDL_EventState (SDL_KEYDOWN, SDL_ENABLE);
	SDL_EventState (SDL_KEYUP, SDL_ENABLE);
	SDL_EventState (SDL_QUIT, SDL_ENABLE);
	SDL_EventState (SDL_MOUSEMOTION, SDL_ENABLE);
	SDL_EnableKeyRepeat (1,0);
	SDL_GetKeyState(&_keyCount);

	// create and full the keyboard state arrays
	_keyState.reset(new byte[_keyCount]);
	_oldKeyState.reset(new byte[_keyCount]);

	byte* ks = SDL_GetKeyState(NULL);
	
	memcpy(_oldKeyState.get(), ks, sizeof(byte) * _keyCount);
	memcpy(_keyState.get(), ks, sizeof(byte) * _keyCount);

	
	_isInitialized = true;
	
	return NR_OK;
}



/**
 * Hide/Show the mouse cursor
 * @param bShow if false cursor will be hided
 **/
void nrCFrameworkSDL::showCursor(bool bShow){
	if (!isInitialized()){
		return;
	}

	if (bShow){
		SDL_ShowCursor(SDL_ENABLE);
	}else{
		SDL_ShowCursor(SDL_DISABLE);
	}
}



/**
 * This function will create an render context (render window) and
 * will return you an id.
 * @param returnID pointer to integer value where id will be returned
 * @param width,height Size of the to created render context(window)
 * @param bpp Bit per pixel for the created rendering context
 * @return either NR_OK or:
 *		- NR_FW_NOT_INITIALIZED if framework was not started before
 *		- NR_OUT_OF_MEMORY if there is no memory to create one
 *		- NR_BAD_PARAMETERS if given pointer is invalid
 **/
nrResult nrCFrameworkSDL::createRenderContext(int32* returnID, int32 width, int32 height, int32 bpp){

	if (!isInitialized()){
		nrLog.Log(NR_LOG_ENGINE, "nrCFrameworkSDL::createRenderContext: WARNING: Framework is not initialized until now");
		return NR_FW_NOT_INITIALIZED;
	}
	
	if (returnID == NULL){
		return NR_BAD_PARAMETERS;
	}
	
	boost::shared_ptr<nrIRenderContext>	cxt;
	
	try{
		nrLog.Log(NR_LOG_ENGINE, "nrCFrameworkSDL: Create SDL render context");


		cxt.reset (new nrCRenderContextSDL(this));

		cxt->setupPixelFormat();
		cxt->changeWindowTitle("nrEngine - Application");
		cxt->changeWindowPosition(0,0);
		cxt->changeResolution(width, height, bpp);
		
		_renderContext.push_back(cxt);
	
	}catch (...){
		nrLog.Log(NR_LOG_ENGINE, "nrCFrameworkSDL: Can not create render context (Out Of Memory)");
		return NR_OUT_OF_MEMORY;
	}
	
	
	try{
		*returnID = _renderContext.size();

		_activehRC = *returnID;
	}catch(...){
		return NR_BAD_PARAMETERS;
	}
	
	nrLog.Log(NR_LOG_ENGINE, "nrCFrameworkSDL: SDL render context created id=%d", *returnID);
	return NR_OK;
}


/**
 * This function will release all reserved memory and will kill the SDL
 * subsystem.
 * @return either NR_OK or:
 *		- NR_FW_NOT_INITIALIZED if no framework initialized until now
 **/
nrResult nrCFrameworkSDL::stop(){

	if (!isInitialized()){
		nrLog.Log(NR_LOG_ENGINE, "nrCFrameworkSDL::stop(): WARNING: want to stop the framework but not initialized");
		return NR_FW_NOT_INITIALIZED;
	}
	
	_isInitialized = false;
	
	return NR_OK;
}


/**
 * Update the engine.
 * Here SDL_Events will proccessing. So we can react to user inputs.
 **/
nrResult nrCFrameworkSDL::update(){

	if (!isInitialized()){
		return NR_FW_NOT_INITIALIZED;
	}

	// sdl event 
    SDL_Event event;

	// check for events 
	while( SDL_PollEvent(&event)){
		switch ( event.type ){
			case SDL_QUIT :
				quit();
				break;
			
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
					_mouseButton[NRBUTTON_LEFT] = true;
				else if (event.button.button == SDL_BUTTON_RIGHT)
					_mouseButton[NRBUTTON_RIGHT] = true;
				break;
			
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT)
					_mouseButton[NRBUTTON_LEFT] = false;
				else if (event.button.button == SDL_BUTTON_RIGHT)
					_mouseButton[NRBUTTON_RIGHT] = false;
				break;

			default:
				break; 
        }
    }

		
	// get keyboard input
	memcpy(_oldKeyState.get(), _keyState.get(), sizeof(byte) * _keyCount);
	byte* ks = SDL_GetKeyState(NULL);
	memcpy(_keyState.get(), ks, sizeof(byte) * _keyCount);
	

	return NR_OK;
}


/**
 * Returns current position of the mouse cursor
 **/
void nrCFrameworkSDL::getCursorPosition(int32* x, int32* y){
	if (!isInitialized() && x!=NULL && y!=NULL){
		*x = 0;
		*y = 0;
		return;
	}

	SDL_GetMouseState(x,y);
}


/**
 * Set the mouse cursor on certain position
 **/
void nrCFrameworkSDL::setCursorPosition(int32 x, int32 y){
	if (!isInitialized()){
		return;
	}
	SDL_WarpMouse(x, y);
}
