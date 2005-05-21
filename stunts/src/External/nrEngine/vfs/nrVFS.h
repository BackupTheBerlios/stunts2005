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
 
#ifndef _NR_CVFS_H_
#define _NR_CVFS_H_

/**-------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include <map>
#include <string>
#include <stdarg.h>

#include "../nrTypes.h"
#include "../nrISingleton.h"
#include "nrVFSFile.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

using boost::shared_ptr;
using boost::weak_ptr;


/**
 * nrCFileSystem is a class representing virtual file system for our game.
 * It has support for opening and loading files from game own file system.
 * We need our own file system to allow to store all game data in one or
 * more big files. Each game has thousands of files provided with it. The size
 * of this file on the disk can be very big. With help of internal filesystem you
 * can compress all files into big one and use this compression transparent to the rest
 * of the game. It allows also access to files by not knowing how the real path string
 * should looks like on used operating system. nrCFileSystem do this work for you.
 * @todo Implement support for paking/compressing the filesystem (e.g. *.pak file)
 * \ingroup vfs
 **/
class nrCFileSystem : public nrISingleton <nrCFileSystem> {
private:

	// Stores path on the real file system to our virtual.
	// If our file system is only one file on the system then this is a path to 
	// this file. Else if it is a directory then variable contains path to directory
	std::string	pathToFileSystem;
	bool	isVFSOpen;
	unsigned int lastGivenFileID;

	// This hash_map stores our currently opened files.
	std::map<int, shared_ptr<nrCFile> >	openedFiles;
	
	// Private methods
	void	closeAllFiles();
	
public:

	// Constructor & Destructor
	nrCFileSystem();
	virtual ~nrCFileSystem();
	
	// Init methods
	void 		setPathToVFS(const char* pathToVFS);
	nrResult	openFileSystem();
	const char*	getPathToFileSystem();
	
	// Deinit methods
	nrResult	closeFileSystem();

	// Access methods
	nrResult	openFile(int* fileID, const char* fileName, bool binaryMode = false);
	nrResult	readLine(int fileID, std::string* resultStr, char delim='\n');
	nrResult	readData(int fileID, unsigned int bytesToRead, void* resultData);
	nrResult	scanRead(int fileID, unsigned int wantToRead, const char* format, ...);
	nrResult	seek(int fileID, long pos, int whence);
	nrResult	closeFile(int fileID);

	// Get file object directly
	shared_ptr<nrCFile>	getFile(int fileID);
	
};

#endif
