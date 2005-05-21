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

 /*!
 * \defgroup console In game console
 *
 * This classes represents our 'ingame' console. With the help of this console you
 * can write scripts to control gameplay or to debug the game during playing.
 * Remember on the beautiful console which was first used in first 3D-Ego-Shooter
 * like <i>Doom</i> or <i>Quake</i>. After the introduction of possibility of
 * using console in the game, this invitation was used in a lot of game projects.
 * We want to use this also in our projects, so here is small implementation of 
 * console which can ease be expanded for future using. 
 * \author Art Tevs <tevs@mpi-sb.mpg.de>
 */
 
#ifndef _NR_C_CONSOLE_BASE_H_
#define _NR_C_CONSOLE_BASE_H_

class nrCConsoleBase;

/*--------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include <string>
#include <vector>
#include <list>
#include <deque>

#include "nrIConsoleItem.h"
#include "nrCConsoleFunctionItem.h"
#include "nrCConsoleVariableItem.h"
#include "../../nrTypes.h"
#include <boost/shared_ptr.hpp>
#include "../../framework/nrIFrameWork.h"

using std::string;
using std::list;
using std::vector;
using std::deque;
using boost::shared_ptr;


// define item database type
typedef list< shared_ptr<nrIConsoleItem> > nrConsoleItem_DB;


/*--------------------------------------------------------------------------
 * Defines for using with the console
 *------------------------------------------------------------------------*/
#define NR_CONSOLE_FUNC(name)\
	int cnsl_ ## name (nrCConsoleBase* console, const std::vector<std::string>& args, void* param)


/**
 * nrCConsole - is a class representing our in game console. In game console is
 * useful for debugging the gameplay (game code) or to write scripts to execute
 * parts of game engine. Think on Quake like game console.
 * \ingroup console
 **/
class nrCConsoleBase {
public:

	// Constr & Destr
	nrCConsoleBase();
	virtual ~nrCConsoleBase();

	// console 
	nrResult	registerFunction(const string& name, nrConsoleFunctionPtr func);
	void		deregisterFunction(const string& name);

	
	// Intialize console with default functions
	virtual nrResult initConsole();
	virtual void	 deinitConsole();

	// adds an item to the console
	nrResult addItem(const shared_ptr<nrIConsoleItem> & item);

	// deletes an item from the console
	void deleteItem(const string & strName);

	// sets the command that is to be executed when no commandline match is found
	void setDefaultCommand(const shared_ptr<nrCConsoleFunctionItem> & def) {
				_defaultCommand = def;
	}
	
	
	
	// sets commandline buffer size
	void setCommandBufferSize(int size);
	
	// clears the console's commandline text buffer
	void clearCommandBuffer(){ _commandBuffer.clear(); }

	
	
	// sets text buffer size
	void setTextBufferSize(int size);
	
	// clears the console's text buffer
	void clearTextBuffer(){ _textBuffer.clear(); }

	
	// changes the commandline text to a previous one
	void historyGo(int where);

	
	// prints text on the console
	void print(const string & strText);
	void printf(const char* fmt, ...);
	
	// call if we detect any key activity
	void onKeyPress(nrKeyIndex index, nrIFramework* frameWork = NULL);
	void onKeyRelease(nrKeyIndex index, nrIFramework* frameWork = NULL);


	// some setter and getter functions for our console
	void setEchoCommand(bool s){_echoCommand = s;}
	bool getEchoCommand(){return _echoCommand;}
	
	
	const string& getCommandLine(){return *_commandLine;}
	
	void setLineSize(unsigned int lineWidth){_lineWidth = lineWidth;}
	unsigned int getLineSize(){return _lineWidth;}
	
	// get all available items
	const nrConsoleItem_DB & getItemDB(){return _itemList;}
	
	/**
	 * draw the console. Define it here as virtual because this class
	 * provides just a base console. Rendering should be done in derived classes.
	 **/
	virtual void render(){};
		
	// couple of console properties for global usement
	string consoleInputString;
	string consoleCursorString;
		
protected:
	
	// holds the text buffer
	deque<string> 	_textBuffer;
	unsigned int 	_textBufferSize;

	// holds the buffer for previously entered commands (command history)
	deque<string>	_commandBuffer;
	unsigned int 	_commandBufferSize;
	int				_lineBufferIndex;
	
	// holds the command line
	bool 			_echoCommand;
	unsigned int 	_lineWidth;
	string*  		_commandLine;	

	// Default command
	shared_ptr<nrCConsoleFunctionItem> _defaultCommand;

	// holds the list of items
	nrConsoleItem_DB _itemList;
			
private:
	int getEmptyIndex(){
		for(int i=0; i < (int)_commandBuffer.size(); i++)
			if (_commandBuffer[i].length()==0) return i;

		return 0;			
	}

	vector<string> splitLine(const string& line);

	// parses command line
	void parseCommandLine();

};

#endif
