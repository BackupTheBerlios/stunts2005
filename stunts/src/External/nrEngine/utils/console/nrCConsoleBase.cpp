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


/**-------------------------------------------------------------------------
 * Includes
 *-------------------------------------------------------------------------*/
#include "../../nrEngine.h"
#include <stdarg.h>
#include "nr_console_default_functions.h"



/**-------------------------------------------------------------------------
 * getCharacter - is a helping function which helps us to get an character 
 * from a keyboard input index and from shift or alt keys
 *-------------------------------------------------------------------------*/
char getCharacter(nrKeyIndex index, nrIFramework* frameWork){

	// return character 
	char c = ' ';
	
	// if we have characters
	if (index >= NRKEY_a && index <= NRKEY_z){
		c = (char)index;
		
		if (frameWork->keyDown(NRKEY_LSHIFT) || frameWork->keyDown(NRKEY_RSHIFT)){
			c = c - (char)32;	
		}
	
	}else if (index == NRKEY_SPACE){
		c = (char)' ';
		
	}else if (index >= NRKEY_1 && index <= NRKEY_9){
		c = (char)index;
		if (frameWork->keyDown(NRKEY_LSHIFT) || frameWork->keyDown(NRKEY_RSHIFT)){
			c = c - (char)16;	
		}
		
	}else if (index == NRKEY_0){
		c = (char)'0';
		if (frameWork->keyDown(NRKEY_LSHIFT) || frameWork->keyDown(NRKEY_RSHIFT)){
			c = (char)'=';	
		}		
		
	}else if (index == NRKEY_MINUS){
		c = (char)'-';
		if (frameWork->keyDown(NRKEY_LSHIFT) || frameWork->keyDown(NRKEY_RSHIFT)){
			c = (char)'_';	
		}		
		 
	}else if (index >= NRKEY_SPACE && index <= NRKEY_z){
		c = (char)index;	
	}
	
	return c;
}



/**-------------------------------------------------------------------------
 * splitLine - splits the line into more lines according to line size 
 *-------------------------------------------------------------------------*/
vector<string> nrCConsoleBase::splitLine(const string& line){

	// local variables
	vector<string> sub;
	int index = 0;
	
	sub.push_back("");
	
	// scan the string for new line characters
	// split also the line, if line is longer as our allowed line size  
	for (unsigned int i=0; i < line.length(); i++){
		if (line[i] != char('\n')){
			(sub[index]) += line[i];
		}else{
			i++;
			sub.push_back("");
			index ++;
		}
		if (sub[index].length() > _lineWidth){
			sub.push_back("");
			index ++;
		}
	}
	
	//if (sub[index].length() == 0)
	//	sub.pop_back();
	
	// return splitted line
	return sub;
}

/**
 * Constructor - set default values to the console size and fill buffers with default values
 **/
nrCConsoleBase::nrCConsoleBase(){
	_commandBufferSize = 50;
	_textBufferSize = 40;

	consoleInputString = ">: ";
	consoleCursorString = "_";
	
	_lineBufferIndex = 0;
	_commandBuffer.push_front("");
	_textBuffer.push_front("");

	_commandLine = &(_commandBuffer[0]);

	setLineSize(60);
	
}
nrCConsoleBase::~nrCConsoleBase(){
}


/**
 * Deinitialize console. Release all console items. 
 **/
void nrCConsoleBase::deinitConsole(){

	nrConsoleItem_DB::iterator it;
	
	// find item
	if (_itemList.size() >= 0){
		for (it = _itemList.begin(); it != _itemList.end(); it++){
			nrLog.Log(NR_LOG_ENGINE, "nrConsole: delete item %s", (*it)->name.c_str());
			_itemList.erase(it);
		}
	}
	_itemList.clear();
	
	_defaultCommand.reset();
	
}


/**
 * Initialize console with default functions. (like: echo, showcmds, ...) 
 **/
