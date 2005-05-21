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


#ifndef _NR_C_TIMESOURCE_FRAMEWORK_H_
#define _NR_C_TIMESOURCE_FRAMEWORK_H_


#include "nrITimeSource.h"

/**
 * nrCTimeSourceFramework is a simple class using engine's framework to get
 * the time passed. This will be instantiated per default and clock engine
 * will use them per default.
 * \ingroup time
 **/
class nrCTimeSourceFramework : public nrITimeSource {
private:

	shared_ptr<nrIFramework>	fw;
	
public:
	
	// constructor & destructor
	nrCTimeSourceFramework(shared_ptr<nrIFramework> fw){this->fw = fw;}
	virtual ~nrCTimeSourceFramework(){}
	
	double getTime();
	
};


#endif
