#include "FileIO.h"

/*
*	Default constructor, initializes internal variables
*/
FileIO::FileIO()
{
	mCurrentMode = BYTE_MODE;
	mFileName = NULL;
	mFileHandle = NULL;
	mRecordSize = 0;
	mFileSize = 0;
}

/*
*	Constructor, additionally opens the file in the parameter
*/
FileIO::FileIO(char* fileName, FileAccessMode accessMode)
{
	mCurrentMode = BYTE_MODE;
	mFileName = NULL;
	mFileHandle = NULL;
	mRecordSize = 0;
	mFileSize = 0;

	FileIO::Open(fileName, accessMode);
}

/*
*	Destructor
*/
FileIO::~FileIO()
{
	if (mFileName)
		delete mFileName;

	if (mFileHandle != NULL)
		fclose(this->mFileHandle);
}

/*
*	opens the file in the parameter,
*	closes the file already opened
*/
bool FileIO::Open(char* fileName, FileAccessMode accessMode)
{
	if (!fileName)
		return false;

	if (mFileName)
		delete mFileName;
	mFileName = NULL;

	if (this->mFileHandle != NULL)
		fclose(this->mFileHandle);


	//open the file
	char* openAccess;

	if (accessMode == FILE_RW_MODE)
		openAccess = "a+b";
	else if (accessMode == FILE_READ_MODE)
		openAccess = "rb";
	else
		openAccess = "wb";

	this->mFileHandle = fopen(fileName, openAccess);

	if (this->mFileHandle == NULL)
		return false;

	//disable buffering
	setvbuf(this->mFileHandle, NULL, _IONBF, 0);

	//get file size
	fseek(this->mFileHandle, 0, SEEK_END);
	mFileSize = ftell(this->mFileHandle);
	fseek(this->mFileHandle, 0, SEEK_SET);


	//copy new file name
	int fileNameLength = strlen(fileName);
	this->mFileName = new char[fileNameLength + 1];
	this->mFileName[fileNameLength] = 0;
	memcpy(this->mFileName, fileName, fileNameLength);


	//initialize other attributes
	mRecordSize = 0;
	DetermineNextMode();

	return true;
}


/*
*	appends one byte to the file (BYTE_MODE)
*/
bool FileIO::PushByte(unsigned char byte)
{
	//initialize other attributes
	mCurrentMode = BYTE_MODE;
	mRecordSize = 0;

	//check for errors
	if (this->mFileHandle == NULL)
		return false;

	//goto end
	fseek(this->mFileHandle, 0, SEEK_END);


	int rval;
	unsigned char byteMode = BYTE_MODE;

	//write byte mode
	rval = fwrite(&byteMode, 1, 1, this->mFileHandle);

	if (rval != 1)
		return false;

	mFileSize++;

	//write byte
	rval = fwrite(&byte, 1, 1, this->mFileHandle);

	if (rval != 1)
	{
		mFileSize--;

		return false;
	}

	mFileSize++;

	return true;
}

/*
*	get next byte if in BYTE_MODE
*	returns 0 if not in BYTE_MODE or another error occured
*/
unsigned char FileIO::PopByte()
{
	//check for errors
	if (this->mCurrentMode != BYTE_MODE ||
		(this->mFileHandle == NULL) ||
		EndOfFile())
		return 0;

	unsigned char currentByte;
	int rval;
	rval = fread(&currentByte, 1, 1, this->mFileHandle);

	//initialize other attributes
	mRecordSize = 0;

	//check for errors
	if (rval != 1)
		return 0;

	DetermineNextMode();

	return currentByte;
}


/*
*	appends one record to the file (RECORD_MODE)
*/
bool FileIO::PushRecord(void* byteRecord, unsigned int recordSize)
{
	//initialize other attributes
	mCurrentMode = RECORD_MODE;
	mRecordSize = 0;

	//check for errors
	if ((this->mFileHandle == NULL) || !byteRecord || !recordSize)
		return false;

	//goto end
	fseek(this->mFileHandle, 0, SEEK_END);


	unsigned int rval;
	unsigned char recordMode = RECORD_MODE;

	//write record mode
	rval = fwrite(&recordMode, 1, 1, this->mFileHandle);

	if (rval != 1)
		return false;

	mFileSize++;

	//write record size
	rval = fwrite(&recordSize, 1, sizeof(unsigned int), this->mFileHandle);


	if (rval != sizeof(unsigned int))
	{
		mFileSize--;

		return false;
	}

	mFileSize+=sizeof(unsigned int);


	//write record
	rval = fwrite(byteRecord, 1, recordSize, this->mFileHandle);

	if (rval != recordSize)
	{
		mFileSize-= sizeof(unsigned int);

		return false;
	}

	mFileSize+=recordSize;

	return true;
}

