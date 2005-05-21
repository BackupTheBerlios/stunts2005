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

#ifndef _NR_I_CONSOLE_VARIABLE_ITEM_H_
#define _NR_I_CONSOLE_VARIABLE_ITEM_H_

/**--------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include "nrIConsoleItem.h"


/**
 * nrCConsoleVariableItem - is a class which stands for any variable in the console.
 * It can store any type of variable you want to use in console. 
 * This object must be built up by a nrDator - Interface (that means you have
 * to declare any dator and than you can build object of this class.
 * Through the dator this item has access to variable stored in the dator.
 * So you have automaticly an access from console to the variable stored in the dator.
 * \ingroup console
 **/
class nrCConsoleVariableItem : public nrIConsoleItem{
public:
	
	/**
	 * Constructor - instantiate an object of this class
	 * @param var - smart pointer to dator representing the variable
	 **/
	nrCConsoleVariableItem (const shared_ptr<nrIDator> var){_var = var;}
	~nrCConsoleVariableItem(){}

	/**
	 * @see nrIConsoleItem::isFunction()
	 * @return here always <b>false</b>
	 **/
	bool isFunction(){return false;}
	
	
	/**
	 * Do nothing here. Variable is not a function.
	 **/
	int operator()(const std::vector<std::string>&){return 0;}
	
	
	/**
	 * Return the name and current value of the variable represented by this item
	 */
	operator string(){return name + "=" + string(*_var);}

protected:
	
	shared_ptr<nrIDator>	_var;
	
};

#endif
