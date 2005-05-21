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

#ifndef _NR_CONSOLE_DEFAULT_FUNCTIONS_H_
#define _NR_CONSOLE_DEFAULT_FUNCTIONS_H_

/**--------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include "nrCConsoleBase.h"


/**---------------------------------------------------------------------------
 * Here you will find some default console functions. This functions will be
 * added to our console to implement basic functionality.
 * Write other functions to implement libraries for our console :-)
 *--------------------------------------------------------------------------*/
int cnsl_echo(nrCConsoleBase* console, const vector<string>& args, void*);
int cnsl_not_detected(nrCConsoleBase* console, const vector<string>& args, void*);
int cnsl_show_commands(nrCConsoleBase* console, const vector<string>& args, void*); 
 


#endif
