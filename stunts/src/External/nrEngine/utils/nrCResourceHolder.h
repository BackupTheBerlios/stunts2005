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


#ifndef _NR_C_RESOURCE_HOLDER_H_
#define _NR_C_RESOURCE_HOLDER_H_


/**-------------------------------------------------------------------------
 * Forward declarations
 *------------------------------------------------------------------------*/
class nrCResourceHolder;


/**-------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

//! Interstage between manager and resources for more efficience and safety
/**
 * This is a holder class which is holding a resource of different type.
 * Resource pointers are pointing to this holder, so by referencing the pointer
 * you do not have to ask the manager to get the resource, but directly to access
 * it through this holder. So actually resource manager manages holders and
 * not resources or pointers directly.
 * We use such a system to be more efficient by dereferncing the resource pointer.
 * Other and more trivial way is to ask the manager for resource by giving him the
 * handle. This end in a look up of handle tables and returning the resource pointer.
 * If we use holders, pointers will directly access that holders and each holder
 * is showing to one resource. So we do not have to do any lookups.
 *
 * This class is working transparent to the whole resource management system, so
 * you actually do not have to know how this class works and you also do not have
 * to derive any classes from it.
 *
 * This system was get from Game Programming Gems 4, and was expanded to be more
 * flexible and more efficient.
 *
 * \ingroup resource
 **/
class nrCResourceHolder{
public:

	friend class nrCResourceManager;
	template <class ResType> friend class nrCResourcePtr;
	
	/**
	 * Create the object of this class
	 **/
	nrCResourceHolder(){
	}

	
	/**
	 * Remove the object from the memory and also delete the resource
	 * which is holded by this holder
	 **/	
	~nrCResourceHolder(){
		mResource.reset();
	}

	
	/**
	 * Returns true if this holder does not assigned to any resource.
	 **/
	inline bool isNull(){
		return mResource.get() == NULL;
	}

private:

	shared_ptr<nrIResource>	mResource;
	shared_ptr<nrIResource> mEmptyResource;
	
		
	/**
	 * Reset the stored resource to a new value
	 **/
	void resetRes(nrIResource* res){
		mResource.reset(res);
	}


	/**
	 * Returns true if this holder has a pointer to a empty resource
	 **/
	inline bool hasEmpty(){
		return mEmptyResource.get() == NULL;
	}

	
	/**
	 * This will bind this holder to specific empty resource.
	 * After loading of a resource from a file the manager will
	 * create/load the empty resource according to the type of the resource.
	 * Then it will generate a name for it and will bind the holder to that
	 * empty resource. So if you will next load a resource of the same type,
	 * so holder's empty resource pointer will still point to the same one.
	 **/
	void setEmptyResource(nrIResource* res){
		mEmptyResource.reset(res);
	}
	void setEmptyResource(shared_ptr<nrIResource> res){
		mEmptyResource = res;
	}

	
	nrIResource* getEmpty(){
		return mEmptyResource.get();
	}

	
	/**
	 * Get access to the stored resource.
	 * We declare this function as private, so we cannot access to the resource
	 * from outside either as from manager or a smart pointer to resources.
	 * We need to do it in such a way, to prevent you from deleting the resources
	 * from outside as from the resource management system.
	 *
	 * If the resource is temporary unloaded or if it is not exists, the holder will
	 * give you the empty resource back, which still can be NULL or not
	 **/
	nrIResource* getResource(){
		if (!isNull()){
			if (mResource->isResLoaded()){
				return mResource.get();
			}
		}
		return mEmptyResource.get();
	}
		
};

#endif
