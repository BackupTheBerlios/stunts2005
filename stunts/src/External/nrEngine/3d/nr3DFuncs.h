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

#ifndef __NR_3DFUNCS_H_
#define __NR_3DFUNCS_H_

#include "nr3DEngine.h"

void		nrOrthoMode(int left, int top, int right, int bottom);
void		nrPerspectiveMode();

void 	nrPBufferInit(int width, int height, int flags);
void 	nrPBufferEnable();
void 	nrPBufferDisable();
int    	nrPBufferWidth();
int    	nrPBufferHeight();

#endif
