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


#ifndef _NR_C_RESOURCE_POINTER_H_
#define _NR_C_RESOURCE_POINTER_H_


/**-------------------------------------------------------------------------
 * Forward declarations
 *------------------------------------------------------------------------*/
template<class ResType>
class nrCResourcePointer;


/**-------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include <boost/shared_ptr.hpp>
using boost::shared_ptr;


//! Resource pointer is a smart pointer pointing to the resource
/**
 * This is a smart pointer that can be used to access resources manages by the
 * resource manager. This pointer will automaticly be pointing to empty resource
 * if the resource was unloaded by the manager.
 *
 * You can create more than one resource pointer to a resource. Each access to such
 * a resource through the manager will create a new one for you. However the pointers
 * are pointing to one resource holder. Each resource holder is controlled/manged
 * by the manager. So if you for example unload a resource to which one you has pointers,
 * the manager will replace the resource holding by the holder with empty one. So if
 * you try to access the resource you can still access them, but they are empty.
 * Our systems also allows you to delete the manager from the memory, but the pointers
 * will stay valid. So you can also remove the manager after you loaded all resources.
 * However you can then do not manage the resources, so this is a way you should <b>not</b>
 * do it !!!
 *
 * @note We do not have any public constructors or destructors. This will prevent
 *		 you by using the resource management system in the wrong way. You have
 *		 to use manager before you can use the pointers to resources. This will
 *		 give the nrEngine the safety of using its resource management system.
 * 
 * \ingroup resource
 **/
template<class ResType>
class nrCResourcePtr{
public:

	/**
	 * Copy constructor. This will copy one pointer to the another one.
	 **/
	nrCResourcePtr(const nrCResourcePtr<ResType>& ptr);
	
	/**
	 * Assign a pointer to this one.
	 **/
	nrCResourcePtr<ResType> & operator= (const nrCResourcePtr<ResType>& resPtr);


	/**
	 * Access to the resource to which one this pointer points. This access need
	 * 1 static_cast, 3 function calls until it returns the resource.
	 * So this function is running in O(1) and is pretty efficient
	 **/
	ResType* operator->() const;
	
	/**
	 * Access to the resource to which one this pointer points. This access need
	 * 1 static_cast, 3 function calls until it returns the resource.
	 * So this function is running in O(1) and is pretty efficient
	 **/
	ResType& operator*() const;


	/**
	 * Check if the given pointer is the same as this one.
	 * @note Two pointers are the same, if they showing to the same resource.
	 * 		 They also the same if their holders are the same.
	 *
	 **/
	bool operator==(nrCResourcePtr<ResType>& res) const;
	
	/**
	 * Check if two pointers are not the same
	 **/
	bool operator!=(nrCResourcePtr<ResType>& res) const;

	/**
	 * Check whenever this pointer is NULL
	 **/
	bool isNull()const;

	
private:

	friend class nrCResourceManager;
	
	/**
	 * Create an instance of the resource pointer. This pointer is pointing
	 * to the resource stored by the holder.
	 **/
	nrCResourcePtr(shared_ptr<nrCResourceHolder> holder){
		mHolder = holder;
	}

	nrCResourcePtr(){

	}
	
	/**
	 * Release used memory
	 **/
	~nrCResourcePtr(){
		mHolder.reset();
	}


	/**
	 * Get the holder to which one this pointer shows
	 **/
	shared_ptr<nrCResourceHolder> getResourceHolder(){
		return mHolder;
	}

	
	//------------------------------------
	// Variables
	//------------------------------------
	shared_ptr<nrCResourceHolder> mHolder;
	
};

#endif
