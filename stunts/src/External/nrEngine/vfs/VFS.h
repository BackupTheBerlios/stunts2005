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
#ifndef _NR__VFS_H
#define _NR__VFS_H

/*!
 * \defgroup vfs Virtual filesystem
 *
 * This is our implementation of game's filesystem. nrEngine gives you support
 * to use simple virtual filesystem in your applications. Just setup nrVFS
 * with any path to virtual filesystem object. It can be either directory or
 * any file (e.g. *.pak) which contains game data. After nrVFS was initialized
 * you can access stored data transparently without knowing how this data is stored
 * by just using these classes. You can also forget about path names on different
 * operating systems. Just use unix like pathnames (e.g. "sounds/data.wav").
 * @note There is at now no support for packed files or other things like 
 * path conversion. 
 * @todo Expand our virtual filesystem classes
 * \author Art Tevs <tevs@mpi-sb.mpg.de>
 */
 
/*--------------------------------------------------------------------------
 * Provided classes
 *------------------------------------------------------------------------*/
class nrCFileSystem;
class nrCFile;

/*--------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include "../nrTypes.h"
#include "nrVFS.h"
#include "nrVFSFile.h"


 
#endif
