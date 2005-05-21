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

#include "nrVFSFile.h"

#include <string.h>
#include <stdlib.h>

#include <iostream>

/**-------------------------------------------------------------------------
 * Constructor
 *------------------------------------------------------------------------*/
//nrCFile::nrCFile(int id, unsigned int length, unsigned char* data){
//	this->ID = id;
//	this->length = length;
//	this->data.reset(data);
//	this->position = 0;
//}

/**
 * Instantiate object of nrCFile-class.
 * @param id - id of the file (ID should be unique)
 * @param length - Count of bytes containing in the data field
 * @param data - smart array containing the data 
 **/
nrCFile::nrCFile(int id, unsigned int length, shared_array<byte> data){
	this->ID = id;
	this->length = length;
	this->data = data;
	this->position = 0;
}


/**
 * Release used memory
 **/
nrCFile::~nrCFile(){
	data.reset();
}

	
/**
 * Returns unbuffered pointer to data from the file.
 * Please be carefull by using non smart pointers, because of segmentation falts.
 **/
const byte* nrCFile::getData(){
	return data.get();
}
	

/**
 * Returns length of file data
 **/
unsigned int nrCFile::dataLength(){
	return this->length;
}
	

/**
 * Read data from file
 * @param ptr - pointer where readed data will be returned
 * @param size - size of one element to be readed
 * @param nmemb - count of elements to be readed (see man fread)
 * @return count of readed elements
 **/
size_t nrCFile::fread(void *ptr, size_t size, size_t nmemb){

	size_t i;
	for (i = 0; i < nmemb; i++) {
		if ((unsigned int)this->position + (unsigned int)size > dataLength()) {
			return i;
		}
		memcpy((unsigned char *)(ptr) + i * size, getData() + this->position, size);
		this->position += size;
	}
	return i;
}


/**
 * Read data until delimiter was found
 * @param lineptr - here the readed string will be returned
 * @param n - number of readed elements will be returned
 * @param delim - delimiter
 * @return -1 if error otherwise number of readed characters including delimiter
 * @note returned data has to be freed with free()
 **/
int nrCFile::getdelim(char **lineptr, size_t *n, unsigned char delim){

	// first find out where first delimiter occurs
	size_t i;
	ssize_t count = 0;
	
	for (i=position; i <= (size_t)dataLength();i++){
		if ( i < (size_t)dataLength()){
			if (getData()[i] != delim){
				count ++;
			}else{
				count += 1;
				break;
			}
		}
	}
	
	// if no such elemnt found, so return -1
	if (i == (size_t)dataLength()) return -1;
	
	// allocate memory and read data
	if ((unsigned int)this->position + (unsigned int)count > dataLength()) {
		count = dataLength() - this->position;
	}

	if (dataLength() - this->position > 0){
		*lineptr = (char*)malloc((count+1)*sizeof(char));

		memcpy(*lineptr, getData() + this->position, count);
		(*lineptr)[count] = 0;
		*n = count;
		this->position += count;
	}else{
		*lineptr = NULL;
		*n = 0;
	}
	return (int)count;
}

/**
 * read() - read data from file
 * @param buf - pointer where readed data will be returned
 * @param count - number of bytes to be readed (if greater than file size)
 *				  file size will be returned
 * @return count of readed bytes
 * @note <b>buf</b> should have at least size <b>count</b>
 **/
size_t nrCFile::read(void *buf, int count){

	if ((unsigned int)this->position + (unsigned int)count > dataLength()){
		count = this->dataLength() - this->position;
	}
	memcpy(buf, this->getData() + this->position, count);
	return (size_t)count;
}


/**
 * Returns current position of file cursor
 **/
long nrCFile::ftell(){
	return this->position;
}

/**
 * Seeks to new position relative to <b>whence</b>
 * @param offset - relative offset
 * @param whence - from where to seek
 * @return -1 if error occurs or 0 otherwise
 **/
int nrCFile::fseek(long offset, int whence){
	
	int real_offset;
	switch (whence) {
	case SEEK_SET:
		real_offset = 0;
		break;
	case SEEK_CUR:
		real_offset = this->position;
		break;
	case SEEK_END:
		real_offset = this->dataLength();
		break;
	default:
		return -1;
	}

	if (real_offset + offset < 0 ||
            real_offset + offset > (int)dataLength()) {
		return -1;
	}

	this->position = real_offset + offset;
	return 0;
}
