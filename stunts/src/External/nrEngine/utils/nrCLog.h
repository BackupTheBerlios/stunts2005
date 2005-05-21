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


#ifndef _NR_C_LOG_H_
#define _NR_C_LOG_H_


/**-----------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------------*/
#include "../nrTypes.h"
#include "../nrISingleton.h"
#include "../nrEngine.h"

#include <vector>
#include <string>
#include <fstream>
#include <stdarg.h>
#include <map>

using std::vector;
using std::string;
using std::ofstream;
using std::ifstream;
using std::map;

/**-----------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/
#define NR_LOG_TARGET	int
#define NR_LOG_MSG_ID	unsigned long

#define NR_LOG_CLIENT	1 << 0
#define NR_LOG_SERVER	1 << 1
#define NR_LOG_APP		1 << 2
#define NR_LOG_KERNEL	1 << 3
#define NR_LOG_ENGINE	1 << 4
#define NR_LOG_CONSOLE	1 << 5


/**
 * nrCLog Class is using to generate and print logging information on console,
 * MessageBox(WIN32) or in a log file on the disk. This class is also derived 
 * by nrISingleton class to guarantee that we have only one instance of nrCLog
 * in the memory. You can generate log messages either by giving the message 
 * directly or by using ID-Number of needed line string. This guarantee to
 * be log messages not hardcoded and they can also be localized (language). All used
 * log messages should be stored in an extra file to allow logging by Msg-Id-Number.
 **/
class nrCLog : public nrISingleton<nrCLog> {
private:
	
	// This are our logging targets
	ofstream _appLog;
	ofstream _engineLog;
	ofstream _clientLog;
	ofstream _serverLog;
	ofstream _kernelLog;

	// This vector contains all possible error messages (localized)
	vector<string> logStrings;
	string msgFile;
	string logPath;

	// map which stores the logging echo targets
	map <NR_LOG_TARGET, NR_LOG_TARGET> _echoMap;

	// Loading of messages
	bool LoadStrings();
	void LogIt(NR_LOG_TARGET target, const char *msg);

public:
	
	nrCLog();
	~nrCLog();
	
	// Init operation
	nrResult Init(const string& logPath, const string& messagesFile);

	// want to echo each log to the console 
	void echoLogging(NR_LOG_TARGET from, NR_LOG_TARGET to){
		_echoMap[from] = to;	
	}

	// Logging operations
	void Log(NR_LOG_TARGET target, const char *msg, ...);
	void Log(NR_LOG_TARGET target, NR_LOG_MSG_ID msgID, ...);
	
	void appLog(const char* msg, ...);
	void appLog(NR_LOG_MSG_ID msgID, ...);
	
};


#endif	//_NRCLOG_H_
