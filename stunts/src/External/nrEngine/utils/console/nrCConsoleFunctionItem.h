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

#ifndef _NR_I_CONSOLE_FUNCTION_ITEM_H_
#define _NR_I_CONSOLE_FUNCTION_ITEM_H_

/**--------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include "nrIConsoleItem.h"


/**
 * nrCConsoleFunctionItem - is a class which stands for our function in the console
 * This class is derived from nrIConsoleItem-Interface, so it can be used in 
 * nrConsole class. If you want to bind any function to this item, you have
 * to know that binded function should return an int and get as parameter
 * const vector<string>& and void*.
 * \ingroup console
 **/
class nrCConsoleFunctionItem : public nrIConsoleItem{
public:
	
	// check if this item is a function
	bool isFunction(){return true;}
	
	/**
	 * Constructor - initialize the instance
	 **/
	nrCConsoleFunctionItem(){_func = NULL; _parameter = NULL;}
	
	
	/**
	 * Constructor - initialize the instance
	 * @param function - pointer to the function which is represented by this object.
	 * @param param - this parameter will be passed to console functions as
	 * 				  addiditional parameter. This helps us to pass extra data
	 *				  from the creator of nrCConsoleFunction-Instance to any console
	 *				  function registered by them
	 **/
	nrCConsoleFunctionItem (const nrConsoleFunctionPtr function, void* param = NULL){
				_func = function;
				_parameter = param;
	}
	
	~nrCConsoleFunctionItem(){_func = NULL;}


	/**
	 * @see nrIConsoleItem::operator()()
	 **/
	int operator()(nrCConsoleBase* console, const std::vector<std::string>& args){
		try{
			return (*_func)(console, args, _parameter);
		}catch (...){
			return -1;	
		}
	}
	
	
	/**
	 * @see nrIConsoleItem::operator string()
	 **/
	operator string() {
		return "int " + name + "(args, ...);";
	}
	
	
protected:
	
	nrConsoleFunctionPtr	_func;
	void* _parameter;

		
};

#endif
