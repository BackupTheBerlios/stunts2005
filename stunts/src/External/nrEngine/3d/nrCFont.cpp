
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "../nrEngine.h"


/**
 * Set the Size of the displayed font. Think on glScalef
 **/
void nrCFont::setSize(float x, float y){
	sizex=x;
	sizey=y;
}

/**
 * Build font from the bitmap file.
 * @param file - name of the file containing 16x16 Character ASCII-Table
 * @return always NR_OK
 **/
nrResult nrCFont::buildFont(const string& file){

	// kill the old font
	killFont();
		
	// load bitmap font texture
	font.reset( new nrCTexture () );
	font->loadFromFile(file, 
			NR_TEXTURE_2D, NR_RGBA | NR_LINEAR | NR_CLAMP_TO_EDGE, false, true);
		
	// get texture data
	const shared_array<byte>& data = font->getData();
	int size = font->getWidth();
	internal_scale = 1.0f / ((float)size / 256.0f);
	step = size / 16;
	shared_array<int> space_t; space_t.reset(new int[256]);
	space.reset(new int[256]);
	
	// read pixel data to get width of each character
	for(int y = 0, i = 0; y < 16; y++) {
		for(int x = 0; x < 16; x++, i++) {
			byte* ptr = NULL;
			
			space_t[i] = 0;
			for(int j = 0; j < step; j++) {
				ptr = data.get() + (size * y * step + x * step + j) * 4;
				int k;
				for(k = 0; k < step; k++) {
					if(*(ptr + 3) != 0) break;
					ptr += size * 4;
				}
				if(k != step) break;
				space_t[i]++;
			}
			
			space[i] = 0;
			if(space_t[i] == step) continue;
			for(int j = step - 1; j >= 0; j--) {
				ptr = data.get() + (size * y * step + x * step + j) * 4;
				int k;
				for(k = 0; k < step; k++) {
					if(*(ptr + 3) != 0) break;
					ptr += size * 4;
				}
				if(k != step) break;
				space[i]++;
			}
			space[i] = step - space_t[i] - space[i];
			
			if (space[i] < 1){
				space[i] = step;
			}
		}
	}
	
	// Generate List for each Character
	//base=glGenLists(256);
	for (int i=0; i < 256; i++){
		base[i] = glGenLists(1);
	}
	
	// create character lists
	for(int y = 0, i = 0; y < 16; y++) {
		for(int x = 0; x < 16 && i < 256; x++, i++) {
		
			// calculate texture coordinates
			float s = (float)x / 16.0f + (float)space_t[i] / (float)size;
			float t = (float)y / 16.0f;
			float ds = (float)space[i] / (float)size;
			float dt = 1.0 / 16.0;
			
			// create display list and render the character
			glNewList(base[i],GL_COMPILE);
			 glBegin(GL_QUADS);
			  glTexCoord2f(s,t);
			  glVertex2i(0,0);
			  
			  glTexCoord2f(s,t + dt);
			  glVertex2i(0,step);
			
			  glTexCoord2f(s + ds,t + dt);
			  glVertex2i(space[i], step);
			
			  glTexCoord2f(s + ds,t);
			  glVertex2i(space[i],0);
			 glEnd();
			glTranslatef((float)space[i],0,0);
			glEndList();	
		}
	}
	
	return NR_OK;
}


/**
 * Kill the font. Release used memory.
 **/
void nrCFont::killFont(){
//	glDeleteLists(base,256);
	for (int i=0; i < 256; i++)
		glDeleteLists(base[i],1);

	font.reset();
}


/**
 * Print formatted string as output on the display
 * @param x,y - position of the string (2D-Position on the screen)
 * @param fmt - formatted string to be printed
 **/
void nrCFont::printf(int x, int y, const char *fmt,...){

	// If no tesxt, so return	
	if (fmt == NULL) return;
	if (strlen(fmt) <= 0) return;
		
	// Local variables
	char    text[1024];                          
	int 	matrixMode,screenStats[4],blendSrc,blendDst;
	va_list ap;                                    // Pointer To List Of Arguments
		
	// get formatted input
	va_start(ap, fmt);                             // Parses The String For Variables
	vsprintf(text, fmt, ap);                       // And Converts Symbols To Actual Numbers
	va_end(ap);                                    // Results Are Stored In Text
	
	// Get current OpenGL states
	glGetIntegerv(GL_MATRIX_MODE, &matrixMode);
	glGetIntegerv(GL_VIEWPORT, screenStats);
	glGetIntegerv(GL_BLEND_SRC, &blendSrc);
	glGetIntegerv(GL_BLEND_DST, &blendDst);
	
	// change OpenGL-State
	nrPushOnTexture2D();
	nrPushOnScissorTest();
	nrPushOnBlend();
	nrPushOffLit();
	nrPushOffDepthRead();
	nrOrthoMode(0,0,screenStats[2], screenStats[3]);
	
	// Set the correct blending mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	
	// Activate Font-Texture
	font->bind();
	puts((float)x, (float)y, text);
	
	// Go to the old OpenGL state
	nrPerspectiveMode();
	nrPopDepthRead();
	nrPopLit();
	nrPopBlend();
	nrPopTexture2D();
	nrPopScissorTest();
	
	// set mode back
	glBlendFunc(blendSrc, blendDst);
	glMatrixMode(matrixMode);
	glBindTexture(GL_TEXTURE_2D, 0); 
	
	return;
}


/**
 * Just print the text on display.
 */
void nrCFont::puts(float x,float y,const char *str) {

	// Push current matrix
	glPushMatrix();
	glTranslatef(x,y,0);
	glScalef(sizex * internal_scale, sizey * internal_scale,1.0f);

	unsigned char *s = (unsigned char*)str;
	int lines = 0;
	
	// for each line, print line
	for(; *s != 0; s++) {
		
		// if we detect new line character, so create new line
		if(*s == '\n') {
			lines++;
			glLoadIdentity();
			glScalef(sizex * internal_scale, sizey * internal_scale,1.0f);
			glTranslatef(x, y + 16 * lines,0);
		
		// generate hack tab mode (move 2 spaces)
		} else if(*s == '\t') {
			glTranslatef(16.0f * 2.0f,0,0);
		
		// print of space character is just moving the print cursor
		} else if(*s == ' ') {
			glTranslatef((float)step / 2.0f,0,0);
		
		// each other character should be printed
		} else {
			glCallList(base[*s]);
		}
	}
	glPopMatrix();

}
