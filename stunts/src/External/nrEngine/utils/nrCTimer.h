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


#ifndef _NR_C_TIMER_H_
#define _NR_C_TIMER_H_


/**-------------------------------------------------------------------------
 * Forward declarations
 *------------------------------------------------------------------------*/
class nrCTimer;


/**-------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include "nrCClock.h"
#include "nrITimeObserver.h"


/**
 * nrCTimer is an standard timer shipped with the engine. You can 
 * create your own timers because they are specialized for your application.
 * This timer can only ticks and provide user with information he/she need.
 * Also this timer supports scaling and pausing, so you are able to use
 * at lesser speed or hold on parts of your game using this timer.
 *
 * @note Because each timer is created from the clock, so the timer lifespan
 * is the same as from the clock. So if clock is destroyed, timer will also
 * be killed. You also can not destroy the timer object, only clock can do this. 
 *
 * \ingroup time
 **/
class nrCTimer: public nrITimeObserver {
public:

	// constructor & destructor
	nrCTimer(nrCClock& clock);
	virtual ~nrCTimer();
	
	/**
	 * Returns current time since this timer is started
	 **/
	double getTime() const { return _currentTime; }
	
	/**
	 * Returns the time interval between two frames
	 * Frame interval depends on the scale value you set for
	 * this timer. So it can be different for different timers
	 **/
	float getFrameInterval() const { return static_cast<float>(_frameTime); }

	/**
	 * Returns true if this timer is currently sleeping/paused
	 **/
	bool isPaused () const{return _bPaused; }
		
	/**
	 * Returns the time scale value of this timer. Each timer can have each own
	 * opinion how much time is passed. This is done by setting the scale value
	 * of the timer. This scale shows how much faster/slower this timer runs
	 **/
	float getScale () const{return _fScale; }

	/**
	 * This will hold on this timer so time stops
	 * @param bOn if true timer stops, otherwise it runs
	 **/
	void setPause (bool bOn){ _bPaused = bOn; }
	
	/**
	 * Set time scale value for this timer. With the help of this value you have
	 * the possibility to run your local time at different speed
	 **/
	void	setScale (float fScale){ _fScale = fScale; }
	
	// notify function from nrITimeObserver-Interface
	virtual void notifyTimeObserver();

private:
	
	// clock to be used with this timer	
	nrCClock& _clock;
	
	// current time and interval
	double	_currentTime;
	double	_frameTime;
	
	// pausing and scaling
	bool		_bPaused;
	float		_fScale;
			
};


#endif
