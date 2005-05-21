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

#include "nrVFS.h"
#include "../nrEngine.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef __GNUC__
#include <unistd.h>
#endif
#include <fcntl.h>
#include <sys/stat.h>

#include <iostream>

#define _FILE_MAP std::map<int, shared_ptr<nrCFile> >

/*--------------------------------------------------------------------------
 * Constructor
 -------------------------------------------------------------------------*/
nrCFileSystem::nrCFileSystem(){
	isVFSOpen = false;
	lastGivenFileID = 0;
}

/**
 * Release used memory and close all opened files.
 **/
nrCFileSystem::~nrCFileSystem(){
	if (isVFSOpen){
		closeAllFiles();
	}
}


/**
 * Returns the current used path to our virtual file system. This path is the real path
 * to our virtual filesystem. By using *.pak-files this would be path to the *.pak-file 
 * which is currently opened.
 **/
const char*	nrCFileSystem::getPathToFileSystem(){
	return pathToFileSystem.c_str();
}

	 
/**
 * Just close all currently opened files.
 **/
void nrCFileSystem::closeAllFiles(){

	nrLog.Log(NR_LOG_ENGINE, "VFS: Close all opened files");
	
	// Iterate throuch all opened files and close it.
	_FILE_MAP::iterator it = openedFiles.begin();
	while (it != openedFiles.end()){
		if ((*it).second.get() != NULL){
			(*it).second.reset();
		}
		
		it ++;
	}
	
	openedFiles.clear();
	
}


/**
 * Set real path to our virtual file system file/directory
 * @param pathToVFS - is a string indicating real path to file/dir containing vfs
 **/
void nrCFileSystem::setPathToVFS(const char* pathToVFS){
	pathToFileSystem = pathToVFS;	
}
	


/**
 * Open our virtual file system so that now files can be accessed
 * @return either NR_OK or:
 *		- NR_VFS_ALREADY_OPEN if virtual filesystem is already open
 **/
nrResult nrCFileSystem::openFileSystem(){

	// check whenever vfs is already open
	if (isVFSOpen) return NR_VFS_ALREADY_OPEN;
	
	nrLog.Log(NR_LOG_ENGINE, "VFS: Open virtual filesystem on %s", pathToFileSystem.c_str());
		
	// open file system
	isVFSOpen = true;	
	
	return NR_OK;
}


/**
 * Close our file system, so no access would be possible
 * @return either NR_OK or:
 *		- NR_VFS_IS_NOT_OPEN is vfs is not open at now
 **/
nrResult nrCFileSystem::closeFileSystem(){
	
	// if file system is not opened 
	if (!isVFSOpen) return NR_VFS_IS_NOT_OPEN;

	nrLog.Log(NR_LOG_ENGINE, "VFS: Close virtual filesystem");
	
	// close file system
	isVFSOpen = false;

	return NR_OK;	
}



/**
 * Open a file from our virtual file system
 * @param fileID  - will be filled with ID for opened file
 * @param fileName - is a fileName of file (relative to root dir of virtual fs)
 * @param binaryMode - should we open this file in binary mode (has no effect at now)
 * @return either NR_OK or:
 *		- NR_VFS_IS_NOT_OPEN if virtual filesystem is not open at now
 *		- NR_VFS_FILE_NOT_FOUND if such a file does not exists
 *		- NR_VFS_FILE_READ_ERROR if we have error by reading the file
 *		- NR_OUT_OF_MEMORY if no more memory is available
 *		- NR_UNKNOWN_ERROR
 **/
