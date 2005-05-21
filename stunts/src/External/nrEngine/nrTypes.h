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

#ifndef __NR_TYPES_H_
#define __NR_TYPES_H_


//------------------------------------------------------------------------------
//	Standard library includings and definitions
//------------------------------------------------------------------------------
#ifndef EXT_HASH
#define EXT_HASH
#endif

#include "nrStdHeaders.h"

//------------------------------------------------------------------------------
//	Basic Type Definitions
//------------------------------------------------------------------------------
typedef unsigned char 	BYTE;
typedef unsigned char 	byte;
typedef unsigned char 	uchar;

typedef char			int8;
typedef unsigned char	uint8;

typedef signed short	int16;
typedef	unsigned short	uint16;

typedef signed int		int32;
typedef unsigned int	uint32;

typedef float			float32;
typedef double			float64;

#if defined(_WIN32)
typedef   signed __int64  int64;
typedef unsigned __int64 uint64;
// #elif defined(LINUX)
#else
typedef   signed long long  int64;
typedef unsigned long long uint64;
#endif


typedef uint32		nrResult;


//------------------------------------------------------------------------------
//	Some definitions
//------------------------------------------------------------------------------
#define NR_AUTO_OBJ_SIZE uint64 size(){return sizeof(*this);}

#define MCHAR2(a, b) (a | (b << 8))
#define MCHAR4(a, b, c, d) (a | (b << 8) | (c << 16) | (d << 24))



//------------------------------------------------------------------------------
//	Assert definitions
//------------------------------------------------------------------------------
#define ASSERT( exp )	if ( !(exp) ) nrAssertGeneric( #exp, __FILE__, __LINE__ )
void 	nrAssertGeneric(const char *szExp, const char *szFilename, int iLineNum);


//------------------------------------------------------------------------------
//	Enums for our Engine
//------------------------------------------------------------------------------
#define NR_PBUFFER_RGB 				1 << 0
#define NR_PBUFFER_RGBA				1 << 1
#define NR_PBUFFER_DEPTH			1 << 2
#define NR_PBUFFER_STENCIL			1 << 3
#define NR_PBUFFER_FLOAT 			1 << 4
#define NR_PBUFFER_MULTISAMPLE_2 	1 << 5
#define NR_PBUFFER_MULTISAMPLE_4 	1 << 6



//------------------------------------------------------------------------------
//	Error code definitions
//------------------------------------------------------------------------------
#define NR_OK	 					0
#define NR_UNKNOWN_ERROR 			0xFFFFFFFF
#define NR_BAD_PARAMETERS			1 << 0
#define NR_OUT_OF_MEMORY			1 << 1

#define NR_FILE_ERROR 				0x01000000
#define NR_FILE_NOT_FOUND 			NR_FILE_ERROR | (1 << 0)
#define NR_FILE_ERROR_IN_LINE		NR_FILE_ERROR | (1 << 1)

#define NR_PROFILE_ERROR			0x02000000
#define NR_PROFILE_NOT_FOUND		NR_PROFILE_ERROR | (1 << 0)
#define NR_PROFILE_NOT_LOADED		NR_PROFILE_ERROR | (1 << 1)
#define NR_PROFILE_NOT_EXISTS		NR_PROFILE_ERROR | (1 << 2)
#define NR_PROFILE_ALREADY_EXISTS 	NR_PROFILE_ERROR | (1 << 3)

#define NR_EXTENSION_ERROR			0x03000000
#define NR_EXTENSION_NOT_SUPPORTED	NR_EXTENSION_ERROR | (1 << 0)
  
