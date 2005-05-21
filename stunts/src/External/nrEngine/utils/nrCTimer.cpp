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

#include "nrCTimer.h"
#include "../nrEngine.h"

#include <boost/shared_ptr.hpp>


/**
 * Define default values and set clock for using with this timer.
 * This timer will automaticly be added to the clock as observer.
 **/
nrCTimer::nrCTimer(nrCClock& clock): _clock(clock){
	
	// unpause the timer and set default scaling
	setPause(false);
	setScale (1.0f);
	
	// set default values
	_currentTime = 0;
	_frameTime = 0;
		
}



	
/**
 * Release used memory and remove this timer from observer list of
 * the clock
 **/
nrCTimer::~nrCTimer(){

}


/**
 * This function will be always called by clock.
 **/
void nrCTimer::notifyTimeObserver(){
	// if there is no pause, so update times
	if (!_bPaused){
		_frameTime = _clock.getFrameInterval() * _fScale;
		_currentTime += _frameTime;
	}
}



