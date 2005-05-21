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


#ifndef _NR_I_RESOURCE_LOADER_H_
#define _NR_I_RESOURCE_LOADER_H_


/**-------------------------------------------------------------------------
 * Forward declarations
 *------------------------------------------------------------------------*/
class nrIResourceLoader;


/**-------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/


//! Resource loader interface for loading different resources
/**
 * This is an interface which has to be implemented by each resource loader.
 * A resource loader is an object which can load different kind of resources
 * from different kind of files. For example there can be a image loader
 * which can load PNG, DDS, TGA, .. images and this one can be used to load
 * texture resources. In this case you can derive resource loader classes for
 * different filetypes from your texture resource loader class or just use
 * image loading functions for different types.
 *
 * Resource loader can be registered by the resource manager. After it is
 * registered it will be automaticly used to load files of different types.
 * If there is no loader registered for that type, so file will not be loaded,
 * error will occurs and empty resource will be used.
 *
 * Each loader has also to support creating/loading of empty resources.
 * By creating of an empty resource it's holder will not hold any resource.
 * After you loading the resource it will be initialized with the resource data
 * and with the empty resource.
 *
 * If you have files that are also resources but are not supported by the nrEngine
 * you can derive a loader class from this interface. Then register the object,
 * created from this derived class, by the resource manager and this class will
 * automaticly used for such kind of resources. This is something like a plugin
 * system on compiling layer/level.
 *
 * \ingroup resource
 **/
class nrIResourceLoader{
public:

	// Constr & Destr
	nrIResourceLoader(){}
	virtual ~nrIResourceLoader(){}


	/**
	 * Load the resource.
	 **/
	virtual nrResult loadResource(nrIResource* res) = 0;

	/**
	 * Creating of an empty resource.
	 **/
	virtual nrIResource* createEmptyResource() = 0;


};

#endif
