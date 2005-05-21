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

/*!
 * \defgroup gp General purpose
 *
 * General purpose are classes or interfaces which can be used for a lot of
 * things. There is singleton implementation or dator interface. Just
 * read description of each class separatly you can find here.
 */

#ifndef _NR_I_SINGLETON_H_
#define _NR_I_SINGLETON_H_


#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include "nrTypes.h"

using boost::scoped_ptr;
using boost::shared_ptr;
using boost::static_pointer_cast;
using boost::dynamic_pointer_cast;


/**
 * This is an null_deleter - deleter. We need this to create smart pointer 
 * (shared_ptr<T>) from our singleton objects. Each pointer created with null 
 * deleter can not be released otherwhere else here. So we can give smart pointer 
 * to our singleton back and we can be sure, that our singleton object will not 
 * be deleted before Release-Function will be executed
 * @see boost.org for smart pointers
 * \ingroup gp
 **/
struct null_deleter{
    void operator()(void const *) const
    {
    }
};


struct nr_empty{

};

/**
 * nrISingleton class is used to define an object as an only one possible. 
 * That means if you try to create new instance of this class there will be an error. 
 * This is very usefull for objects like Kernel, FileSystem or something similar. 
 * @note Thanks to: by Richard "superpig" Fine who get this from Scott Bilas (Game
 * Programming Gems) showing this technique in his tutorial about Enginuity.
 * \ingroup gp
 **/
template<class Obj>
class nrISingleton{
	
protected:
	
	nrISingleton(){
	}
	
	virtual ~nrISingleton(){
	}

	static scoped_ptr<Obj> _ms_singleton;
	
public:


	
	/**
	* isValid() - returns true if singleton object exists
	**/
	static inline bool isValid(){
		return (bool)(_ms_singleton.get());
	}

	/**
	* GetSmartPtr() - returns smart pointer to our singleton
	**/
	static inline const shared_ptr<Obj> GetSmartPtr(){
		shared_ptr<Obj> _ptr(GetSingletonPtr(), null_deleter());
		return _ptr;
	}
	
	/**
	* GetSingleton() - returns reference to singleton instance
	**/
	static inline Obj& GetSingleton(){
		ASSERT(_ms_singleton.get() && "No Singleton Object created until now");
		return *_ms_singleton;			
	}

	/**
	* GetSingletonPtr() - returns pointer to singleton instance
	**/
	static inline Obj* GetSingletonPtr(){
		ASSERT(_ms_singleton.get() && "No Singleton Object created until now");
		return _ms_singleton.get();			
	}
	
	/**
	 * Access instance directly through -> operator. Can be overloaded
	 **/
	virtual Obj* operator->(){
		return GetSingletonPtr();
	}
	
	/**
	* Create the singleton instance of an object of type <i>Obj</i>
	**/
	static inline void Instantiate(){
		ASSERT(_ms_singleton.get()==NULL && "Singleton Object already created");
		_ms_singleton.reset (new Obj());
	}


	/**
	* Create the singleton instance of an object of type <i>Derived</i> which is
	* derived class of the <i>Obj</i> Base instance.
	* You ask here: Why we do it in this way?
	* So the answer is: You can so access to your base object. But it was
	* created from subclass. Usefull for example for objects which's base has
	* pure virtual functions.	
	*/
	template<class Derived>
	static inline void InstantiateAs(){
		ASSERT(_ms_singleton.get()==NULL && "Singleton Object already created");
		_ms_singleton.reset (new Derived());
	}
	
#if 0
	/**
	* Create the singleton instance of an object of type <i>Derived</i> which can be
	* derived class of the <i>Obj</i> Base instance.
	* You ask here: Why we do it in this way?
	* So the answer is: You can so access to your base object, but this one was
	* created from derived. Usefull for example for objects which's base has
	* pure virtual functions.	
	**/
	static inline void InstantiateDerived(){
		ASSERT(_ms_singleton.get()==NULL && "Singleton Object already created");
		_ms_singleton.reset (new Derived());
	}
#endif

	/**
	* Release() - release the instance and free memory
	**/
	static inline void Release(){
		ASSERT(_ms_singleton.get()!=NULL && "No Singleton Object created until now");
		_ms_singleton.reset();
	}
	
};

template <class Obj>
scoped_ptr<Obj> nrISingleton<Obj>::_ms_singleton(0);


#if 0
/**
 * Same as nrISingleton but used to have singleton object of a base class created from derived class.
 * So you can create singleton object of any class. But by referencing the base class will be used.
 * @note BaseClass should be the base of Obj
 * \ingroup gp
 **/
template<class BaseClass, class Obj>
class nrIBasisSingleton{	
protected:
	
	nrIBasisSingleton(){
	}
	
	virtual ~nrIBasisSingleton(){
	}

	static scoped_ptr<BaseClass> _ms_singleton;
	
public:


	
	/**
	* isValid() - returns true if singleton object exists
	**/
	static inline bool isValid(){
		return (bool)(_ms_singleton.get());
	}

	/**
	* GetSmartPtr() - returns smart pointer to our singleton
	**/
	static inline const shared_ptr<BaseClass> GetSmartPtr(){
		shared_ptr<BaseClass> _ptr(GetSingletonPtr(), null_deleter());
		return _ptr;
	}
	
	/**
	* GetSingleton() - returns reference to singleton instance
	**/
	static inline BaseClass& GetSingleton(){
		ASSERT(_ms_singleton.get() && "No Singleton Object created until now");
		return *_ms_singleton;			
	}

	/**
	* GetSingletonPtr() - returns pointer to singleton instance
	**/
	static inline BaseClass* GetSingletonPtr(){
		ASSERT(_ms_singleton.get() && "No Singleton Object created until now");
		return _ms_singleton.get();			
	}
	
	/**
	 * Access instance directly through -> operator. Can be overloaded
	 **/
	virtual BaseClass* operator->(){
		return GetSingletonPtr();
	}
	
	/**
	* Instantiate() - create the singleton instance
	**/
	static inline void Instantiate(){
		ASSERT(_ms_singleton.get()==NULL && "Singleton Object already created");
		_ms_singleton.reset (new Obj());	
	}

	/**
	* Release() - release the instance and free memory
	**/
	static inline void Release(){
		ASSERT(_ms_singleton.get()!=NULL && "No Singleton Object created until now");
		_ms_singleton.reset();
	}

};

template<class BaseClass, class Obj>
scoped_ptr<BaseClass> nrIBasisSingleton<BaseClass,Obj>::_ms_singleton(0);
#endif


#endif	//_NRCSINGLETON_H_