/*
*	get record size if in RECORD_MODE
*/
unsigned int FileIO::PopRecordSize()
{
	//check for errors
	if (this->mCurrentMode != RECORD_MODE ||
		(this->mFileHandle == NULL) ||
		EndOfFile())
		return 0;

	//if PopRecordSize has already been called
	if (mRecordSize)
		return mRecordSize;

	//read RecordSize
	int rval;
	rval = fread(&mRecordSize, 1, sizeof(unsigned int), this->mFileHandle);

	if (rval == sizeof(unsigned int))
		return mRecordSize;

	return 0;
}

/*
*	get total record from file,
*	byteRecord must be large enough to hold the data
*/
bool FileIO::PopRecord(void* byteRecord)
{
	//check for errors
	if ((this->mCurrentMode != RECORD_MODE) || !byteRecord ||
		(this->mFileHandle == NULL) ||
		EndOfFile())
		return false;

	//if PopRecordSize has not been called
	PopRecordSize();

	if (!mRecordSize)
		return false;

	//read Record
	int rval;
	rval = fread(byteRecord, 1, mRecordSize, this->mFileHandle);

	//check for errors
	if (rval != mRecordSize)
		return false;

	//initialize other attributes
	mRecordSize = 0;

	DetermineNextMode();

	return true;
}

/*
*	get the total size of the file
*/
unsigned int FileIO::Size()
{
	return this->mFileSize;
}

/*
*	the current store mode (r/w)
*/
FileStoreMode FileIO::Mode()
{
	return this->mCurrentMode;
}

/*
*	the current file name
*/
char* FileIO::FileName()
{
	return this->mFileName;
}

/*
*	returns true if a file is open
*/
bool FileIO::IsOpen()
{
	return (this->mFileHandle != NULL ? true : false);
}


/*
*	returns true if the end of the file has been reached
*/
bool FileIO::EndOfFile()
{
	if (this->mFileHandle == NULL)
		return true;

	return (ftell(this->mFileHandle) >= this->mFileSize);
}


/*
*	go to the begin of the file
*/
bool FileIO::GotoBegin()
{
	//initialize other attributes
	mRecordSize = 0;

	//check for errors
	if (this->mFileHandle == NULL)
		return false;

	fseek(this->mFileHandle, 0, SEEK_SET);
	DetermineNextMode();

	return true;
}


/*
*	go to the next record/byte
*/
bool FileIO::GotoNext()
{
	//initialize other attributes
	mRecordSize = 0;

	//check for errors
	if (this->mFileHandle == NULL)
		return false;


	if (this->mCurrentMode == BYTE_MODE)
	{
		fseek(this->mFileHandle, 1, SEEK_CUR);

		DetermineNextMode();
	}
	else if (this->mCurrentMode == RECORD_MODE)
	{
		this->mRecordSize = PopRecordSize();

		if (!this->mRecordSize)
			return false;

		fseek(this->mFileHandle, this->mRecordSize, SEEK_CUR);

		DetermineNextMode();
	}
	else
		return false;

	return true;
}


/*
*	push a chunk header
*/
bool FileIO::PushHeader(char* header)
{
	//initialize other attributes
	mCurrentMode = BYTE_MODE;
	mRecordSize = 0;

	//check for errors
	if ((this->mFileHandle == NULL) || (!header))
		return false;

	//push single bytes
	for (unsigned char c=0; c<strlen(header); c++)
		PushByte(header[c]);

	return true;
}


/*
*	go to the next header
*/
bool FileIO::GotoHeader(char* header)
{
	//check for errors
	if ((this->mFileHandle == NULL) || (!header))
		return false;

	bool found = false;
	long headerSize = strlen(header);
	char* currentHeader = new char[headerSize];

	for(;;)
	{
		//go to a byte
		if (Mode() == RECORD_MODE)
		{
			//goto next unit
			if (!GotoNext())
				break;	//if there is no further unit
		}

		//reset current header
		memset(currentHeader, 0, headerSize);

		//there is a byte, find the correct bytes (header)
		for(;;)
		{
			//continue search if in BYTE_MODE and there is something to read
			if ((Mode() == RECORD_MODE) ||
				(EndOfFile()))
				break;

			//append one byte to the back of the current header
			for (int c=0; c<headerSize - 1; c++)
				currentHeader[c] = currentHeader[c+1];
			currentHeader[headerSize - 1] = PopByte();

			//continue search if the current header is not full or
			//	it's full but doesn't match the right header
			if (!currentHeader[0] || memcmp(currentHeader, header, headerSize))
				continue;
			else
			{
				found = true;
				break;
			}
		}

		//called break above -> check if found or finished by EOF
		if (found || EndOfFile())
			break;
	}

	//clean up memory
	if (currentHeader)
		delete currentHeader;

	//found header?
	return found;
}


/*
*	determine next mode
*/
bool FileIO::DetermineNextMode()
{
	if ((this->mFileHandle == NULL) ||
		EndOfFile())
		return false;

	int rval;
	rval = fread(&this->mCurrentMode, 1, 1, this->mFileHandle);

	//just use the last byte
	if ((this->mCurrentMode != BYTE_MODE) && (this->mCurrentMode != RECORD_MODE))
		this->mCurrentMode = BYTE_MODE;

	if (rval != 1)
		return false;

	return true;
}