nrResult nrCConsoleBase::initConsole(){

	nrLog.Log(NR_LOG_ENGINE, "nrConsole: initialize...");
	
	// set default console function
	nrConsoleFunctionPtr func = cnsl_not_detected;	
	shared_ptr<nrCConsoleFunctionItem> fitem(
		new nrCConsoleFunctionItem(func));
	fitem->name = "_default";
	
	setDefaultCommand(fitem);
	
	// local variables
	shared_ptr<nrIConsoleItem> item;
	
	// add "echo" command
	func = cnsl_echo;
	item.reset(new nrCConsoleFunctionItem(func));
	item->name = "echo";
	addItem(item);
	
	// add "showcmds" command
	func = cnsl_show_commands;
	item.reset(new nrCConsoleFunctionItem(func));
	item->name = "showcmds";
	addItem(item);
	
	
	// OK
	return NR_OK;
}


/**
 * Print new text on the console. This is done by adding the string to the text buffer
 * @param strTxt - text to be printed
 **/
void nrCConsoleBase::print(const string & strTxt){
	
	// add the rest to the first line and split to line size
	vector<string> sub = splitLine(_textBuffer[0] + strTxt);
	if (sub.size() == 1){
		(_textBuffer[0]) = sub[0];					 
	}else if (sub.size() > 1){
		(_textBuffer[0]) = sub[0];					
		for (unsigned int i=1; i < sub.size(); i++)	
			_textBuffer.push_front(sub[i]);		
	}
		
	// if we are out of bounds
	if(_textBuffer.size() > _textBufferSize)
		_textBuffer.pop_back();
	
}

/**
 * Print new text on the console. This is done by adding the formatted string to the text buffer
 * @param fmt - formatted string to be printed
 * @param ...
 **/
void nrCConsoleBase::printf(const char* fmt, ...){
	
	// read data out
	char buffer[2048];
	va_list ap;
	va_start(ap,fmt);
	vsprintf(buffer, fmt, ap);
	va_end(ap);
	
	string strTxt (buffer);
	
	// push it
	print(strTxt);

	
}


/**
 * Add new item to the console. Add new item is similar as register
 * new function or new variable by the console. So if you want to use new
 * functions in the console you have to add them here
 * @param item - is a new console item object.
 * @return either NR_OK or:
 *		- NR_CONSOLE_ITEM_ALREADY_EXISTS if such an item already exists
 **/
nrResult nrCConsoleBase::addItem(const shared_ptr<nrIConsoleItem> & item){
	
	// check if such an already exists
	nrConsoleItem_DB::iterator it;
	for (it = _itemList.begin(); it != _itemList.end(); it++){
		if ((*it)->name == item->name){
			return NR_CONSOLE_ITEM_ALREADY_EXISTS;
		}
	}
	
	nrLog.Log(NR_LOG_ENGINE, "nrConsole: add item %s", item->name.c_str());

	// add item
	_itemList.push_back(item);
	
	// OK
	return NR_OK;
}



/**
 * Delete an item from console
 * @param strName - name of the item to be deleted
 **/
void nrCConsoleBase::deleteItem(const string & strName){
	
	// iterator
	nrConsoleItem_DB::iterator it;
	
	// find item
	for (it = _itemList.begin(); it != _itemList.end(); it++){
		if ((*it)->name == strName){
			_itemList.erase(it);
			break;
		}
	}
	
	nrLog.Log(NR_LOG_ENGINE, "nrConsole: delete item %s", strName.c_str());

}



/**
 * Set the size of text buffer (count of rows that text buffer contains)
 * @param size - new size of the buffer
 **/
void nrCConsoleBase::setTextBufferSize(int size){
	
	_textBufferSize = size;

	// remove any extra line
	_textBuffer.resize(size);
	
}


/**
 * Set the size of command buffer (count of command history entries)
 * @param size - new size of the buffer
 **/
void nrCConsoleBase::setCommandBufferSize(int size){
	
	_commandBufferSize = size;

	// resets the commadn history (it is a queue)
	_commandBuffer.resize(size);
	
}


/**
 * historyGo - Changes actual command line into some past command. Set parameter to -1 to go back
 * through the command history buffer (think on terminal in Unix-Systems)
 * @param whereInPast - relative index of old command (in history) <= 0
 **/
void nrCConsoleBase::historyGo(int whereInPast){
	
	// vars
	_lineBufferIndex += -1 * whereInPast;
	
	if (_lineBufferIndex < 0){
		_lineBufferIndex = 0;
	
	}else if (_lineBufferIndex >= (int)_commandBufferSize){
		_lineBufferIndex = _commandBufferSize - 1;
	
	}else if (_lineBufferIndex >= (int)_commandBuffer.size()){
		_lineBufferIndex = _commandBuffer.size() - 1;
	}
	

	// restore the old command line or get one from history
	_commandLine = &(_commandBuffer[_lineBufferIndex]);	
		
}


