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


#ifndef _NR_I_SINGLETON_TASK_H_
#define _NR_I_SINGLETON_TASK_H_


#include "nrISingleton.h"
#include "nrITask.h"
#include "nrCKernel.h"

/**
 * nrISingletonTask combines the interface for nrITask-class and functionality
 * of a singleton object. All derived classes will have to implement standard
 * nrITask interface for tasking. nrISingletonTask provides also couple of
 * usefull functions for each class implementing this interface.
 * \ingroup gp
 **/
template<typename T>
class nrISingletonTask : public nrISingleton<T>, public nrITask {
	
public:
	
	/**
	 * returns intance of singleton part
	 **/
	virtual T* operator->(){
		return T::GetSingletonPtr();
	}
	
	/**
	 * returns casted smart pointer to @a nrITask Interface
	 **/
	static shared_ptr<nrITask> GetTaskInterface(){
		return dynamic_pointer_cast<nrITask, T>(nrISingleton<T>::GetSmartPtr());
	}
	
	/**
	 * add task to kernel
	 * @param kernel object of nrCKernel-Interface
	 * @param prior - priority for the task
	 * @return result provided by @a nrCKernel::AddTask() method
	 **/
	virtual nrResult AddToKernel(nrCKernel& kernel, nrPriority prior = 0){
		if (prior != 0) this->_taskPriority = prior;
		return kernel.AddTask(GetTaskInterface());
	}
	
};

#endif	//_NRCSINGLETON_H_
