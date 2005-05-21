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


#ifndef _NR_I_TIMEOBSERVER_H_
#define _NR_I_TIMEOBSERVER_H_


  
/**
 * nrITimeObserver is an objects who observers our global clock. Each time clock ticks
 * (this is not every second, but every frame) observer will be notified about
 * this and can do his work. Good example for such an observer is an timer, who
 * is updated in every frame and provide time for parts of your application.
 * This also can be an alarm-clock which wait until certain time point and then fires.
 *
 * \ingroup time
 **/
class nrITimeObserver {
public:
	
	// Clock class is a friend, so he has full access
	friend class nrCClock;
	
	// constructor & destructor
	nrITimeObserver(){_observerID = 0;}
	virtual ~nrITimeObserver(){}
	
	/**
	 * Clock should notify me every frame.
	 **/
	virtual void notifyTimeObserver() = 0;
	
	/**
	 * Returns the observer id given us by the clock.
	 * You will need this id if you want to delete observer
	 * from the clock's list of observers
	 **/
	int getObserverID(){return _observerID; }
	
	
private:
	int _observerID;
	
};


#endif
