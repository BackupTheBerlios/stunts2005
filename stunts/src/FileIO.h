/*
*	Includes
*/

#include <stdio.h>
#include <string.h>


#ifndef _FILEIO_H_
#define _FILEIO_H_


/**
*	There are two modi available:
*	BYTE_MODE to write single bytes to the file
*	RECORD_MODE to write a whole record to the file
*/
enum FileStoreMode
{
	BYTE_MODE = 0,
	RECORD_MODE = 1
};

/**
*	There are three modi available:
*	FILE_READ_MODE to have read access
*	FILE_WRITE_MODE to have write access
*
*	additionaly there is a mixture of both: FILE_RW_MODE
*/
enum FileAccessMode
{
	FILE_READ_MODE = 1,
	FILE_WRITE_MODE = 2,
	FILE_RW_MODE = FILE_READ_MODE | FILE_WRITE_MODE
};


/**
*	The FileIO class provides low level access to binary files
*/
class FileIO
{
	public:
		/**
		*	Default constructor, initializes internal variables
		*/
		FileIO();

		/**
		*	Constructor, additionally opens the file in the parameter
		*/
		FileIO(char* fileName, FileAccessMode accessMode = FILE_RW_MODE);

		/**
		*	Destructor
		*/
		~FileIO();

		/**
		*	opens the file in the parameter,
		*	closes the file already opened
		*/
		bool Open(char* fileName, FileAccessMode accessMode);


		/**
		*	appends one byte to the file (BYTE_MODE)
		*/
		bool PushByte(unsigned char byte);

		/**
		*	get next byte if in BYTE_MODE
		*	returns 0 if not in BYTE_MODE or another error occured
		*/
		unsigned char PopByte();


		/**
		*	appends one record to the file (RECORD_MODE)
		*/
		bool PushRecord(void* byteRecord, unsigned int recordSize);

		/**
		*	get record size if in RECORD_MODE
		*/
		unsigned int PopRecordSize();

		/**
		*	get total record from file,
		*	byteRecord must be large enough to hold the data
		*/
		bool PopRecord(void* byteRecord);


		/**
		*	get the total size of the file
		*/
		unsigned int Size();

		/**
		*	the current store mode (r/w)
		*/
		FileStoreMode Mode();

		/**
		*	the current file name
		*/
		char* FileName();

		/**
		*	returns true if a file is open
		*/
		bool IsOpen();


		/**
		*	returns true if the end of the file has been reached
		*/
		bool EndOfFile();


		/**
		*	go to the begin of the file
		*/
		bool GotoBegin();

		/**
		*	go to the next record/byte
		*/
		bool GotoNext();

		/**
		*	push a chunk header
		*/
		bool PushHeader(char* header);

		/**
		*	go to the next header
		*/
		bool GotoHeader(char* header);

	private:
		FileStoreMode mCurrentMode;
		char* mFileName;
		FILE* mFileHandle;
		long mRecordSize;
		long mFileSize;

		/*
		*	determine next mode
		*/
		bool DetermineNextMode();

};

#endif