nrResult nrCFileSystem::openFile(int* fileID, const char* fileName, bool binaryMode){

	try{ 
		
		// We can only open files if vfs is also opened 
		if (!isVFSOpen) {
			*fileID = 0;
			return NR_VFS_IS_NOT_OPEN;
		}
		
		// Generate new ID for the file
		unsigned int id = ++ lastGivenFileID;
		
		// get number of bytes in file and allocate memory to hold data
		struct stat buf;
		if (stat((pathToFileSystem + fileName).c_str(), &buf) == -1){
			return NR_VFS_FILE_NOT_FOUND;
		}
		
		nrLog.Log(NR_LOG_ENGINE, "VFS: Open file %s", fileName);
		
		// Open file and check if no error occurs
		FILE* file = fopen((pathToFileSystem + fileName).c_str(), "rb");
		if (file == NULL) return NR_VFS_FILE_NOT_FOUND;
			
		// allocate memory
		shared_array<byte> data (new byte[buf.st_size]);
		if (data.get() == NULL){
			fclose(file);
			nrLog.Log(NR_LOG_ENGINE, "VFS: Out Of Memory !!!");
			return NR_OUT_OF_MEMORY;		
		}
	
		// read data whole file at once
		int err = fread(data.get(), 1, buf.st_size, file);

		// check errors
		if (feof(file)){
			fclose(file);
			data.reset();
			nrLog.Log(NR_LOG_ENGINE, 
				"VFS: File read error, End of File reached!!!");
			return NR_VFS_FILE_READ_ERROR;
			
		} else if (err != buf.st_size){
			fclose(file);
			data.reset();
			nrLog.Log(NR_LOG_ENGINE, 
				"VFS: File read error (readed %i, need to read %i)!!!", err, buf.st_size);
			return NR_VFS_FILE_READ_ERROR;
		}

		// cached data read		
		//int i = 0;
		/*while (i < buf.st_size) {
			int num_bytes = buf.st_size - i;	
			if (num_bytes > 1024) num_bytes = 1024;
				
			int err = fread(data.get() + i, num_bytes, 1, file);
	
			if (err <= 0){
				fclose(file);
				data.reset();
				return NR_VFS_FILE_READ_ERROR;
			}
			
			i += err * num_bytes;
		}*/
		
		// initialize new file object and pass data to it
		shared_ptr<nrCFile> of(new nrCFile(id, buf.st_size, data));
	
		// close file from reading
		fclose(file);
	
		// store new allocated object in our map
		openedFiles[id] = of;
		*fileID = id;
	
		nrLog.Log(NR_LOG_ENGINE, "VFS: File opened and readed in the buffer");

	}catch(...){
		return NR_UNKNOWN_ERROR;
	}
		
	// OK	
	return NR_OK;	
}


/**
 * Close file opened before by openFile() method
 * @param fileID - is ID of file opened before
 * @return either NR_OK or:
 *		- NR_VFS_IS_NOT_OPEN
 *		- NR_VFS_FILE_NOT_OPEN if such file is not open
 **/
nrResult nrCFileSystem::closeFile(int fileID){

	// We can only access to files if vfs is open 
	if (!isVFSOpen) {
		return NR_VFS_IS_NOT_OPEN;
	}

	// check whenever such file exists
	_FILE_MAP::iterator it = openedFiles.find(fileID);
	if (it == openedFiles.end()){
		return NR_VFS_FILE_NOT_OPEN;	
	}
	
	nrLog.Log(NR_LOG_ENGINE, "VFS: Close file with id=%i", fileID);
	
	// close file specified by file pointer
	(*it).second.reset();
	
	// erase file pointer from our map
	openedFiles.erase(it);
	
	// OK
	return NR_OK;	
}


/**
 * Read a line from opened file. Read until delim is found.
 * @param fileID - is ID of file opened before
 * @param resultStr - Here the readed line will be stored
 * @param delim - Delimiter until which line is readed (Default is \n - newline)
 * @return either NR_OK or:
 *		- NR_BAD_PARAMETERS if parameters are not valid (e.g. resultStr is NULL)
 *		- NR_VFS_IS_NOT_OPEN
 *		- NR_VFS_FILE_NOT_OPEN
 *		- NR_VFS_LINE_READ_ERROR if by reading the line an error occurs
 *		- NR_VFS_FILE_END_REACHED if end reached before delimiter was found
 *		- NR_UNKNOWN_ERROR 
 **/
nrResult nrCFileSystem::readLine(int fileID, std::string* resultStr, char delim){

	try{ 
		
		// Check if parameters are valid
		if (fileID == 0 || resultStr == NULL)
			return NR_BAD_PARAMETERS;
		
		// We can only access to files if vfs is open 
		if (!isVFSOpen) {
			return NR_VFS_IS_NOT_OPEN;
		}
	
		// check whenever such file exists
		_FILE_MAP::iterator it = openedFiles.find(fileID);
		if (it == openedFiles.end()){		
			return NR_VFS_FILE_NOT_OPEN;	
		}
		
		// get file pointer and prepare for reading data
		shared_ptr<nrCFile> file ((*it).second);
		char* line = NULL;
		size_t len = 0;
		int read;
		
		// read line and check if do not have any errors
		read = file->getdelim(&line, &len, delim);
		if (read == -1) 
			return NR_VFS_LINE_READ_ERROR;	
	
	
		// allocate memory for readed data and copy it 
		//line[len] = '\0';
		if (len != 0){
			*resultStr = line;	
			free(line);
		}else{
			return NR_VFS_FILE_END_REACHED;
		}

	}catch(...){
		return NR_UNKNOWN_ERROR;
	}
	
	// OK
	return NR_OK;
}


