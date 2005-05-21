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

#ifndef _NR_C_CONSOLE_H_
#define _NR_C_CONSOLE_H_


/*--------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include "nrCConsoleBase.h"
#include "../../3d/nrCFont.h"

/**
 * nrCConsole - is a class representing our in game console. In game console is
 * useful for debugging the gameplay (game code) or to write scripts to execute
 * parts of game engine. Think on Quake like game console.
 * This calss was derived from nrCConsoleBase and implements simple OpenGL render
 * console. It provides also nrITask - Interface. On each update cycle the content
 * of the text buffer will be rendered to the front buffer. 
 * \ingroup console
 **/
class nrCConsole: public nrCConsoleBase, public nrISingletonTask<nrCConsole> {
private:
	shared_ptr<nrCFont>	font;

	bool isActive;

	unsigned int rowCount;

public:
	
	nrCConsole();
	virtual ~nrCConsole();
	
	// load functions
	nrResult loadFontFile(const string& fileName);
	
	/**
	 * The default built in font can be overloaded through this function
	 * @param f - smart pointer to nrCFont-Object representing new font
	 **/
	void bindFont(const shared_ptr<nrCFont> f){font = f;}

	// render function
	void render();

	/**
	 * Set if if this console is active or not
	 **/
	void setActive(bool onoff){ isActive = onoff; }
	bool getActive(){return isActive;}
	
	/**
	 * Set the font size on the console 
	 * @param size - should be greater than 0 (16 is default)
	 **/
	void setFontSize(int size){font->setSize(float(size)/16.0f, float(size)/16.0f);}
	
	/**
	 * Set visible rows count (each row is a line in the text buffer)
	 **/
	void setRowCount(unsigned int c){rowCount = c;}
	
	// Task-Functions
	nrResult taskInit();
	nrResult taskStart();
	nrResult taskUpdate();
	nrResult taskStop();
	
	const char* taskGetName(){return "nrCConsole";}
	
};

#endif
