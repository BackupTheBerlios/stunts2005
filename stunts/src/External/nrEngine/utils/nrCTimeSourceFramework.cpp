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
#include "nrCTimeSourceFramework.h"

 	
/**
	* Get the current time in seconds. Returned time is not the same as system time.
	* @return current time in seconds
	**/
double nrCTimeSourceFramework::getTime(){
	
	static bool error_given = false;
	
	// only if framework is active
	try{
		return static_cast<double>(fw->getTicks()) * 0.001;
	}catch(string k){
		if (!error_given){
			nrLog.Log(NR_LOG_ENGINE, "nrCTimeSourceFramework::getTime(): %s", k.c_str());
			error_given = true;
		}
		return 0;
	}
	
}

