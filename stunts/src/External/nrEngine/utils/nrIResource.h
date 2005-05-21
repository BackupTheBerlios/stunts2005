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


#ifndef _NR_I_RESOURCE__H_
#define _NR_I_RESOURCE__H_


/**-------------------------------------------------------------------------
 * Forward declarations
 *------------------------------------------------------------------------*/
class nrIResource;

/**-------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include "nrCResourceManager.h"

//! General interface to hold any kind of resources
/**
 * This is an interface which is describing how resource classes should looks like.
 * Derive your own classes to get resource management system working. Each
 * resource containing only once in the memory if it has the same name. So all
 * names of resources should be unique. Resource is not directly stored by the
 * manager but by the nrCResourceHolder. This holder just stores the resource
 * and is unique for each resource. Manager just manages such a holders and
 * gives the resource pointers access to it.
 *
 * \ingroup resource
 **/
class nrIResource{
public:

	/**
	 * We need to declare this classes as friend, so it can get full access to the data
	 * provided by this resource.
	 **/
	friend class nrIResourceLoader;
	friend class nrCResourceManager;

	nrIResource();
	virtual ~nrIResource();

	/**
	 * Here the class should return cound of bytes reserved
	 * by the resource. The size must not contain the size of the class itself.
	 * For example: 32Bit Texture of 16x16 Pixel has the size: 16*16*4 = 1024 Bytes
	 **/
	virtual uint32 getResDataSize(){return 0;}


	/**
	 * Return here the name of the resource type. This name will be used
	 * to assign right empty resources to the resource pointer/holder objects.
	 * This name must be unique. E.g: "Texture", "Sound", "nrCTexture", ...
	 **/ 
	virtual const string getResTypeName() = 0;

	
	/**
	 * Unload the resource.
	 * This function is declared as protected/private to be accessable only
	 * from the manager.
	 **/
	virtual nrResult unloadRes() = 0;
	

	/**
	 * Reload the resource.
	 * This function is declared as protected/private to be accessable only
	 * from the manager.
	 **/
	virtual nrResult reloadRes() = 0;

	// Connect to resource manager
	virtual nrResult addToResourceManager(nrCResourceManager& manager,
										const string& name, const string group = "");

	
	/**
	 * Return true if the resource is marked as loaded. If any resource is loaded
	 * so it contains in the memory and has it's full data there.
	 **/
	bool 				isResLoaded(){return mResIsLoaded;}

	/**
	 * Return handle of this resource
	 **/
	nrResourceHandle 	getResHandle(){return mResHandle;}

	/**
	 * Return group name to which one this resource belongs
	 **/
	const string&		getResGroup(){return mResGroup;}

	/**
	 * Get the name of the resource
	 **/
	const string&		getResName(){return mResName;}
		
protected:
	//! Shows whenever resource is loaded (is in the memory)
	bool mResIsLoaded;

	//! Resource's loader with which one it was created
	nrResourceLoader	mResLoader;

private:
	//! Group name to which one this resource own
	string mResGroup;

	//! Handle of the resource given from manager
	nrResourceHandle	mResHandle;

	//! name of the resource
	string mResName;
	
};

#endif
