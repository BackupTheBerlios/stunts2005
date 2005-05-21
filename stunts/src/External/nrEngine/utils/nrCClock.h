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


#ifndef _NR_C_CLOCK_H_
#define _NR_C_CLOCK_H_


/**-------------------------------------------------------------------------
 * Forward declarations
 *------------------------------------------------------------------------*/
class nrCClock;

/**-------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include "../nrISingletonTask.h"
#include "../nrEngine.h"

#include "nrITimeSource.h"
#include "nrITimeObserver.h"
#include "nrCTimer.h"
#include "nrCTimeSourceFramework.h"

#include <deque>
#include <vector>
#include <map>
#include <string>

#include <boost/shared_ptr.hpp>


using std::vector;
using std::deque;
using std::map;
using std::string;

using boost::shared_ptr;

/**
 * nrCClock allows you to create timers from it. Clock is always ticks if any 
 * time source is bounded. In contrast to Einstein's relativity theory we have
 * only one global clock. So your complete application has one time which runs
 * with the same speed. But to be able to use Einstein's theory you can create 
 * timers from each clock, which allows you to use different time speeds.
 * Clock can also derive/create timers. But if you want to create the timer by
 * yourself you can do this.
 * @note 
 *		- Our clock works internal with 64Bit double values to stay precise.
 * 			Because using of float values is enough for every simulation you will get 
 * 			32Bit float values as frame interval time.
 * 		- If you compile the engine on 64Bit systems, so you can change this and use
 *			64Bit doubles instead.
 *		- Each created timer dies when clock dies.
 *		- You can only destroy timers through the clock interface or it will be 
 *			deleted automaticly from the memory when clock is deleted
 *		- Deleting the timer outside from the clock has no effect
 *
 * \ingroup time
 **/
class nrCClock : public nrISingletonTask<nrCClock> {
public:

	// Constructor & Destructor
	nrCClock();
	~nrCClock();
	
	// time source access
	void setTimeSource(const shared_ptr<nrITimeSource> timeSource);
	bool	isAnySourceBounded();
	
	// task functions
	nrResult taskUpdate();
	nrResult taskInit(){return NR_OK;};
	nrResult taskStart(){return NR_OK;};
	nrResult taskStop(){return NR_OK;}
	const char* taskGetName(){return "nrCClock";}
	
	/**
	 * Same as taskUpdate(). This name is choosen to separate calls
	 * to the clock by not knowing, that it is a task.
	 * Use this function from somewhere else than a kernel.
	 **/
	void		Ticks(){taskUpdate();}
	
	// time access
	double 	getTime() const;
	float 	getFrameInterval() const;
	int 		getFrameNumber() const;
	float		getFrameRate() const;

	// traditional time observer access
	int 			addObserver(shared_ptr<nrITimeObserver> timeObserver);
	nrResult	removeObserver(int observerID);
	
	// special time observer access
	nrResult	addObserver(const string& obsName, shared_ptr<nrITimeObserver> timeObserver);
	nrResult	removeObserver(const string& obsName);
	
	// addiditional methods
	void	setFrameWindow(int frameCount = 10, float defaultFrameTime = 0.03333);
	void	setFixFrameRate(bool setFixRate, float fixFrameRate = 60.0f);
	
	// creating the timers
	shared_ptr<nrCTimer>	createTimer();
	
private:

	// observers
	typedef vector<shared_ptr<nrITimeObserver> > ObserverList;
	map <string, int>	observerIDList;
	ObserverList			observers;
	
	// frame filtering
	deque<double>	frameDurationHistory;
	double				frameDefaultTime;
	int						frameFilteringWindow;
	
	// time source
	shared_ptr<nrITimeSource>	timeSource;
	double	currentTime;
	double	frameTime;
	int			frameNumber;
	bool		useFixRate;
	float		fixFrameRate;
	
	// methods
	void		_addToFrameHistory(double duration);
	double	_getFilteredFrameDuration() const;
	double	_getExactFrameDuration();
	
	// helping variables to find out the frame rate
	double sourceStartValue;
	double sourceLastValue;

  
};


#endif
