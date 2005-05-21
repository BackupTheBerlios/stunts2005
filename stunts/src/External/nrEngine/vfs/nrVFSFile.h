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

#ifndef _NR_CVFFILES_H_
#define _NR_CVFFILES_H_


/*--------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include "VFS.h"

#include <stdio.h>
#include <boost/shared_array.hpp>
using boost::shared_array;


/**
 * This class represents any file of our virtual file system. 
 * By opening of any file through virtual filesystem manager, the whole file content 
 * will be readed in and you get access to data sorted in the file. This class implements
 * simple functions like: fread(), fseek() and so on. So if you know how stdio-library works
 * you also know how this class should work. 
 * Most of all functions are defined as virtual, so you can ovverride them. This can help
 * us to create files that support compression (like *.zip) or something else.
 * You can use this class as standalone by providing whole data from the file to an object
 * of this class. nrCFileSystem do this in the same way.
 * @todo Try to create buffered file system. That means you do not have to load whole content from 
 * the virtual file system, just parts you need.
 * @see nrCFileSystem, <a href="http://www.boost.org">boost.org</a>
 * @note be carefull by using methods which handles with non smart pointers.
 * It is better if you always use smart pointers
 * \ingroup vfs
 **/
class nrCFile {
public:

	// Constructor & Destructor
	//nrCFile(int id, unsigned int length, unsigned char* data);
	nrCFile(int id, unsigned int length, shared_array<byte> data);
	virtual ~nrCFile();
	
	// Get data from file (unbuffered)
	virtual const byte* getData();
	
	// Get length of the file
	virtual unsigned int dataLength();
	
	// file functions like from stdio.h
	virtual size_t fread(void *ptr, size_t size, size_t nmemb);
	virtual size_t read(void *buf, int count);
	virtual long ftell();
	virtual int  fseek(long offset, int whence);
	virtual int getdelim(char **lineptr, size_t *n, unsigned char delim);
	
	// get id
	int getID(){return ID;}
	
protected:
	// Id of file given from vfs class
	int	ID;
	
	// Position of file cursor
	size_t	position;

	// Length of file
	unsigned int length;
	
	// stored (unbuffered) data
	shared_array<byte>	data;
		
};

#endif
