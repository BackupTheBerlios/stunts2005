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


/**
 * Includes
 **/
#include "nrCKernel.h"
#include "nrEngine.h"

/**
 * Constructor - clear all lists and initialize internal variables
 **/
nrCKernel::nrCKernel(){
	taskList.clear();
	pausedTaskList.clear();
	
	lastTaskID = 0;
	bTaskStarted = false;
}


/**
 * Destructor
 */
nrCKernel::~nrCKernel(){
	taskList.clear();
	pausedTaskList.clear();
}


/**
 * Executes all task according to their priorities. After that
 * return back.
 **/
nrResult nrCKernel::OneTick(){

	// start tasks if their are not started before
	if (!bTaskStarted)
		startTasks();

	// get iterator through our list
	list< shared_ptr<nrITask> >::iterator it, thisIt;
	nrTaskID tempID;

	// scan the list and update tasks
	it = taskList.begin();
	while (it != taskList.end()){

		shared_ptr<nrITask>& t=(*it);

		// update the task
		if (t.get()){
			if (!t->_taskCanKill && t->_taskState == NR_TASK_RUNNING){
				t->taskUpdate();
			}
		}

		it ++;

	}

	//loop again to remove dead tasks
	for( it = taskList.begin(); it != taskList.end();){

		shared_ptr<nrITask> &t=(*it);
		thisIt = it;
		it ++;

		// check if task is valid
		if (t.get()){
			// kill task if we need this
			if(t->_taskCanKill){
				nrLog.Log(NR_LOG_KERNEL, "Stop task %s (id=%d) before removing",t->taskGetName(), t->_taskID);
				t->taskStop();tempID = t->_taskID;
				taskList.erase(thisIt);
				nrLog.Log(NR_LOG_KERNEL, "Task (id=%d) removed", tempID);
			}

			// check whenver priority of the task was changed by outside
			if (t->_priorityChanged){
				ChangePriority(t->_taskID, t->_taskPriority);
			}
		}
	}

	return NR_OK;	
}

/**
 * This function will start each task and set it running state to running
 * if starting was successful otherwise the task will not run
 **/
void nrCKernel::startTasks(){
	
	// get iterator through our list
	list< shared_ptr<nrITask> >::iterator it;

	// start all tasks, which are not running at now
	for(it = taskList.begin(); it != taskList.end(); it++){
		if ((*it)->_taskState == NR_TASK_STOPPED){
			nrLog.Log(NR_LOG_KERNEL, "Start task %s with id=%d", (*it)->taskGetName(), (*it)->_taskID);

			// start the task, if can not start so not add this task to the list
			if( (*it)->taskStart() != NR_OK){
				nrLog.Log(NR_LOG_KERNEL, "Cannot start the Task, because of Task-Internal Error");
				(*it)->_taskState = NR_TASK_STOPPED;
			}else{
				(*it)->_taskState = NR_TASK_RUNNING;
			}
		}
	}

	bTaskStarted = true;
}


/**
 * Executes the kernel (old school main loop :-)
 * Before main loop is started all tasks will be intialized by calling task
 * function @a nrITask::taskStart() . If taskStart returns error, so this task will be
 * excluded from execution task list.
 * @return NR_OK or error code:
 * 		- NR_ERROR_UNKNOWN for unknown error
 **/
nrResult nrCKernel::Execute(){

	try{
		
		nrLog.Log(NR_LOG_KERNEL, "Start kernel main loop");
	
		// start tasks
		if (!bTaskStarted)
			startTasks();
		
		// loop until we have tasks in our pipeline
		while (taskList.size()){
			OneTick();
		}
		
		nrLog.Log(NR_LOG_KERNEL, "Stop kernel main loop");
		
	} catch(...){
		return NR_UNKNOWN_ERROR;
	}
	
	// ok
	return NR_OK;
	
}


/**
 * Add the given task into our kernel pipeline (main loop)
 * Before task will be added it's @a nrITask::taskInit()) function will be executed
 * @param t - is a smart pointer to an object implementing nrITask-Interface
 * @return task id of added task or NR_TASK_NO_ID if task can not be added
 **/
