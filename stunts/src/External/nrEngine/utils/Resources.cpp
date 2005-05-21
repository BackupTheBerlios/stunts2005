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
#include "Resources.h"

/**
 * This will initialize the resource management system.
 * You have to call this before you can use it.
 *
 * \ingroup resource
 **/
void nrResourceInit(){
	nrCResourceManager::Instantiate();
}


/**
 * This will deactive the resource management system. Call this
 * if you do not need the resource database anymore.
 *
 * \ingroup resource
 **/ 
void nrResourceDelete(){
	nrCResourceManager::Release();
}


