/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/**
 * Includes
 **/
#include "nrIFrameWork.h"
#include "../nrEngine.h"

/**
 * Create instance of an framework interface
 **/
nrIFramework::nrIFramework(){
	_activehRC = 0;
	_isInitialized = false;
	_bCallbackSetted = false;
	_pCallbackParam = NULL;
}


/**
 * Release all reserved memory and delete the instance
 **/
nrIFramework::~nrIFramework(){
	
	_keyCount = 0;
	_keyState.reset();
	_oldKeyState.reset();
	
	for (uint32 i=0; i < _renderContext.size(); i++){
		_renderContext[i].reset();
	}
	_renderContext.clear();
	
	
}



/**
 * Get an render context by his id. The returned render context cannot be
 * destroyed or deleted from outside. Only framework has this possibility.
 **/
boost::shared_ptr<nrIRenderContext> nrIFramework::getRenderContext(int32 id){

	ASSERT(id > 0 && id <= static_cast<int32>(_renderContext.size()) && "Invalid id given");
	
	// create an render context with null_deleter(), so it can not be deleted from outside
	boost::shared_ptr<nrIRenderContext> cxt(_renderContext[id-1].get(), null_deleter());
	
	return cxt;
	
}


/**
 * Set the given rendering context as default context to be used.
 * After you set it to default you can simple get the rendering context
 * from elsewhere in your programm without giving them an render context id
 * @param hRC id of the rendering context created before
 * @return either NR_OK or error code
 **/
nrResult nrIFramework::setActiveRenderContext(int32 hRC){
	
	ASSERT(hRC > 0 && hRC <= static_cast<int32>(_renderContext.size()) && "Invalid id given");

	_activehRC = hRC;

	return NR_OK;
}


/**
 * Returns to you the current active rendering context.
 * This function was introduced to give you the possibility to write
 * shorter code
 **/
boost::shared_ptr<nrIRenderContext> nrIFramework::getRC(){

	ASSERT(_activehRC > 0 && _activehRC <= static_cast<int32>(_renderContext.size()) && "No active RC choosen");
	
	return getRenderContext(_activehRC);
}


/**
 * This will destroy a render context which ID is the same as given.
 * @param id ID of the render context to be destroyed
 * @return either NR_OK or:
 *		- NR_BAD_PARAMETERS if id is invalid
 *		- NR_UNKNOWN_ERROR if other error occurs
 **/
nrResult nrIFramework::destroyRenderContext(int32 id){

	if (id <= 0 || id > static_cast<int32>(_renderContext.size())){
		return NR_BAD_PARAMETERS;
	}

	// destroy the context and erase it from the vector
	try{
		nrLog.Log(NR_LOG_ENGINE, "nrIFramework: Destroy render context %d", id);
		_renderContext[id-1].reset();
		_renderContext.erase(_renderContext.begin() + id - 1);
	}catch(...){
		nrLog.Log(NR_LOG_ENGINE, "nrIFramework: There was an error by deleting the render context %d", id);
		return NR_UNKNOWN_ERROR;
	}
	
	return NR_OK;
	
}


/**
 * This function will be called by kernel. Here we initialize our framework
 * engine.
 * @see init() for derived classes
 **/
nrResult nrIFramework::taskInit(){
	if (!isInitialized()){
		return init();
	}

	return NR_OK;
}


/**
 * Return always NR_OK
 **/
nrResult nrIFramework::taskStart(){
	return NR_OK;
}


/**
 * Called to update the framework task. Updating means for example getting
 * the current keyboard state or working through system messages.
 * @see update() for derived classes
 **/
nrResult nrIFramework::taskUpdate(){
	return update();
}


/**
 * This will stop the task and will close the framework engine
 * @see stop() for derived classes
 **/
nrResult nrIFramework::taskStop(){
	return stop();
}


/**
 * Return the current keyboard state. Keyboard state is an smart array
 * containing for each key on the keyboard an value. If such an value is
 * not equal 0, so this key is down. If equal 0 so it is up.
 **/
boost::shared_array<byte> nrIFramework::getKeyboardState(){
	return _keyState;
}


/**
 * Returns the current state of the mouse button
 * @param button is the number of the button (0=left, 1=right, ...)
 * @return true if down, otherwise falls
 **/
bool nrIFramework::getMouseButton(nrMouseButtonIndex button){
	
	ASSERT (button >= 0 && button < NRBUTTON_COUNT);
	
	return _mouseButton[button];
}


/**
 * Here you can setup the callback function which will be called if application
 * should quit. This happens either by quit message from outside (e.g. User close
 * the window) or by internal calling of quit()-method
 * @param fCallback Is an boost functor containing function to be called back
 * @param pCallbackParam Is an parameter which will be given to the callback function.
 **/
void nrIFramework::setQuitCallback(boost::function<void (void*)> fCallback, void* pCallbackParam){
	_fQuitCallback = fCallback;
	_pCallbackParam = pCallbackParam;
	_bCallbackSetted = true;
}


/**
 * Quit the application either by calling internally or from outside signal
 **/
void nrIFramework::quit(){

	nrLog.Log(NR_LOG_ENGINE, "nrCFrameworkSDL::quit(): Quit framework");

	// Stop if not done until now
	stop();

	// Callback
	if (_bCallbackSetted){
		_fQuitCallback(_pCallbackParam);
	}
}

