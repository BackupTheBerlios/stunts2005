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
 * \defgroup kernel Kernel (Heart) of the nrEngine
 *
 * As you know each operating system has got a kernel. This kernel run tasks
 * (user programms) to allow to do a lot of things. So our engine is nothing
 * else as a little operating system. Here we implement kernel of the engine.
 * This kernel runs tasks in there priority order, so you can run for example
 * display task as last task and input as first. Kernel has also support to freeze
 * tasks (suspend) and resume them from sleeping. To run application written
 * for nrEngine it is better if you use this system. Actually kernel is nothing else
 * than the old style game loop. Kernel runs tasks in a one big loop until there
 * is no more tasks. 
 * \author Art Tevs <tevs@mpi-sb.mpg.de>
 */

#ifndef _NR_C_KERNEL_H_
#define _NR_C_KERNEL_H_


/**
 * Includes
 **/
#include "nrISingleton.h"
#include "nrITask.h"
#include "nrTypes.h"
#include <boost/shared_ptr.hpp>
#include <list>

using boost::shared_ptr;
using std::list;


/**
 * nrCKernel - is the heart of our engine. Kernel is something like the oldschool
 * Game Loop. You can add, remove, suspend or resume tasks from kernel's pipeline
 * Tasks would be executed in their priority order smaller priority comes before
 * greater priority number. 
 * What should tasks supports:
 *	- They should implement the @a nrITask Interface
 *	- They should be loaded and be ready to start before adding to kernels pipeline
 *	- In each game cycle @a nrITask::taskUpdate() Method will be executed
 *	- By suspending or resuming the Tasks @a nrITask::taskOnSuspend()/taskOnResume() Method will beexecuted
 *	- Before task will be removed @a nrITask::taskStop() Method will executed
 *	- Each TaskID is unique and is greater than 0
 *	- There is no TaskID wich will be given twice
 *
 * NOTE: Try to optimise this kernel by implementing O(1) Scheduler or something
 *		 else if kernel loop/pipeline need a lot of time for updating.
 *		 Better if you implement supporting of multithreaded Kernel, so we can 
 *		 support Multiple-CPU-System to increase game speed
 * \ingroup kernel
 **/
class nrCKernel : public nrISingleton<nrCKernel>{
public:
	
	// constructor & destructor
	nrCKernel();
	virtual ~nrCKernel();

	// execute the kernel
	nrResult Execute();
	nrResult OneTick();
	
	// methods to add/remove tasks
	nrTaskID AddTask (shared_ptr<nrITask> t);
	nrResult RemoveTask  (nrTaskID id);
	
	// pausing the task and resuming it
	nrResult SuspendTask (nrTaskID id);
	nrResult ResumeTask  (nrTaskID id);

	// other methods for tasking
	nrResult KillAllTasks();
	nrResult ChangePriority(nrTaskID id, nrPriority prior = NR_PRIORITY_NORMAL);

	// get tasks list to allow statistics
	const list< shared_ptr<nrITask> >& getTaskList(){return taskList;}
	const list< shared_ptr<nrITask> >& getPausedTaskList(){return pausedTaskList;}
	
protected:

	// lists to store current tasks
	list< shared_ptr<nrITask> > taskList;
	list< shared_ptr<nrITask> > pausedTaskList;

private:
	// get functions
	list< shared_ptr<nrITask> >::iterator getTaskByID(nrTaskID id, bool usePausedList = false);

	// id counter
	nrTaskID lastTaskID;

	// Kernel Tasks already sorted
	bool bTaskStarted;

	// sort kernel's tasks (done by first call of execution or OneTick methods)
	void startTasks();
	
};

#endif	//_NR...
