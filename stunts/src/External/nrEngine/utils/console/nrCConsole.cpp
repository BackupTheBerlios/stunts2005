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


/*--------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include "../../nrEngine.h"


/**
 * Constructor - set default values. (e.g. <b>rowCount</b>=20)
 **/
nrCConsole::nrCConsole(){
	nrCConsoleBase();
	font.reset(new nrCFont());
	setActive(false);
	rowCount = 20;
}

nrCConsole::~nrCConsole(){
	font.reset();
}


/**
 * Render the console to OpenGL's frame buffer
 **/
void nrCConsole::render(){

	// read screen size
	GLint view[4];
	glGetIntegerv(GL_VIEWPORT, view);
	GLint consoleX = (GLint)(float(view[2]));
	GLint consoleY = (GLint)(float(rowCount + 1) * font->getCharHeight());
	
	// change OpenGL-State
	nrPushOnBlend();
	nrPushOffLit();
	nrPushOffDepthRead();
	nrPushOffDepthWrite();
	nrOrthoMode(0,0,view[2],view[3]);
	nrPushOffCullFace();
	
	// draw background
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0,0,0,0.5f);
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2i(0,0);
		glVertex2i(consoleX,0);
		glVertex2i(0,consoleY);
		glVertex2i(consoleX,consoleY);
	glEnd();
	
	// restore old OpenGL-State	
	nrPopCullFace();
	nrPerspectiveMode();
	nrPopDepthWrite();
	nrPopDepthRead();
	nrPopLit();
	nrPopBlend();

	// calculate console output positions
	int step = (int)font->getCharHeight();
	int posY = consoleY - step;
	int posX = 10;

	// Write console input
	glColor3f(0,1,0);
	font->printf(posX, posY, "%s%s%s",
		consoleInputString.c_str(), 
		getCommandLine().c_str(), 
		consoleCursorString.c_str());
	
	// write text buffer
	deque<string>::const_iterator it;
	for (it = _textBuffer.begin(); posY > 0 && it != _textBuffer.end(); it++){
		posY -= step;
		font->printf(posX, posY, "%s", (*it).c_str());
	}
		
}


/**
 * Load image file containing the font for our console
 * @param name - name of the file to be loaded
 * @return return value from nrCFont::buildFont()
 **/
nrResult nrCConsole::loadFontFile(const string& name){
	return font->buildFont(name);
}


/**
 * Initalize the console 
 **/
nrResult nrCConsole::taskInit(){
	return initConsole();
}

/**
 * Do nothing at now 
 **/
nrResult nrCConsole::taskStart(){

	// OK
	return NR_OK;
}

/**
 * Updates the console task by getting user input and rendering the console.
 * Update is only done if console is active. @see setActive()
 * @return always NR_OK
 **/
nrResult nrCConsole::taskUpdate(){
	
	if (isActive){
		// check console input 
		for (int i=NRKEY_SPACE; i <= NRKEY_z; i++){
			if (nrFramework.keyPush((nrKeyIndex)i))
				onKeyPress((nrKeyIndex)i, &nrFramework);
		}
		
		// check special keys
		if (nrFramework.keyPush(NRKEY_RETURN)) onKeyPress(NRKEY_RETURN);
		if (nrFramework.keyPush(NRKEY_BACKSPACE)) onKeyPress(NRKEY_BACKSPACE);
		if (nrFramework.keyPush(NRKEY_UP)){historyGo(-1);}
		if (nrFramework.keyPush(NRKEY_DOWN)){historyGo(1);}
			
		
		// render the console 
		render();
	}

	// OK
	return NR_OK;
}

/**
 * Stop the console task.
 * @return always NR_OK 
 **/
nrResult nrCConsole::taskStop(){

	// OK
	return NR_OK;
}
