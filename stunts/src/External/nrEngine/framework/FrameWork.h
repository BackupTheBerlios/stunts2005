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
#ifndef _NR__FRAMEWORK_H
#define _NR__FRAMEWORK_H

/*!
 * \defgroup fw Framework of the engine
 *
 * <b>nrEngine</b> provides small framework to you to use in your applications. At now there is
 * only support for SDL. In the future this framework will be expanded to support GLFW or other
 * beautifull libraries. With helpf of this classes you can programm or control application
 * written for nrEngine without thinking on how to create window or how to read user input.
 * This stuff should be done here and it is platform independent.
 *
 * How nrEngine's framework works:
 *	- First you initialize the framework engine
 *	- Then you create an render context and use it
 *	- Render context is a class representing one rendering window. So if you want
 *		to have more than one rendering window running at the same time you have to
 *		create more then one rendering context (see: nrIRenderContext - Documentation for
 *		more information about nrEngine's rendering contexts)
 *	- Because nrIFramework is an singleton task you can add it into kernel's task list
 *		and let it update itself each frame automaticly.
 *	- If you do not add it to the task list, you have to update it by yourself to
 *		get user and system events passed to your application
 *	- You have also to specify the callback function for quitting the application.
 *		Because there could be a signal from outside which should quit your app.
 *
 * @note At now nrEngine supports only one rendering context. Creating more than one
 *		context will do not have any effect (it could be buggy). So please do not
 *		create more than one.
 * <img src="../dias/Framework.png">
 **/

 
/*--------------------------------------------------------------------------
 * Provided classes
 *------------------------------------------------------------------------*/
class nrIFramework;
class nrIRenderContext;

class nrCFrameworkSDL;
class nrCRenderContextSDL;


/*--------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include "../nrTypes.h"
#include "nrIRenderContext.h"
#include "nrIFrameWork.h"
#include "nrCFrameWorkSDL.h"
#include "nrCRenderContextSDL.h"



/*--------------------------------------------------------------------------
 * Provided global functions
 *-------------------------------------------------------------------------*/
#define		nrFramework		nrIFramework::GetSingleton()
nrResult	nrFrameworkInit();
nrResult	nrFrameworkDelete();

 
#endif