nrTaskID nrCKernel::AddTask (shared_ptr<nrITask> t){

	try {
		
		nrLog.Log(NR_LOG_KERNEL, "Add Task %s (addr = %p, priority = %d)", 
					t->taskGetName(), t.get(), t->_taskPriority);
		
		// check whenever such task already exists
		list<shared_ptr<nrITask> >::iterator it;
		for (it = taskList.begin(); it != taskList.end(); it++){
			if ((*it) == t){
				nrLog.Log(NR_LOG_KERNEL, "Found same task in task list !");
				return NR_TASK_NO_ID;
			}
		}
		
		for (it = pausedTaskList.begin(); it != pausedTaskList.end(); it++){
			if ((*it) == t){
				nrLog.Log(NR_LOG_KERNEL, "Found same task in paused task list !");
				return NR_TASK_NO_ID;
			}
		}
		
		// init task and check his return code
		nrLog.Log(NR_LOG_KERNEL, "Init task %s addr = %p", t->taskGetName(), t.get());
		if (t->taskInit() != NR_OK){
			nrLog.Log(NR_LOG_KERNEL, "Cannot initalize Task because of Task internal error");	
			return NR_TASK_NO_ID;
		}
		
		
		// find the place for our task according to his priority
		for( it=taskList.begin(); it != taskList.end(); it++){
			shared_ptr<nrITask> &comp = (*it);
			if (comp->_taskPriority >= t->_taskPriority) break;
		}
		
		// create new task id and add the task
		t->_taskID = ++lastTaskID;
		t->_priorityChanged = false;
		taskList.insert (it,t);
	
		nrLog.Log(NR_LOG_KERNEL, "Task %s (addr=%p) was added TaskID=%d",t->taskGetName(), t.get(), t->_taskID);
		
	} catch(...){
		return NR_UNKNOWN_ERROR;
	}

	return t->_taskID;
}


/**
 * Remove the task from our game loop (pipeline)
 * @param id - id of a task to be removed
 * @return NR_OK or error code:
 * 		- NR_ERROR_UNKNOWN for unknown error
 *		- NR_KERNEL_NO_TASK_FOUND if no such task was found
 **/
nrResult nrCKernel::RemoveTask  (nrTaskID id){

	try{
		
		nrLog.Log(NR_LOG_KERNEL, "Remove task with id=%d", id);
		
		// find the task 
		list< shared_ptr<nrITask> >::iterator it = getTaskByID(id);
		
		// check whenever iterator is valid
		if (it == taskList.end()){
			nrLog.Log(NR_LOG_KERNEL, "No such Task (id=%d) found !!!", id);
			return NR_KERNEL_NO_TASK_FOUND;
		}
	
		// say task want to remove his self
		nrLog.Log(NR_LOG_KERNEL, "Prepare to die: %s (id=%d)", (*it)->taskGetName(), (*it)->_taskID);
		(*it)->_taskCanKill = true;
	
	} catch(...){
		return NR_UNKNOWN_ERROR;
	}

	// OK
	return NR_OK;
}


/**
 * Suspend task to prevent his update. Task will get to sleep
 * @param id - id of a task to be suspended
 * @return NR_OK or error code:
 * 		- NR_ERROR_UNKNOWN for unknown error
 *		- NR_KERNEL_NO_TASK_FOUND if such a task was not found
 **/
nrResult nrCKernel::SuspendTask  (nrTaskID id){
	
	try{
		
		nrLog.Log(NR_LOG_KERNEL, "Suspend task (id=%d)", id);
		
		// find the task 
		list< shared_ptr<nrITask> >::iterator it = getTaskByID(id);
		
		// check whenever iterator is valid
		if (it == taskList.end()){
			nrLog.Log(NR_LOG_KERNEL, "No task with id=%d found", id);
			return NR_KERNEL_NO_TASK_FOUND;
		}else{
		
			// suspend task
			shared_ptr<nrITask> &t = (*it);
			t->taskOnSuspend();
			t->_taskState = NR_TASK_PAUSED;
			
			// before removing the task from task list move it to paused list
			// so we can guarantee that task object will be held in memory
			pausedTaskList.push_back(t);
			taskList.erase(it);
			
			nrLog.Log(NR_LOG_KERNEL, "Task id=%d is sleeping now", id);
		}
		
	} catch(...){
		return NR_UNKNOWN_ERROR;
	}

	// OK
	return NR_OK;
}

/**
 * Resume task after from sleeping
 * @param id - id of a task to waik
 * @return NR_OK or error code:
 * 		- NR_ERROR_UNKNOWN for unknown error
 *		- NR_KERNEL_NO_TASK_FOUND if such a task was not found
 **/
