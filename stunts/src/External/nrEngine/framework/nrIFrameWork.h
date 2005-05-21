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
#ifndef _NR_I_FRAMEWORK_H
#define _NR_I_FRAMEWORK_H


/*--------------------------------------------------------------------------
 * What this file provides?
 *------------------------------------------------------------------------*/
class nrIFramework;


/*--------------------------------------------------------------------------
 * What this file need?
 *------------------------------------------------------------------------*/
class nrIRenderContext;


/*--------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include "../nrISingletonTask.h"
#include "nrIRenderContext.h"

#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <vector>
#include <string>


/**
 * This is a base class for the framework. This interface defines basic functions
 * which should be implemented by each framework class. Framework is used to create
 * rendering window or to get input from user.
 * You can use it to stay independent from operating system.
 *
 * \ingroup fw
 **/
class nrIFramework: public nrISingletonTask<nrIFramework> {
public:

	// constructor & destructor
	nrIFramework();
	virtual ~nrIFramework();
	
	
	// Render Context functions
	virtual nrResult							createRenderContext(int32* returnID, int32 width, int32 height, int32 bpp) = 0;
	boost::shared_ptr<nrIRenderContext> 		getRenderContext(int32 id);
	boost::shared_ptr<nrIRenderContext> 		getRC();
	nrResult									destroyRenderContext(int32 id);
	nrResult									setActiveRenderContext(int32 hRC);
	

	// Framework's functions
	virtual nrResult		init() = 0;
	virtual nrResult		update() = 0;
	virtual nrResult		stop() = 0;
	void					quit();
	
	// Input cursor functions
	virtual void	showCursor(bool bShow) = 0;
	virtual void	getCursorPosition(int32* x, int32* y) = 0;
	virtual void	setCursorPosition(int32 x, int32 y) = 0;
	bool	getMouseButton(nrMouseButtonIndex button);


	// Input Keyboard functions
	boost::shared_array<byte>	getKeyboardState();
	
	bool inline keyPush(nrKeyIndex index)		{ return ( curKey(index))&&(!oldKey(index)); }
	bool inline keyDown(nrKeyIndex index)		{ return ( curKey(index))&&( oldKey(index)); }
	bool inline keyPull(nrKeyIndex index)		{ return (!curKey(index))&&( oldKey(index)); }
	bool inline keyUp(nrKeyIndex index)			{ return (!curKey(index))&&(!oldKey(index)); }

	// Time functions
	virtual uint32	getTicks() = 0;

	// Initialized functions
	bool	isInitialized(){return _isInitialized;}

	// Quit-Callback-Function
	void	setQuitCallback(boost::function<void (void*)> fCallback, void* pCallbackParam);

	/**
	 * Overloaded function from nrISingletonTask - Interface. We need to overload this
	 * function here, because we want that the kernel does not allowed to delete
	 * the framework task from the memory. Only the user can delete it. So we give the
	 * task with null_deleter() to the kernel.
	 * @param kernel object of nrCKernel-Interface
	 * @param prior - priority for the task
	 * @return result provided by @a nrCKernel::AddTask() method
	 **/
	nrResult AddToKernel(nrCKernel& kernel, nrPriority prior = 0){
		if (prior != 0) this->_taskPriority = prior;

		// I create a smart pointer to myself with null_deleter()
		shared_ptr<nrITask> pTask(
				dynamic_cast<nrITask*>(nrIFramework::GetSingletonPtr()),
				null_deleter());
		
		return kernel.AddTask(pTask);
	}

	// Task functions
	nrResult		taskInit();
	nrResult		taskStart();
	nrResult		taskUpdate();
	nrResult		taskStop();
	const char* 	taskGetName(){return "nrIFramework";}

protected:

	// Initialized
	bool		_isInitialized;


	// callback functions
	boost::function<void (void*)> 	_fQuitCallback;
	void*							_pCallbackParam;
	bool							_bCallbackSetted;
	
	// Render contexts
	std::vector<boost::shared_ptr<nrIRenderContext> >	_renderContext;
	int32	_activehRC;
	
	
	// Keyboard state
	int32						_keyCount;
	boost::shared_array<byte>	_keyState;
	boost::shared_array<byte>	_oldKeyState;

	// Mouse state
	bool	_mouseButton[NRBUTTON_COUNT];
	
private:

	
	// Methods to get keyboard input
	bool inline curKey(nrKeyIndex index) {
		ASSERT(_keyState != NULL && index >=0 && index < _keyCount);
		return (_keyState[index]!=0);
	}
	
	bool inline oldKey(nrKeyIndex index) {
		ASSERT(_oldKeyState != NULL && index >=0 && index < _keyCount);
		return (_oldKeyState[index]!=0);
	}

	
};

 
#endif
