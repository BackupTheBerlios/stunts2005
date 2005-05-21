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

/**--------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include "nr_console_default_functions.h"

 
/**--------------------------------------------------------------------------
 * echo(...) - shows the same input on console as arguments
 *--------------------------------------------------------------------------*/
int cnsl_echo(nrCConsoleBase* console, const vector<string>& args, void* p){
	
	// if console is valid
	if (console == NULL) return -1;
	
	// print text on console
	try{
		for (unsigned int i=1; i < args.size(); i++){
			console->printf("%s ",args[i].c_str());
		}
		console->print("\n");
		
	}catch (...){
		return -2;
	}
	
	// OK
	return 0;
	
}
 

/**--------------------------------------------------------------------------
 * notDetected(...) - default function, if console command line was not
 * detected as valid function or variable
 *--------------------------------------------------------------------------*/
int cnsl_not_detected(nrCConsoleBase* console, const vector<string>& args, void* p){
	
	// if console is valid
	if (console == NULL) return -1;
	
	// print text on console
	try{
		console->printf("%s: command not found\n", args[0].c_str());	
	}catch (...){
		return -2;
	}
	
	// OK
	return 0;

}


/**--------------------------------------------------------------------------
 * show_commands(...) - show all supported commands
 *--------------------------------------------------------------------------*/
int cnsl_show_commands(nrCConsoleBase* console, const vector<string>& args, void* p){
	
	// if console is valid
	if (console == NULL) return -1;
	
	// print text on console
	try{
		nrConsoleItem_DB::const_iterator it;
		nrConsoleItem_DB itemDB = console->getItemDB();
		
		console->print("Available commands:\n");
		
		// access each method and check if it is a supported function
		for (it = itemDB.begin(); it != itemDB.end(); it++){
			if ((*it)->isFunction())
				console->printf("%s  ", (*it)->name.c_str());				
		}
		console->print("\n");
		
	}catch (...){
		return -2;
	}
	
	// OK
	return 0;

}
