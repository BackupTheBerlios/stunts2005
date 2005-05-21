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

#ifndef __NR_C_POINT_LIGHT_H
#define __NR_C_POINT_LIGHT_H


/*------------------------------------------------------------------------------
 * Forward Declarations
 *----------------------------------------------------------------------------*/
class nrCPointLight;
	

/*------------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------------*/
#include "nrCLight.h"


/**
 * Point light source. 
 * @see nrCLight
 * \ingroup 3d
 */
class nrCPointLight: public nrCLight {
public:
	
	// Constructor & destructor
	nrCPointLight();
	~nrCPointLight();

	// get properties
	bool isSpotLight(){return false;}
	bool isDirectionalLight(){return false;}
	bool isPointLight(){return true;}
	bool isPositional(){return true;}
		
};

#endif
