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

#include "Resources.h"

/**
* Default constructor of each resource type.
* Here we will initialize default values.
**/
nrIResource::nrIResource(){
	mResIsLoaded = false;
}

/**
* Destruct the resource object
**/
nrIResource::~nrIResource(){

}

/**
* This will add the resource to the resource manager.
*
* @param manager Resource manager to which one the resource should be added
* @param name Unique name of the resource
* @param group Name of a group to which one this resource belongs.
* @return either NR_OK or error code.
**/
nrResult nrIResource::addToResourceManager(nrCResourceManager& manager,
									const string& name, const string group){

	return manager.add(this, name, group);
}