/**
 * OnKeyPress - should be called if any keyboard activity is detected.
 * This function will pass character according to this key into commandline
 * buffer and will start parsing the command if return was pressed
 * @param index is an index of key
 * @param frameWork Framework you are using (to get user inputs)
 **/
void nrCConsoleBase::onKeyPress(nrKeyIndex index, nrIFramework* frameWork){

	// check whenever we have some not character keys
	switch (index){
		case NRKEY_BACKSPACE:
			if (_commandLine->length() > 0)
				_commandLine->resize(_commandLine->length()-1);
			return;
			break;
			
		case NRKEY_RETURN:
			if (_commandLine->length() > 0){
				parseCommandLine();
			}else
				printf("%s\n", consoleInputString.c_str());
			
			_lineBufferIndex = 0;
			_commandLine = &(_commandBuffer[0]);
			*_commandLine = "";
			return;
			break;
			
		default:
			break;
	}
	
	// check if input is allowed
	char c = getCharacter(index, frameWork);
	*_commandLine += c;
}


/**
 * Parses the current command line.
 * You can specify parameters to functions registered on console. This parameters
 * should be separated by epmty character (space). Each function will return an
 * int value wich will be displayed if it is unequal to 0.
 **/
void nrCConsoleBase::parseCommandLine(){

	string::size_type index = 0;
	vector<string> arguments;
	nrConsoleItem_DB::const_iterator it;
	int result = 0;
	bool commandFound = false;
	
	// add to text buffer
	if(getEchoCommand()){
		printf("%s\n", (consoleInputString +  *_commandLine).c_str());
	}
	
	// tokenize
	while(index <  _commandLine->length() && index != string::npos){
	
		// find SPACE character 
		string::size_type next_space = _commandLine->find((char)' ', index);
		
		// check whenever the substring is not an empty string
		if (index != next_space &&  _commandLine->substr(index, next_space - index) != " "){
			// get sub string
			arguments.push_back(_commandLine->substr(index, next_space - index));
		}
		
		// move current index to next space position
		if (next_space != string::npos) index = next_space + 1;
		else break;
	}

	// if we didn't find any command, so give empty line out 
	if (arguments.size() == 0){
		printf("%s\n", consoleInputString.c_str());
	}else{
	
		// now go through each console item and check what to do with this
		for (it = _itemList.begin(); it != _itemList.end() && !commandFound; it++){
			
			// if first argument of our command line is the same as the name 
			// of our item.
			if (arguments[0] == (*it)->name){
				
				// if this item is a function, so do function call
				if ((*it)->isFunction()){

					// call function with parameter
					result = (*(*it))(this, arguments);
					
					// check return result
					if (result != 0){
						printf ("\tCommand returns an error:%d\n", result);
						result = 0;
					}	
				}else{
					print(string((*(*it))));
				}
				commandFound = true;
			}
			
		}
		
		// if we didn't find the command, so do default command
		if (!commandFound){
			(*_defaultCommand)(this,arguments);
		}
	
	}


	// add to commandline buffer
	string temp = *_commandLine;
	if (_lineBufferIndex == 0){
		_commandBuffer.push_front("");
	}else{
		_commandBuffer[0] = temp;
		_commandBuffer.push_front("");		
	}
	if(_commandBuffer.size() > _commandBufferSize)
		_commandBuffer.resize(_commandBufferSize);
	
}



/**
 * Register new function by the console
 * After registration, this function can be used in the console
 * @param name - which name should this function have in our bash :-)
 * @param func - function pointer to the function
 * @return result value from addItem() - function
 **/
nrResult nrCConsoleBase::registerFunction(const string& name, const nrConsoleFunctionPtr func){	

	// create an console item
	shared_ptr<nrIConsoleItem> item;
			
	// set created item to a function
	item.reset(new nrCConsoleFunctionItem(func));
			
	// set function name
	item->name = name;

	// add to the console		
	return addItem(item);
}


/**
 * Remove the registered function from the console
 * @param name - which name should this function have in our bash :-)
 **/
void nrCConsoleBase::deregisterFunction(const string& name){	
						
	// add to the console		
	deleteItem(name);
}
