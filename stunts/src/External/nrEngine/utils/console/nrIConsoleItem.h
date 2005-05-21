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

#ifndef _NR_I_CONSOLE_ITEM_H_
#define _NR_I_CONSOLE_ITEM_H_

/**--------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include "../../nrDator.h"
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

using std::vector;
using std::string;
using boost::shared_ptr;

/**
  * Defines the type of any console function.
  * Each console function should return an int and got as parameter
  * vector containing parameters to this function, console pointer and any
  * predefined parameter as pointer to void.
  * \ingroup console
  **/
typedef int(*nrConsoleFunctionPtr)(nrCConsoleBase*, const vector<string>&, void*);


/**
 * nrIConsoleItem - is an item registered in our console. Item can be a function
 * or a variable of different types (float, uchar, string ...)
 * This is only an interface to be implemented by each item which can be registered
 * by the console.
 * @todo Expand the console item definitions to increase functionality for console items.
 *		 For example operators like =, <, >, !=, ==, ... for nrCConsoleVariableItem
 * \ingroup console
 **/
class nrIConsoleItem{	
public:

	// Constructor & Destructor	
	nrIConsoleItem(){};
	virtual ~nrIConsoleItem(){};

	/**
	 * This is a name of our console item object. It is better if the name stay unique.
	 **/
	std::string name;
	
	/**
	 * Check whenever item is handled like a function
	 * @return <b>true</b> if provided class represents a console function
	 **/
	virtual bool isFunction() = 0;
		
	
	/**
	 * Through this operator all derived classes, which represents a function,
	 * can be handled like a normal function call.
	 * I do not know exactly, why I decided to do this thing in this way :-)
	 * @param console - is our console from which this item was called
	 * @param args - arguments for the function given from user
	 * @return return value of function represented by this item
	 **/
	virtual int operator()(nrCConsoleBase* console, const vector<string>& args) = 0;
	
	/**
	 * Convert console item to a string. Function can show here there name and parameters.
	 * Variables can show here their name and value or something else. You are free to implement
	 * this operator in the way you want.
	 **/
	virtual operator string()  = 0;
	
};

#endif