#define NR_VFS_ERROR				0x04000000
#define NR_VFS_ALREADY_OPEN			NR_VFS_ERROR | (1 << 0)
#define NR_VFS_CANNOT_OPEN			NR_VFS_ERROR | (1 << 1)
#define NR_VFS_CANNOT_CLOSE			NR_VFS_ERROR | (1 << 2)
#define NR_VFS_IS_NOT_OPEN			NR_VFS_ERROR | (1 << 3)
#define NR_VFS_FILE_NOT_FOUND		NR_VFS_ERROR | (1 << 4)
#define NR_VFS_FILE_NOT_OPEN		NR_VFS_ERROR | (1 << 5)
#define NR_VFS_FILE_END_REACHED		NR_VFS_ERROR | (1 << 6)
#define NR_VFS_FILE_READ_ERROR		NR_VFS_ERROR | (1 << 7)
#define NR_VFS_LINE_READ_ERROR		NR_VFS_ERROR | (1 << 8)
#define NR_VFS_SCAN_READ_ERROR		NR_VFS_ERROR | (1 << 9)
#define NR_VFS_SEEK_ERROR			NR_VFS_ERROR | (1 << 10)

#define NR_APP_ERROR				0x05000000

#define NR_LOG_ERROR				0x06000000

#define NR_SETTINGS_ERROR			0x07000000
#define NR_SETTINGS_VAR_ALREADY_REGISTERED 	NR_SETTINGS_ERROR | (1 << 1)
#define NR_SETTINGS_VAR_NOT_REGISTERED 		NR_SETTINGS_ERROR | (1 << 2)

#define NR_KERNEL_ERROR				0x08000000
#define NR_KERNEL_NO_TASK_FOUND 	NR_KERNEL_ERROR | (1 << 1)

#define NR_TASK_ERROR				0x09000000
#define NR_TASK_NOT_READY			NR_TASK_ERROR | (1 << 1)

#define NR_CONSOLE_ERROR			0x0A000000
#define NR_CONSOLE_ITEM_ALREADY_EXISTS	NR_CONSOLE_ERROR | (1 << 1)


#define NR_TEX_ERROR				0x0B000000
#define NR_TEX_INVALID_FORMAT		NR_TEX_ERROR | (1 << 1)


#define NR_CLOCK_ERROR					0x0C000000
#define NR_CLOCK_OBSERVER_NOT_FOUND 	NR_CLOCK_ERROR | (1 << 1)
#define NR_CLOCK_OBSERVER_ALREADY_ADDED NR_CLOCK_ERROR | (1 << 2)

#define NR_FW_ERROR					0x0D000000
#define NR_FW_CANNOT_INITIALIZE		NR_FW_ERROR | (1 << 1)
#define NR_FW_FAILED_TO_RESIZE		NR_FW_ERROR | (1 << 2)
#define NR_FW_INVALID_RC_ID			NR_FW_ERROR | (1 << 3)
#define NR_FW_ALREADY_INIT			NR_FW_ERROR | (1 << 4)
#define NR_FW_NOT_INITIALIZED		NR_FW_ERROR | (1 << 5)

#define NR_RC_ERROR					0x0E000000
#define NR_RC_CANNOT_SETUP_PIXEL_FORMAT NR_RC_ERROR | (1 << 1)


#define NR_ENGINE_ERROR				0x0F000000
#define NR_ENGINE_YOU_MUST_CREATE_ENGINE_BEFORE NR_ENGINE_ERROR | (1 << 1)

#define NR_RES_ERROR				0x10000000
#define NR_RES_LOADER_ALREADY_EXISTS NR_RES_ERROR | (1 << 1)
#define NR_RES_LOADER_NOT_REGISTERED NR_RES_ERROR | (1 << 2)
#define NR_RES_ALREADY_EXISTS		 NR_RES_ERROR | (1 << 3)
#define NR_RES_BAD_FILETYPE			 NR_RES_ERROR | (1 << 4)
#define NR_RES_CAN_NOT_LOAD_EMPTY	 NR_RES_ERROR | (1 << 5)
#define NR_RES_NOT_FOUND			NR_RES_ERROR | (1 << 6)
#define NR_RES_LOADER_NOT_EXISTS	NR_RES_ERROR | (1 << 7)

//----------------------------------------------------------------------------------
// Keyboard definitions
//----------------------------------------------------------------------------------
#include "nrKeySym.h"

#endif
