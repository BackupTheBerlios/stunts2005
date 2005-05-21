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

#ifndef __NR_ENGINE_H_
#define __NR_ENGINE_H_


/*--------------------------------------------------------------------------
 * Boost-Library using
 *------------------------------------------------------------------------*/
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_array.hpp>
#include <string>

using boost::shared_ptr;
using boost::scoped_ptr;
using boost::static_pointer_cast;
using boost::dynamic_pointer_cast;
using boost::shared_array;
using std::string;

/*--------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include "nrTypes.h"


/*--------------------------------------------------------------------------
 * Global Definitions
 *------------------------------------------------------------------------*/
#define nrSettings		nrCSettings::GetSingleton()
#define nrVFS			nrCFileSystem::GetSingleton()
#define nrPBuffer		nrCPBuffer::GetSingleton()
#define nrLog			nrCLog::GetSingleton()
#define nrKernel		nrCKernel::GetSingleton()
#define nrConsole		nrCConsole::GetSingleton()
#define nrClock			nrCClock::GetSingleton()


/*--------------------------------------------------------------------------
 * Global Functions
 *------------------------------------------------------------------------*/
// Engine 
void		nrEngineInit();
void		nrEngineDelete();
bool		nrEngineIsActive();

// Error
string 	nrErrorGetShort(nrResult res);
void 	nrAssertGeneric(const char *szExp, const char *szFilename, int iLineNum);


// console 
nrResult		nrConsoleInit();
nrResult		nrConsoleDelete();


// PBuffer
void	nrPBufferInit(int width, int height, int flags = NR_PBUFFER_RGBA | NR_PBUFFER_DEPTH);
void	nrPBufferEnable();
void	nrPBufferDisable();
void	nrPBufferRelease();
int		nrPBufferWidth();
int		nrPBufferHeight();

// Settings
#define 	nrSettingsRegister(type, var, name)\
			{\
				shared_ptr<nrIDator> _temp(new nrCDator<type>(var));\
				nrSettings.registerVariable(name,_temp);\
			}
				
#define		nrSettingsRegisterList(type, var, name)\
			{\
				shared_ptr<nrIDator> _temp(new nrCDatorList<type>(var));\
				nrSettings.registerVariable(name,_temp);\
			}

			
#define 	nrSettingsDeregister(name) nrSettings.deregisterVariable(name);
#define 	nrSettingsSet(name, value) nrSettings.set(name, lexical_cast<string>(value))
			

/*!
 * \defgroup engine Engine's core functions
 *
 * Engine's core function are methods or macros provided to you
 * from the engine. Some of them are essential and some of them helps
 * you to use engine in simpler way than without them.
 **/
 

/*--------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include "nrDator.h"
#include "nrCKernel.h"
#include "nrIChangeable.h"
#include "nrISingletonTask.h"
#include "nrITask.h"

#include "maths/Math.h"

#include "utils/Resources.h"

#include "framework/FrameWork.h"

#include "vfs/nrVFS.h"
#include "vfs/nrVFSFile.h"

#include "utils/nrCLog.h"
#include "utils/nrSettings.h"
#include "utils/console/nrCConsole.h"
#include "utils/nrCClock.h"

#include "3d/nr3DEngine.h"

#endif