nrResult nrCKernel::ResumeTask  (nrTaskID id){

	try{ 
		
		nrLog.Log(NR_LOG_KERNEL, "Resume task (id=%d)", id);
		
		// find the task 
		list< shared_ptr<nrITask> >::iterator it = getTaskByID(id, true);
		
		// check whenever iterator is valid
		if (it == pausedTaskList.end()){
			nrLog.Log(NR_LOG_KERNEL, "No task with id=%d found", id);
			return NR_KERNEL_NO_TASK_FOUND;
		}
	
		shared_ptr<nrITask> &t = (*it);
		
		// resume the task
		t->taskOnResume();
		t->_taskState = NR_TASK_RUNNING;
		
		//keep the order of priorities straight
		for( it = taskList.begin(); it != taskList.end(); it++){
			shared_ptr<nrITask> &comp=(*it);
			if(comp->_taskPriority >= t->_taskPriority) break;
		}
		taskList.insert(it,t);
		
		// erase task from paused list. Therefor we have to find it in the list
		it = getTaskByID(id, true);
		
		// check whenever iterator is valid
		if (it != pausedTaskList.end()){
			pausedTaskList.erase(it);
		}

	} catch(...){
		return NR_UNKNOWN_ERROR;
	}

	// OK
	return NR_OK;
}

/**
 * Kill all tasks, so the game loop will stop
 * @return NR_OK or error code:
 * 		- NR_ERROR_UNKNOWN for unknown error
 **/
nrResult nrCKernel::KillAllTasks(){
	
	try{
		
		nrLog.Log(NR_LOG_KERNEL, "Kill all tasks initiated");
		
		// iterate through all tasks and kill them
		for(std::list< shared_ptr<nrITask> >::iterator it = taskList.begin();
						it != taskList.end(); it++){
			nrLog.Log(NR_LOG_KERNEL, "Prepare to die: (id=%d)", (*it)->_taskID);
			(*it)->_taskCanKill=true;
		}
		
		// iterate also through all paused tasks and kill them also
		for(std::list< shared_ptr<nrITask> >::iterator it = pausedTaskList.begin();
						it != pausedTaskList.end(); it++){
			nrLog.Log(NR_LOG_KERNEL, "Prepare to die: (id=%d)", (*it)->_taskID);
			(*it)->_taskCanKill=true;
		}
		
		// Info if we do not have any tasks to kill
		if (!taskList.size() && !pausedTaskList.size())
			nrLog.Log(NR_LOG_KERNEL, "There is no more tasks to be killed !");
		
	} catch(...){
		return NR_UNKNOWN_ERROR;
	}

	// OK
	return NR_OK;
	
}


/**
 * Changes the priority of task with given id
 * @param id  id of a task
 * @param prior  New priority for the task
 * @return NR_OK or error code:
 * 		- NR_ERROR_UNKNOWN for unknown error
 *		- NR_KERNEL_NO_TASK_FOUND if such a task was not found
 **/
nrResult nrCKernel::ChangePriority(nrTaskID id, nrPriority prior){

	try{

		nrLog.Log(NR_LOG_KERNEL, "Change Priority of task (id=%d) to %d", id, prior);
	
		// find the task 
		list< shared_ptr<nrITask> >::iterator it = getTaskByID(id);
		bool inPauseList = false;
		
		// check whenever iterator is valid
		if (it == taskList.end()){
			
			// check if we have such an trask in the paused list
			it = getTaskByID(id, true);
			if (it == pausedTaskList.end()){
				nrLog.Log(NR_LOG_KERNEL, "No task with id=%d found", id);
				return NR_KERNEL_NO_TASK_FOUND;
			}
			inPauseList = true;
		}
	
		shared_ptr<nrITask> &t = (*it);
	
		// change priority of the task
		t->_taskPriority = prior;
		t->_priorityChanged = false;
		
		// sort affected list
		if (inPauseList) pausedTaskList.sort();
		else taskList.sort();
	
	} catch(...){
		return NR_UNKNOWN_ERROR;
	}
		
	// OK
	return NR_OK;
}

/**
 * Find the task by task ID
 * @param id is the id of task
 * @param usePausedList if true we will search in paused task list
 * @return iterator to the task.
 * @note This Function runs in O(N) so optimize this if you know how
 **/
list< shared_ptr<nrITask> >::iterator nrCKernel::getTaskByID(nrTaskID id, bool usePausedList){

	// get list we want to search in
	list< shared_ptr<nrITask> > *li = NULL;
	
	if (usePausedList){
		li = &pausedTaskList;
	}else{
		li = &taskList;
	}		
		
	// iterate through elements and search for task with same id
	list< shared_ptr<nrITask> >::iterator it;
	for ( it = li->begin(); it != li->end(); it++){
		if ((*it)->_taskID == id){
			return it;
		}			
	}
	
	// not found so return end iterator
	return li->end();
	 
}
