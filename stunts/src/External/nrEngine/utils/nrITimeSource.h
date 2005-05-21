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


#ifndef _NR_I_TIMESOURCE_H_
#define _NR_I_TIMESOURCE_H_


/*!
 * \defgroup time Clock and Timers
 *
 * This groups contains all classes and interfaces you will need by working with the time.
 * Time is relative like Einstein said. So this part of the engine allows you to use
 * this aspect in your applications.
 * To use this you have to derive a time source class and bind it to any clock. Then
 * the clock should ticks and will give you always right time. Now you create any timer
 * from the clock. The timer has its own speed relative to the clock. This allows you
 * to use one timer for GUI and one for your game. If you pause the game timer gui
 * is still working. Also some nice effects are possible by using more than one
 * timer in you rapplication.
 **/

  
/**
 * nrITimeSource is an interface for each time emitter. You have to bind any time source to
 * the clock to have possibility to know the current time. You can think on time source
 * as on tick mechanism in every clock.
 * \ingroup time
 **/
class nrITimeSource {
public:
	
	// constructor & destructor
	nrITimeSource(){}
	virtual ~nrITimeSource(){}
	
	/**
	 * Get the current time in seconds. Returned time is not the same as system time.
	 * @return current time in seconds
	 **/
	virtual double getTime() = 0;
	
};


#endif
