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


#ifndef _NR_I_TASK_H_
#define _NR_I_TASK_H_


/**-----------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------------*/
#include "nrTypes.h"


/**-----------------------------------------------------------------------------
 * Define types
 *----------------------------------------------------------------------------*/
typedef unsigned int  nrPriority;
typedef unsigned long nrTaskID;

#define NR_TASK_NO_ID		0


/**
 * Each task can be in one of this states. It can be either stopped, running or paused (sleeping)
 * \ingroup kernel
 **/
typedef enum {
	NR_TASK_STOPPED,
	NR_TASK_RUNNING,
	NR_TASK_PAUSED
}nrTaskState;


 
#define NR_PRIORITY_STEP		10000
#define NR_PRIORITY_FIRST		NR_PRIORITY_STEP * 0
#define NR_PRIORITY_ULTRA_HIGH	NR_PRIORITY_STEP * 1
#define NR_PRIORITY_VERY_HIGH	NR_PRIORITY_STEP * 2
#define NR_PRIORITY_HIGH		NR_PRIORITY_STEP * 3
#define	NR_PRIORITY_NORMAL		NR_PRIORITY_STEP * 4
#define NR_PRIORITY_LOW			NR_PRIORITY_STEP * 5
#define NR_PRIORITY_VERY_LOW	NR_PRIORITY_STEP * 6
#define NR_PRIORITY_ULTRA_LOW	NR_PRIORITY_STEP * 7
#define NR_PRIORITY_LAST		0xFFFFFFFF


/**
 * nrITask - is an interface for tasks of our kernel system. Kernel runs all
 * specified and added task in their priority order (think on OS). So nrITask
 * Interface defines functions to be implemented by each task
 *
 * There is a rule to understanding priorites:
 *	- Each Priority is a number
 *	- Bigger number smaller priority
 *	- Between each defined default Priority there is NR_PRIORITY_STEP numbers available
 *	- There is no guarantee that two or more tasks with same priority would be
 *		executed in next cycle in same order
 *	- you can increase/decrease the priority by substracting/adding of any number to any
 *		predefined priority state
 * \ingroup kernel
 **/
class nrITask{
public:
	
	/**
	 * Constructor - defines default variables. Set also default task priority to NR_PRIORITY_NORMAL
	 * and define the current state of the task to NR_TASK_STOPPED.
	 **/
	nrITask(){
		_taskCanKill = false;
		_taskPriority= NR_PRIORITY_NORMAL;
		_taskID = NR_TASK_NO_ID;
		_taskState = NR_TASK_STOPPED;
		_priorityChanged = false;
	}

	/**
	 * Operators - useful for sorting tasks.
	 * All comparisons are based on task ids.
	 **/
	bool operator < (nrITask &t){return this->_taskID < t._taskID;}
	bool operator <= (nrITask &t){return this->_taskID <= t._taskID;}
	bool operator > (nrITask &t){return this->_taskID > t._taskID;}
	bool operator >= (nrITask &t){return this->_taskID >= t._taskID;}
	
	bool operator == (nrITask &t){return this == &t;}
	bool operator != (nrITask &t){return this != &t;}
		
	/**
	 * Initialization of a task. 
	 **/
	virtual nrResult taskInit()=0;
	
		
	/**
	 * Here task will be started. 
	 **/
	virtual nrResult taskStart()=0;
	
	
	/**
	 * In each cycle of our game loop this method will be called if task was added to our kernel. 
	 **/
	virtual nrResult taskUpdate()=0;
	
	
	/**
	 * Stop the task before task will be killed. 
	 **/
	virtual nrResult taskStop()=0;
	
	
	/**
	 * taskOnResume() will be executed on waiking up the task from sleeping. 
	 **/
	virtual nrResult taskOnResume(){return NR_OK;};
	
	
	/**
	 * Kernel will call this method if task will be aked to go for sleeping. 
	 **/
	virtual nrResult taskOnSuspend(){return NR_OK;};
	
	
	/**
	 * Task should return his name. Name must not be unique. We will need this to read 
	 * Log-Files in simple way. 
	 **/
	virtual const char* taskGetName(){return "NoName";}
	

	/**
	 * Set priority for this task
	 * @param prior Priority of this task
	 **/
	void setTaskPriority(nrPriority prior){
		_taskPriority = prior;
		_priorityChanged = true;
	}
		
	/**
	 * Get priority of this task
	 * @return Priority of the task
	 **/
	nrPriority getTaskPriority(){
		return _taskPriority;
	}
	

	/**
	 * Get priority of this task
	 * @return Priority of the task
	 **/
	nrTaskID getTaskID(){
		return _taskID;
	}
	
	friend class nrCKernel;
	
protected:
	bool 		_taskCanKill;		// we can kill this task in next system cycle
	nrTaskState	_taskState;		
	nrTaskID 	_taskID;			//from kernel given task ID (unique)
	nrPriority 	_taskPriority;		// priority of our tasks

	bool _priorityChanged;
};

#endif	//_NR...
