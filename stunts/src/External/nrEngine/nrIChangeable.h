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

#ifndef __NR_I_CHANGEABLE_H_
#define __NR_I_CHANGEABLE_H_

/**
 * nrIChangeable - is an interface which provides a method for registering
 * variables which can be changed by settings engine
 * How To Use: Just derive any class from this interface and you have to implement
 * registerAllVars - Function. This function should register variables by settings
 * manager @a nrCSettings to allow storing its values in settings files and so on.
 * \ingroup gp
 */
class nrIChangeable{
public:

	/**
	* Register variables by settings manager to allow access to it from console or
	* just by name (name is a string)
	**/
	virtual void registerAllVars() = 0;
	
	
	/**
	* Deregister/remove variables from settings manager if you do not need settings
	* functionality anymore 
	**/
	virtual void deregisterAllVars() = 0;
};

#endif
