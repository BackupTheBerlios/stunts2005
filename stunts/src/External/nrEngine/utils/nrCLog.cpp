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


#include "nrCLog.h"
#include <iostream>
#include <time.h>

using std::ios;

nrCLog::nrCLog(){	
}


/**
 * Init - Load all messages and prepare logging
 * @param logPath - Path to directory where log files will be created
 * @param messagesFile - file which contains log messages (can be localized)
 * @return either NR_OK or:
 *		- NR_LOG_ERROR - if messagesFile was not found
 **/
nrResult nrCLog::Init(const string& logPath, const string& messagesFile){
	
	this->logPath = logPath;
	
	// open log files
	_appLog.open((logPath + "/applog.txt").c_str(), ios::out | ios::app);
	_kernelLog.open((logPath + "/krnllog.txt").c_str(), ios::out | ios::app);
	_clientLog.open((logPath + "/clntlog.txt").c_str(), ios::out | ios::app);
	_serverLog.open((logPath + "/srvrlog.txt").c_str(), ios::out | ios::app);
	_engineLog.open((logPath + "/engilog.txt").c_str(), ios::out | ios::app);
		
	Log (NR_LOG_KERNEL, "\n\nLogging activated");
	Log (NR_LOG_APP, "\n\nLogging activated");
	Log (NR_LOG_CLIENT, "\n\nLogging activated");
	Log (NR_LOG_SERVER, "\n\nLogging activated");
	Log (NR_LOG_ENGINE, "\n\nLogging activated");
	
	
	// load the strings file
	if (messagesFile.length() > 0){
		msgFile = messagesFile;
		if (!LoadStrings()){
			Log(NR_LOG_KERNEL, "File %s was not found !!!", msgFile.c_str());
			return NR_LOG_ERROR;		
		}
	}
	
	return NR_OK;
}

/**
 * Free memory and close log files
 **/
nrCLog::~nrCLog(){
	
	// clear messages 
	logStrings.clear();
	
	// close streams
	_kernelLog.close();
	_engineLog.close();
	_serverLog.close();
	_clientLog.close();
	_appLog.close();
	
}

/**
 * LoadStrings() - load the content of strings messages file
 **/
bool nrCLog::LoadStrings(){
	
	// open file with messages
	ifstream in(msgFile.c_str());
	if(!in.is_open())return false;

	// read lines until there is nothing to read
	while(!in.eof()){
		char szBuf[1024];
		in.getline(szBuf,1024);
		logStrings.push_back(szBuf);
	}

	// OK
	return true;
}


/**
 * Log() - logs the given message to logging target
 * @param target defines the target to which to log
 * @param msg formatted message to be logged
 **/
void nrCLog::Log(NR_LOG_TARGET target, const char *msg, ...){

	// get messages 
	va_list args; 
	va_start(args,msg);
	 char szBuf[2056];
	 memset(szBuf, 0, sizeof(char)*2056);
	 vsprintf(szBuf,msg,args);
	va_end(args);
	
	
	LogIt(target, szBuf);
	
	// echo logging
	if (_echoMap[target]){
		LogIt(_echoMap[target], szBuf);	
	}
	
}
void nrCLog::LogIt(NR_LOG_TARGET target, const char *msg){

	// get messages 
	const char *szBuf = msg;
	
	// Get time & date string
	time_t _time = time(NULL);
	char* timeDate = ctime(&_time);
	timeDate[strlen(timeDate) - 1] = 0;
	
	// Check whereever to log
	if(target & NR_LOG_APP){
		_appLog << timeDate << ": " << szBuf << "\n";
		_appLog.flush();
	}
	
	if(target & NR_LOG_CLIENT){
		_clientLog << timeDate << ": " << szBuf << "\n";
		_clientLog.flush();
	}
	
	if(target & NR_LOG_SERVER){
		_serverLog << timeDate << ": " << szBuf << "\n";
		_serverLog.flush();
	}
	
	if(target & NR_LOG_KERNEL){
		_kernelLog << timeDate << ": " << szBuf << "\n";
		_kernelLog.flush();
	}
	
	if(target & NR_LOG_ENGINE){
		_engineLog << timeDate << ": " << szBuf << "\n";
		_engineLog.flush();
	}
	
	if (target & NR_LOG_CONSOLE){
		if (nrCConsole::isValid()) nrConsole.printf("%s\n", szBuf);	
	}
		
}


/**
 * Log() - logs the given message to logging target
 * @param target defines the target to which to log
 * @param msgID message ID from msgFile to be logged
 **/
void nrCLog::Log(NR_LOG_TARGET target, NR_LOG_MSG_ID msgID, ...){

	// if we do not have any messages, so return back
	if (logStrings.size() == 0) return;

	// get message
	va_list args; 
	va_start(args, msgID);
	 char szBuf[2056];
	 vsprintf(szBuf, logStrings[msgID].c_str(), args);
	va_end(args);
	
	LogIt(target,szBuf);

	// echo logging
	if (_echoMap[target]){
		LogIt(_echoMap[target], szBuf);	
	}
	
}


/**
 * appLog - loggs the message direct to NR_LOG_APP - target
 **/
void nrCLog::appLog(const char* msg, ...){
	// get message
	va_list args; 
	va_start(args, msg);
	 char szBuf[2056];
	 vsprintf(szBuf, msg, args);
	va_end(args);	
	LogIt(NR_LOG_APP, szBuf);
	
	// echo logging
	if (_echoMap[NR_LOG_APP]){
		LogIt(_echoMap[NR_LOG_APP], szBuf);	
	}

}
