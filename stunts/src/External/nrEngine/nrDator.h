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


#ifndef _NRCDATOR_H_
#define _NRCDATOR_H_

/**-------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include <string>
#include <list>
#include "nrTypes.h"
#include <boost/lexical_cast.hpp>

using std::string;
using std::list;
using std::find;
using boost::lexical_cast;

/**
 * nrIDator - is an interface for our templated class to hold all kind of data. 
 * Dator is the same like Functor for functions. Dator can store data with
 * different types (int, float, multiple value, ...). New values can be assigned
 * through the same dator object or through string values (useful for scripting)
 **/
class nrIDator{
public:
	
	// Operators to strings
	virtual nrIDator &operator =(string &s) = 0;
	virtual nrIDator &operator +=(string &s) = 0;
	virtual nrIDator &operator -=(string &s) = 0;
	virtual bool operator ==(string &s) = 0;
	virtual bool operator !=(string &s) = 0;

	// other functions
	virtual bool hasMultipleValues() = 0;
	virtual operator string() = 0;
	virtual operator int() = 0;
	virtual operator bool() = 0;
	virtual operator float() = 0;
	virtual const char* str() = 0;
	
};



/**
 * nrCDator - is a derived class from nrIDator representing storage for all kind
 * of data. This class is templated so you can use it to store data of any type.
 * To convert stored data to string @a boost::lexical_cast<string>(T) is used.
 * Some of functions are defined as virtual, so you can override this
 * to implement dator for non standardt types   
 **/
template<class T>
class nrCDator : public nrIDator{
protected:
	
	// Value/Target to be stored in
	T& target;
	
	/**
	* Convert given string to value of type T
	**/
	virtual T toVal(string &s){
		return lexical_cast<T>(s);
	}
	
	/**
	* Convert given value to a string
	**/
	virtual string toString(T &val){
		return lexical_cast<string>(val);
	}
	
public:
	// constructor
	nrCDator(T& t) : target(t) {}
	nrCDator(const nrIDator &s) {target = toVal(string(s));} 
	
	// access data methods
	T get(){return target;}
	
	//operator T() {return target;}
	operator string() 	{ return toString(target); }
	const char* str() 	{ return toString(target).c_str();}
	virtual operator int() 		{ return static_cast<int>(target); }
	virtual operator bool() 	{ return static_cast<bool>(target); }
	virtual operator float() 	{ return static_cast<float>(target); }
	
	// operators to handle with strings
	nrIDator &operator =(string &s) 	{target=toVal(s); return *this; }
	nrIDator &operator +=(string &s) 	{ target+=toVal(s); return *this; }
	nrIDator &operator -=(string &s) 	{ target-=toVal(s); return *this; }
	bool operator ==(string &s) 		{ return (s==(string)(*this)); }
	bool operator !=(string &s) 		{ return (s!=(string)(*this)); }

	// operators to other nrIDator objects
	nrIDator &operator =(const T &s)	{target=s; return *this;}
	nrIDator &operator +=(const T &s)	{target+=s; return *this;}
	nrIDator &operator -=(const T &s)	{target-=s; return *this;}
	bool operator ==(const T &s)		{return (target == s);}
	bool operator !=(const T &s)		{return (target != s);}
	
	// other methods
	bool hasMultipleValues() 			{ return false; }

};


/**
 * nrCDatorList - same like @a nrCDator but it can contain a multiple data type.
 * That means you can store more elements of the same type in a one Dator.
 * This can be used to implement Array/Multiple Set like behaivor for scripting.
 **/
template<class T>
class nrCDatorList : public nrIDator{
protected:
	
	// Value/Target to be stored in
	list<T>& values;
	virtual T toVal(string &s){
		return lexical_cast<T>(s);
	}
	
	virtual string toString(T &val){
		return lexical_cast<string>(val);
	}
	
public:
	// constructor
	nrCDatorList(list<T>& t) : values(t) {}
	nrCDatorList(const nrIDator &s) {values.clear();values.push_back(toVal(string(s)));} 
	
	// access data methods
	T get(){return values.back();}
	operator list<T>&() { return values; }
	
	// access data methods
	operator string() 	{ return toString(values.back()); }
	const char* str() 	{ return toString(values.back()).c_str();}
	virtual operator int() 		{ return static_cast<int>(values.back()); }
	virtual operator bool() 	{ return static_cast<bool>(values.back()); }
	virtual operator float() 	{ return static_cast<float>(values.back()); }
			
	// operators to handle with strings
	nrIDator &operator =(string &s) 	{values.clear(); values.push_back(toVal(s)); return *this; }
	nrIDator &operator +=(string &s) 	{ values.push_back(toVal(s)); return *this; }
	nrIDator &operator -=(string &s) 	{ values.remove(toVal(s)); return *this; }
	bool operator ==(string &s) 		{ return (find(values.begin(),values.end(),toVal(s))!=values.end());}
	bool operator !=(string &s) 		{  return !((*this)==s); }
		
	// operators to other nrIDator objects
	nrIDator &operator =(const T &s)	{(*this)=toString(s); return *this;}
	nrIDator &operator +=(const T &s)	{(*this)+=toString(s); return *this;}
	nrIDator &operator -=(const T &s)	{(*this)-=toString(s); return *this;}
	bool operator ==(const T &s)		{return ((*this) == toString(s));}
	bool operator !=(const T &s)		{return ((*this) != toString(s));}
	
	// other methods
	bool hasMultipleValues() 			{ return true; }

};

#endif	//_NRCDATOR_H_
