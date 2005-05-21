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
#include "../nrEngine.h"
#include "FrameWork.h"


/**
 * Here the framework of the engine will be initialized.
 * Also the default time source for our clock will be created
 * and bounded to the clock. So at this point the default clock
 * should already be created. This is done by just creating the engine.
 * @return either NR_OK or:
 *		- NR_ENGINE_YOU_MUST_CREATE_ENGINE_BEFORE
 * @see nrEngineInit()
 * \ingroup fw
 **/
nrResult nrFrameworkInit(){

	if (!nrEngineIsActive()){
		return NR_ENGINE_YOU_MUST_CREATE_ENGINE_BEFORE;
	}
	
	// create framework object
	nrIFramework::InstantiateAs<nrCFrameworkSDL>();
	
	// create simple time source object
	shared_ptr<nrITimeSource> time (
			new nrCTimeSourceFramework(nrIFramework::GetSmartPtr()));
	nrClock.setTimeSource(time);

	// Initialize the framework by itself
	nrFramework.init();
	
	// OK
	return NR_OK;	
}


/**
 * Delete the framework, so no framework will be active
 * @return always NR_OK
 * \ingroup fw
 **/
nrResult nrFrameworkDelete(){

	// If not allready done, so delete the framework
	nrFramework.stop();
	
	// delete all singletons
	nrIFramework::Release();
	
	// OK
	return NR_OK;
}
