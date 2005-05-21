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


#ifndef _NR_C_RESOURCE_MANAGER_H_
#define _NR_C_RESOURCE_MANAGER_H_


/**-------------------------------------------------------------------------
 * Forward declarations
 *------------------------------------------------------------------------*/
class nrCResourceManager;


/**-------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include "../nrISingleton.h"


//! General pointer/handle based resource management system
/**
 * This class is a manger for every kind of resource used in our games.
 * Textures, sounds, meshes, ... - all things that can be loaded from disk
 * are resources managed by this module. You can derive your own classes
 * to write own storing algorithms for different kind of resources. This class
 * will use standard implementation and will try to be as efficient as possible.
 * This manager can index resources, look them up, load and destroy them, and
 * temporaly unload resources from memory to use only allowd size of memory.
 *
 * We will use a priority system to determine which resources can be unloaded
 * and for two resources of the same priority the most used will stay in memory.
 *
 * This code and idea was get from Game Programming Gems 4. <a href="http://www.ogre3d.org">
 * Ogre3D-Library</a> was also used to get an idea how to get the resource manager
 * working without using a whole library behind.
 *
 * The member functions createEmptyResource(), loadResource() are build by templates
 * that allows us to create new kind of resource by storing it under standrd nrResourcePtr.
 * For example if you have a class A derived from nrCResource and this class is don't known
 * by our engine, so you can just load the resource object of this class by calling:
 * <code>nrResource.loadResource<A>(...)</code>. This allows us to create/load resources
 * of specified types and not just to create standard nrCResource object which do not always
 * can be casted to derived objects.
 *
 * <b>-</b> Meaning of resource groups:
 *		@par - Each resource can exists in a group.
 *			 - Each group has got it's unique names.
 *			 - You has access to whole groups either to single resources.
 *			 - E.g: you can split your level in patches and each resource in such a patch is
 *			   in patch's resource group. So if the player go to far away from such a patch,
 *			   it can be disabled or unloaded from the memory. You have now just to call one
 *			   unload function for the whole group.
 *			 - Groups must not be disjoint, so you can have same resource in different groups.
 *
 * \ingroup resource
 **/
class nrCResourceManager : public nrISingleton<nrCResourceManager>{
public:

	// Constr & Destr
	nrCResourceManager();
	virtual ~nrCResourceManager();

	// Memory size
	nrResult setMemoryBudget(size_t bytes);
	size_t	 getMemoryBudget() const;
	size_t	 getMemoryUsage() const;
	
	// Resource loader registry
	nrResult	registerLoader(const string& name, nrResourceLoader loader, int32 count = 0, string* fileTypes = NULL);
	nrResult	removeLoader(const string& name);
	
	nrResourceLoader getLoaderByFile(const string& fileType);
	nrResourceLoader getLoader(const string& name);
												
	
	// Resource creating/loading
	template<class ResourceType>
	nrResourcePtr	createResource(const string& name, const string& group = "");


	template<class ResourceType>
	nrResourcePtr	loadResource(const string& name,
								const string& fileName,
								const string& group = "",
								nrResourceLoader manualLoader
									= nrResourceLoader());

	// Resource handling
	virtual nrResult		add(nrIResource* res, const string& name, const string& group = "");
	
	virtual nrResult		unload(const string& name);
	virtual nrResult		unload(nrResourcePtr& res);
	virtual nrResult		unload(nrResourceHandle& handle);
	
	virtual nrResult		reload(const string& name);
	virtual nrResult		reload(nrResourcePtr& res);
	virtual nrResult		reload(nrResourceHandle& handle);
	
	virtual nrResult		remove(const string& name);
	virtual nrResult		remove(nrResourcePtr& res);
	virtual nrResult		remove(nrResourceHandle& handle);

	virtual nrResourcePtr	getByName(const string& name);
	virtual nrResourcePtr	getByHandle(const nrResourceHandle& handle);
	
	
	// Resource Group handling
	virtual nrResult		unloadGroup(const string& group);
	virtual nrResult		reloadGroup(const string& group);
	virtual nrResult		removeGroup(const string& group);

protected:

	//------------------------------------------
	// Variables
	//------------------------------------------
	size_t		mMemBudget;
	size_t		mMemUsage;

	nrResourceHandle	mLastHandle;

	
	typedef vector<nrResourceLoader> loader_list;
	typedef map<string, int32>		 loader_map;
	
	loader_list	mLoaderList;
	loader_map	mLoaderName;
	loader_map	mLoaderToFileType;


	typedef map<nrResourceHandle, shared_ptr<nrCResourceHolder> > 	res_hdl_map;
	typedef map<string, nrResourceHandle> 						res_str_map;
	typedef map<string, list<nrResourceHandle> >		 		res_grp_map;
	typedef map<string, shared_ptr<nrIResource> >				res_empty_map;
	
	res_hdl_map	mResource;
	res_str_map mResourceName;
	res_grp_map mResourceGroup;
	res_empty_map mEmptyResource;
	
	
	//------------------------------------------
	// Methods
	//------------------------------------------
	virtual nrResult checkMemoryUsage();

	nrResult removeImpl(nrResourcePtr& ptr);
	
	/**
	 * This function should create a empty resource and give a pointer to it back.
	 * This is a virtual function which returns NULL in the base class. So if you
	 * want to derive other resource manager classes and you want to have very
	 * special creating function you should write out this function.
	 *
	 * If this method is returns NULL, so we will use the resource object
	 * created createEmptyResource() function.
	 * @see createEmptyResource()
	 **/
	virtual nrIResource* createEmptyImpl(nrResourceHandle hdl, const string& name,
										const string& group = ""){return NULL;}

	/**
	 * This function should load a resource from a file. This function is declared
	 * as virtual so it can be overloaded through functions from dirived classes.
	 * This is not needed but if you want to have your own loading functions, you
	 * can do it here
	 * @see loadResource()
	 **/
	virtual nrIResource* loadResourceImpl(nrResourceHandle hdl,
								const string& name,
								const string& fileName,
								const string& group = "",
								nrResourceLoader manualLoader
									= nrResourceLoader()){return NULL;}
	
};

#endif
