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

 
#include "nrEngine.h"

/**
 * This function will convert the given nrResult number to the
 * string describing it. e.g: NR_OK => "NR_OK"
 *
 * @param res - error code to be converted in string
 * \ingroup engine
 **/
string	nrErrorGetShort(nrResult res){

	char str[256];
	sprintf(str, "%x", res);

#define _ERROR(p) case p: sprintf(str, #p);break;	
	
	switch (res){
	
_ERROR(NR_OK)
_ERROR(NR_UNKNOWN_ERROR)
_ERROR(NR_BAD_PARAMETERS)
_ERROR(NR_OUT_OF_MEMORY)

_ERROR(NR_FILE_ERROR)
_ERROR(NR_FILE_NOT_FOUND)
_ERROR(NR_FILE_ERROR_IN_LINE)

_ERROR(NR_PROFILE_ERROR)
_ERROR(NR_PROFILE_NOT_FOUND)
_ERROR(NR_PROFILE_NOT_LOADED)
_ERROR(NR_PROFILE_NOT_EXISTS)
_ERROR(NR_PROFILE_ALREADY_EXISTS)

_ERROR(NR_EXTENSION_ERROR)
_ERROR(NR_EXTENSION_NOT_SUPPORTED)
  
_ERROR(NR_VFS_ERROR)
_ERROR(NR_VFS_ALREADY_OPEN)
_ERROR(NR_VFS_CANNOT_OPEN)
_ERROR(NR_VFS_CANNOT_CLOSE)
_ERROR(NR_VFS_IS_NOT_OPEN)
_ERROR(NR_VFS_FILE_NOT_FOUND)
_ERROR(NR_VFS_FILE_NOT_OPEN)
_ERROR(NR_VFS_FILE_END_REACHED)
_ERROR(NR_VFS_FILE_READ_ERROR)
_ERROR(NR_VFS_LINE_READ_ERROR)
_ERROR(NR_VFS_SCAN_READ_ERROR)
_ERROR(NR_VFS_SEEK_ERROR)

_ERROR(NR_APP_ERROR)

_ERROR(NR_LOG_ERROR)

_ERROR(NR_SETTINGS_ERROR)
_ERROR(NR_SETTINGS_VAR_ALREADY_REGISTERED)
_ERROR(NR_SETTINGS_VAR_NOT_REGISTERED)

_ERROR(NR_KERNEL_ERROR)
_ERROR(NR_KERNEL_NO_TASK_FOUND)

_ERROR(NR_TASK_ERROR)
_ERROR(NR_TASK_NOT_READY)

_ERROR(NR_CONSOLE_ERROR)
_ERROR(NR_CONSOLE_ITEM_ALREADY_EXISTS)


_ERROR(NR_TEX_ERROR)
_ERROR(NR_TEX_INVALID_FORMAT)

_ERROR(NR_CLOCK_ERROR)
_ERROR(NR_CLOCK_OBSERVER_NOT_FOUND)
_ERROR(NR_CLOCK_OBSERVER_ALREADY_ADDED)

_ERROR(NR_FW_ERROR)
_ERROR(NR_FW_CANNOT_INITIALIZE)
_ERROR(NR_FW_FAILED_TO_RESIZE)
_ERROR(NR_FW_INVALID_RC_ID)
_ERROR(NR_FW_ALREADY_INIT)
_ERROR(NR_FW_NOT_INITIALIZED)

_ERROR(NR_RC_ERROR)
_ERROR(NR_RC_CANNOT_SETUP_PIXEL_FORMAT)


_ERROR(NR_ENGINE_ERROR)
_ERROR(NR_ENGINE_YOU_MUST_CREATE_ENGINE_BEFORE)

		default:
			sprintf(str,"%x", res);
	}
	
#undef _ERROR
	return str;
}


/**
 * nrEngine's own generic assert function. Here we will create an formatted
 * output on the console and in log files.
 * Also the engine will be deleted before exit called.
 *
 * @param szExp Error message
 * @param szFilename Filename of the file where error happens
 * @param iLineNum Line number where assert fails
 * \ingroup engine
 **/
void nrAssertGeneric(const char *szExp, const char *szFilename, int iLineNum){

	// Give message on the console
	fprintf(stderr,"================================================\n");
	fprintf(stderr,"ASSERTION FAILED!\n");
	fprintf(stderr,"Expr: (%s)\n", szExp);
	fprintf(stderr,"File: %s\n", szFilename);
	fprintf(stderr,"Line: %d\n", iLineNum);
	fprintf(stderr,"================================================\n");

	// give message to log engine
	if (nrCLog::isValid()){
		nrLog.Log(NR_LOG_ENGINE,"================================================");
		nrLog.Log(NR_LOG_ENGINE,"ASSERTION FAILED!");
		nrLog.Log(NR_LOG_ENGINE,"Expr: (%s)", szExp);
		nrLog.Log(NR_LOG_ENGINE,"File: %s", szFilename);
		nrLog.Log(NR_LOG_ENGINE,"Line: %d", iLineNum);
		nrLog.Log(NR_LOG_ENGINE,"================================================");
	}

	// Try now to delete the engine
	nrKernel.KillAllTasks();
	nrEngineDelete();

	// Now exit the application
	exit(1);
}


/**
 * This function will initialize the engine. You should call this befaore you
 * want to use the engine, otherwise error occurs.
 * What will be initialized here (only singletons are created):
 *		- Logging Engine
 *		- Kernel
 *		- Virtual Filesystem
 *		- Settings manager
 *		- Clock
 * You have then to initialize each element by hand to be sure that this
 * modules are created.
 * \ingroup engine
 **/
void	nrEngineInit(){
	
	// Create some essential singletons
	nrCLog::Instantiate();
	nrCKernel::Instantiate();
	nrCFileSystem::Instantiate();
	nrCSettings::Instantiate();
	nrCPBuffer::Instantiate();
	nrCClock::Instantiate();
	
}


/**
 * Returns true if the engine was initialized before. Or false otherwise.
 * Because our engine's core contains of a lot of submodules, so we
 * assume that engine is active if each of the module is active.
 * @note Be not confused by the name submodules. Our core of the engine contains
 * of Clock, SettingsManager, Log, Kernel, FileSystem.
 * \ingroup engine
 **/
bool nrEngineIsActive(){
	return (nrCLog::isValid() &&
		nrCKernel::isValid() &&
		nrCFileSystem::isValid() &&
		nrCClock::isValid() &&
		nrCSettings::isValid());
}


/**
 * Delete all singleton objects and release used memory
 * \ingroup engine
 **/
void	nrEngineDelete(){
	
	// delete singletons
	nrCClock::Release();
	nrCPBuffer::Release();
	nrCSettings::Release();
	nrCFileSystem::Release();
	nrCKernel::Release();
	nrCLog::Release();
	
}



/**
 * Initialize default console.
 * You can also define your own console types by deriving basic console class.
 * If you wan't do this, then you can use default console
 * \ingroup console
 **/
nrResult nrConsoleInit(){
	
	// initialize console singleton 
	nrCConsole::Instantiate();
	
	// OK
	return NR_OK;
}

/**
 * Release the default console
 * \ingroup console
 **/
nrResult nrConsoleDelete(){

	// release the console singleton 
	nrCConsole::Release();

	// OK
	return NR_OK;	
}

