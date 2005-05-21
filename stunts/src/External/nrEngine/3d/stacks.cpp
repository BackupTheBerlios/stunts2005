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

#define DEFINE_OPENGL_STATE(name, openglEnum)\
	STACK_DEFINE(name, bool);\
	STACK_PUSH_FUNC(name, bool, glIsEnabled(openglEnum));\
	STACK_POP_ONOFF_FUNC(name);\
	STACK_PUSHON_FUNC(name);\
	STACK_PUSHOFF_FUNC(name);\
	STACK_ON_FUNC_DEF(name){glEnable(openglEnum);}\
	STACK_OFF_FUNC_DEF(name){glDisable(openglEnum);}\
	
/*--------------------------------------------------------------------------
 On/Off einschalten ausschalten
 -------------------------------------------------------------------------*/
DEFINE_OPENGL_STATE(Lit,GL_LIGHTING);
DEFINE_OPENGL_STATE(Blend,GL_BLEND);
DEFINE_OPENGL_STATE(DepthRead,GL_DEPTH_TEST);
DEFINE_OPENGL_STATE(ColorMaterial, GL_COLOR_MATERIAL);
DEFINE_OPENGL_STATE(CullFace,GL_CULL_FACE);
DEFINE_OPENGL_STATE(ScissorTest,GL_SCISSOR_TEST);
DEFINE_OPENGL_STATE(Texture2D,GL_TEXTURE_2D);

 
/*--------------------------------------------------------------------------
 Depth Mask einschalten ausschalten
 -------------------------------------------------------------------------*/
STACK_DEFINE(DepthWrite, bool);

STACK_PUSH_FUNC(DepthWrite, bool, STACK_ISON_STATE(DepthWrite));
STACK_POP_ONOFF_FUNC(DepthWrite);
STACK_PUSHON_FUNC(DepthWrite);
STACK_PUSHOFF_FUNC(DepthWrite);

STACK_ON_FUNC_DEF(DepthWrite){
	glDepthMask(GL_TRUE);
	STACK_ISON_STATE(DepthWrite) = true;
}

STACK_OFF_FUNC_DEF(DepthWrite){
	glDepthMask(GL_FALSE);
	STACK_ISON_STATE(DepthWrite) = false;
}


/*--------------------------------------------------------------------------
 Color Mask einschalten ausschalten
 -------------------------------------------------------------------------*/
STACK_DEFINE(ColorWrite, bool);

STACK_PUSH_FUNC(ColorWrite, bool, STACK_ISON_STATE(ColorWrite));
STACK_POP_ONOFF_FUNC(ColorWrite);
STACK_PUSHON_FUNC(ColorWrite);
STACK_PUSHOFF_FUNC(ColorWrite);

STACK_ON_FUNC_DEF(ColorWrite){
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	STACK_ISON_STATE(ColorWrite) = true;
}

STACK_OFF_FUNC_DEF(ColorWrite){
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	STACK_ISON_STATE(ColorWrite) = false;
}


#undef DEFINE_OPENGL_STATE
