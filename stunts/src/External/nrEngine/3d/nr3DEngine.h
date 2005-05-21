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

#ifndef __NR_3DENGINE_H_
#define __NR_3DENGINE_H_

#define GL_GLEXT_PROTOTYPES
#define GL_GLEXT_LEGACY
#define GLX_GLXEXT_PROTOTYPES
#define GLX_GLXEXT_LEGACY

#include <GL/gl.h>                                      
#include <GL/glx.h>
#include <GL/glu.h>
#include "gl_ext/glext.h"
#include "gl_ext/glxext.h"
#include <GL/glut.h>


#include "../nrTypes.h"

#include "stacks.h"
#include "nrPBuffer.h"
#include "nr3DFuncs.h"
#include "nrCTexture.h"
#include "nrCBillboardTexture.h"
#include "nrCFont.h"
#include "nrCFrustum.h"
#include "nrCCamera.h"

#include "nrCLight.h"
#include "nrCLensFlare.h"

#include "nrCTextureLoader.h"

/*!
 * \defgroup 3d 3D-Engine
 *
 * This module is a part of the nrEngine - Project. Here you can find classes used for
 * 3D-Rendering. At now this is not 3D-Engine but it provide a lot of useful functions & classes
 * for rendering.
 * \author Art Tevs <tevs@mpi-sb.mpg.de>
 */
#endif
