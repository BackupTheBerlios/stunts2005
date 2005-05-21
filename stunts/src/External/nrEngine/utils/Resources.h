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
#ifndef _NR__RESOURCES_H
#define _NR__RESOURCES_H

/*!
 * \defgroup resource Resource managment
 *
 * <b>nrEngine</b> has support for resource managment. Each resource can be
 * loaded or unloaded at the runtime. There is a class for generic support
 * of resource managment system nrCResourceManager. This class provides generic
 * functions used for resource reservation/allocation/release. We also provides
 * a possibility to unload the resource at runtime but not to delete it from
 * the memory completly. So the application can still access to it and will
 * get only empty resource back.
 * 
 * Each resource class has to be derived from nrCResource and provide
 * their own functions for returning an empty resource. nrCResourcePtr is a
 * class representing smart pointer to any resource. This pointers can be
 * used as normal pointers and will give you transparent access to empty
 * item if this resource is not present in the memory.
 *
 * So with the help of
 * such subsystem you do not have to check each time you want to use the
 * data if it still exists in the memory. This happens transparent to you and
 * will stay efficient.
 *
 * <img src="../dias/Resources.png">
 **/

 
/*--------------------------------------------------------------------------
 * Provided classes
 *------------------------------------------------------------------------*/
class nrCResourceManager;
class nrIResourceLoader;
class nrIResource;
class nrCResourceHolder;

/*--------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include "../nrTypes.h"

#include <boost/shared_ptr.hpp>

#include "nrCResourcePtr.h"

/*--------------------------------------------------------------------------
 * Types
 *------------------------------------------------------------------------*/

/**
 * Smart resource pointer to a standard resource item.
 **/
#define nrResourcePtr nrCResourcePtr<nrIResource>

/**
 * Smart pointer pointing to a loader
 **/
#define nrResourceLoader boost::shared_ptr<nrIResourceLoader>


/**
 * This handle will store something like resource id.
 **/
typedef uint32					nrResourceHandle;


/*--------------------------------------------------------------------------
 * Provide interfaces
 *------------------------------------------------------------------------*/
#include "nrIResource.h"
#include "nrCResourceHolder.h"
#include "nrCResourceManager.h"
#include "nrIResourceLoader.h" 

/*--------------------------------------------------------------------------
 * Provide functions
 *------------------------------------------------------------------------*/
#define	nrResources nrCResourceManager::GetSingelton()

void	nrResourceInit();
void	nrResourceDelete();


#endif
