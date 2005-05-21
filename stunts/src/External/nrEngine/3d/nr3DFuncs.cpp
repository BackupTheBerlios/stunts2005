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

#include "../nrEngine.h"

float	projection[16];
float	modelview[16];

/**
 * This function changes our projection mode from 3D to 2D
 */
void	nrOrthoMode(int left, int top, int right, int bottom){
	glGetFloatv (GL_PROJECTION_MATRIX, projection);
	glGetFloatv (GL_MODELVIEW_MATRIX, modelview);
	
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho( left, right, bottom, top, -1, 1 );
	
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


/**
 * This function changes our returns our projection mode from 2D to 3D
 */
void	nrPerspectiveMode(){
    glMatrixMode( GL_PROJECTION );
    glLoadMatrixf (projection);
    glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf (modelview);
}


void nrPBufferRelease(){
	nrPBuffer.clean();
}

void nrPBufferInit(int width, int height, int flags){
	nrPBuffer.clean();
	nrPBuffer.init(width, height, flags);
}


void nrPBufferEnable(){
	nrPBuffer.enable();
}


void nrPBufferDisable(){
	nrPBuffer.disable();
}


int	nrPBufferWidth(){
	return nrPBuffer.width;
}

int	nrPBufferHeight(){
	return nrPBuffer.height;
}