/**
 * Reads binary data from opened file
 * @param fileID - ID of file opened before
 * @param bytesToRead - bytes count to be readed from file
 * @param resultData - pointer which will be filled with data. Memory must be
 * 		  already allocated to store data
 * @return either NR_OK or:
 *		- NR_BAD_PARAMETERS
 *		- NR_VFS_IS_NOT_OPEN
 *		- NR_VFS_FILE_NOT_OPEN
 *		- NR_VFS_FILE_READ_ERROR
 *		- NR_UNKNOWN_ERROR
 **/
nrResult nrCFileSystem::readData(int fileID, unsigned int bytesToRead, void* resultData){
	
	try{
		
		// Check if parameters are valid
		if (fileID == 0 || resultData == NULL)
			return NR_BAD_PARAMETERS;
		
		// We can only access to files if vfs is open 
		if (!isVFSOpen) {
			return NR_VFS_IS_NOT_OPEN;
		}
	
		// check whenever such file exists
		_FILE_MAP::iterator it = openedFiles.find(fileID);
		if (it == openedFiles.end()){
			return NR_VFS_FILE_NOT_OPEN;	
		}
		
		// read data and store it under given data pointer
		shared_array<uchar> data (new uchar[bytesToRead]);
		size_t read = (*it).second->fread(data.get(), 1, bytesToRead);
		
		// If we readed less elements than we want so it could be an error 
		if (read == 0 || read != bytesToRead){
			data.reset();
			return NR_VFS_FILE_READ_ERROR;			
		}
		
		// Read was sucessfull, so copy readed data and free memory
		memcpy(resultData, data.get(), bytesToRead);
		data.reset();
	
	}catch(...){
		return NR_UNKNOWN_ERROR;
	}
	
	// OK
	return NR_OK;
	
}


/**
 * Reads a formatted string from opened file
 * @param fileID - ID of file opened before
 * @param wantToRead - count of items of data you want to read
 * @param format - format of readed string
 * @return either NR_OK or:
 *		- NR_VFS_SCAN_READ_ERROR if wantToRead is not equal to scanned items
 *		- NR_UNKNOWN_ERROR
 * @note Only linewise scanning
 **/
nrResult nrCFileSystem::scanRead(int fileID, unsigned int wantToRead, const char* format, ...){

	try{ 
		
		// read one line from file
		std::string line;
		nrResult ret = readLine(fileID, &line);	
		if (ret != NR_OK) return ret;
			
		unsigned int readed = 0;
		// Scan the string from string
		if (line.length() > 0){	
			va_list ap;
			va_start(ap, format);
			readed = vsscanf(line.c_str(), format, ap);
			va_end(ap);             
		}
		// check whenever we have whished scan size 
		if (wantToRead != readed)
			return NR_VFS_SCAN_READ_ERROR;

	}catch(...){
		return NR_UNKNOWN_ERROR;
	}
	
	// OK
	return NR_OK;
}


/**
 * Seek file to new position
 * @param fileID - ID of file opened before
 * @param pos - Position to wheach it should be seeked
 * @param whence - can be either SEEK_SET, SEEK_CUR, SEEK_END
 * @return either NR_OK or:
 *		- NR_BAD_PARAMETERS if fileID is equal 0
 *		- NR_VFS_IS_NOT_OPEN
 *		- NR_VFS_FILE_NOT_OPEN
 *		- NR_VFS_SEEK_ERROR if there is an error by seeking
 **/
nrResult nrCFileSystem::seek(int fileID, long pos, int whence){
	// Check if parameters are valid
	if (fileID == 0)
		return NR_BAD_PARAMETERS;
	
	// We can only access to files if vfs is open 
	if (!isVFSOpen) {
		return NR_VFS_IS_NOT_OPEN;
	}

	// check whenever such file exists
	_FILE_MAP::iterator it = openedFiles.find(fileID);
	if (it == openedFiles.end()){
		return NR_VFS_FILE_NOT_OPEN;	
	}

	// Seek file
	int k = (*it).second->fseek(pos, whence);	
	if (k == -1) return NR_VFS_SEEK_ERROR;
		
	// OK
	return NR_OK;
}


/**
 * Returns file smart pointer to file if it's opened
 * @param fileID - ID of file opened before
 * @returns a smart pointer to the file object. If such a file is not open, so return smart pointer to NULL
 **/
shared_ptr<nrCFile> nrCFileSystem::getFile(int fileID){
	// Check if parameters are valid
	if (fileID == 0)
		return shared_ptr<nrCFile>();
	
	// We can only access to files if vfs is open 
	if (!isVFSOpen) {
		return shared_ptr<nrCFile>();
	}

	// check whenever such file exists
	_FILE_MAP::iterator it = openedFiles.find(fileID);
	if (it == openedFiles.end()){
		return shared_ptr<nrCFile>();	
	}
	
	return (*it).second;
}

#undef _FILE_MAP
