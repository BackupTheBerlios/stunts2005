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

/**-------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include "Resources.h"


/**
 * Copy constructor. This will copy one pointer to the another one.
 **/
template<class ResType>
nrCResourcePtr<ResType>::nrCResourcePtr(const nrCResourcePtr<ResType>& ptr):mHolder(ptr.mHolder){
}
	
/**
 * Assign a pointer to this one.
 **/
template<class ResType>
nrCResourcePtr<ResType>& nrCResourcePtr<ResType>::operator=(const nrCResourcePtr<ResType>& resPtr){
	mHolder = resPtr.mHolder;
	return *this;
}


/**
 * Access to the resource to which one this pointer points. This access need
 * 1 static_cast, 3 function calls until it returns the resource.
 * So this function is running in O(1) and is pretty efficient
 **/
template<class ResType>
ResType* nrCResourcePtr<ResType>::operator->() const{
	return static_cast<ResType*>(mHolder->getResource());
}
	
/**
 * Access to the resource to which one this pointer points. This access need
 * 1 static_cast, 3 function calls until it returns the resource.
 * So this function is running in O(1) and is pretty efficient
 **/
template<class ResType>
ResType& nrCResourcePtr<ResType>::operator*() const{
	return *(static_cast<ResType*>(mHolder->getResource()));
}


/**
 * Check if the given pointer is the same as this one.
 * @note Two pointers are the same, if they showing to the same resource.
 * 		 They also the same if their holders are the same.
 *
 **/
template<class ResType>
bool nrCResourcePtr<ResType>::operator==(nrCResourcePtr<ResType>& res) const{
	// check for holders
	nrCResourceHolder* A = mHolder.get();
	nrCResourceHolder* B = res.mHolder.get();
	if (A == B) return true;

	// At this point, both holders cannot be NULL
	if (A->getResource() == B->getResource()) return true;
	
	return false;
}

/**
 * Check if two pointers are not the same
 **/
template<class ResType>
bool nrCResourcePtr<ResType>::operator!=(nrCResourcePtr<ResType>& res) const {
	return !operator==(res);
}

/**
 * Check whenever this pointer is NULL
 **/
template<class ResType>
bool nrCResourcePtr<ResType>::isNull()const{
	return mHolder.get() == NULL;
}

