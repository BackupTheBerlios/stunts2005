/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __NR_C_FONT_H__
#define __NR_C_FONT_H__

class nrCFont;
class nrCTexture;
	
/**-------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include <string>


/**
 * Small font class. Can be used to display bitmap fonts.
 * Given font texture will be splitted into small textures representing one character.
 * After that there will be scan for transparency to find out the width of each character.
 * Then for each character an display list will be generated and character is rendered.
 * \ingroup 3d
 **/
class nrCFont{
private:

    // Datas
    shared_ptr<nrCTexture> font;
    int base[256];
	int step;
    float sizex,sizey, internal_scale;
 
	// Here we will save the space which each character need
	shared_array<int>	space;

	// draw string on the display
	void puts(float x, float y, const char *str);
    void killFont();

public:
  
    /**
	 * Instantiate the object and set default values
	 **/
    nrCFont() {sizex=1.0f; sizey=1.0f; internal_scale=1.0f; font.reset();};
    
	/**
	 * Instantiate object and create bitmap font from file. This file should contain
	 * whole ASCII-Table with each character. Characters should be stored in 16x16-Table
	 **/
	nrCFont(const string& file){buildFont(file);}
    virtual ~nrCFont() {killFont();};
  
    // Methoden
    nrResult buildFont(const string& file);
    void printf (int x, int y, const char* fmt, ...);
    void setSize(float x, float y);
	
	/**
	 * Returns zoom state of each character. <b>1.0</b> means there is no zooming.
	 **/
	float getSizeX(){return sizex;}
	
	/**
	 * @see getSizeX()
	 **/
	float getSizeY(){return sizey;}
	
	/**
	 * Returns the real average size of one character.
	 **/
	float getCharWidth(){return sizex * (float)step * internal_scale;}
	
	/**
	 * @see getCharWidth()
	 **/
	float getCharHeight(){return sizey * (float)step * internal_scale;}
	
};

#endif
