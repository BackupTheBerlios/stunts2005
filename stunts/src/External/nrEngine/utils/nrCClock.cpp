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

#include "nrCClock.h"
#include "../nrEngine.h"

#include <boost/lexical_cast.hpp>

using boost::lexical_cast;


/*-------------------------------------------------------------------------
 * Defines
 *------------------------------------------------------------------------*/
#define DEFAULT_OBSERVER_NAME "__no_name__"


/**
 * After the object is created and before it runs the returned value 
 * for the frame rate will be 0.
 * The value for frame window is set to 7 per default.
 **/
nrCClock::nrCClock(){
	
	currentTime = 0;
	frameTime = 0;
	frameNumber = 0;
	
	sourceStartValue = 0;
	sourceLastValue = 0;
	
	useFixRate = false;
	fixFrameRate = 0;
	
	// add default frame time	
	setFrameWindow (7);
	frameTime = _getFilteredFrameDuration();
	
	timeSource.reset();
}


/**
 * Release all used memory.
 **/
nrCClock::~nrCClock(){
	timeSource.reset();
	
	for (unsigned int i=0; i < observers.size(); i++){
		observers[i].reset();
	}
	observers.clear();
	
}


/**
 * Set the time source to be used to get current time. Our clock class
 * is platform independent but the time get code depends mostly on the
 * platform you using. So just derive your time source from nrITimeSource
 * interface and add this source into our clock.
 * @param timeSource smart pointer containing to time source
 **/
void nrCClock::setTimeSource(shared_ptr<nrITimeSource> timeSource){
    
	this->timeSource = timeSource;

	if (timeSource != NULL){
		sourceStartValue = timeSource->getTime();
		sourceLastValue = sourceStartValue;
	}	

}


/**
 * Returns true if any time source is bounded to our clock.
 **/
bool nrCClock::isAnySourceBounded(){
	return (timeSource != NULL);
}


/**
 * Here you can create timers from this clock.
 **/
shared_ptr<nrCTimer> nrCClock::createTimer(){

	// create timer as observer
	shared_ptr<nrITimeObserver> timer(
				new nrCTimer(nrCClock::GetSingleton()), 
				null_deleter());
	
	// add him to the list
	int id = addObserver(timer);
	
	if (id == 0){
		nrLog.Log(NR_LOG_ENGINE, "nrCClock::createTimer(): Can not add timer to the observer list");
	}
	
	// set the id of the timer
	timer->_observerID = id;
	
	// return created timer
	return dynamic_pointer_cast<nrCTimer, nrITimeObserver>(timer);
	
}

	
/**
 * Updates the clock task by getting time values and calculating of 
 * frame rate and frame interval.
 * Also each connected time observer will be notified so he can do
 * his updates. This allows you just to create any observer (like timer)
 * bind them to the clock and forgett it.
 **/
nrResult nrCClock::taskUpdate(){

	// calculate exact frame duration time
	double exactFrameDuration = _getExactFrameDuration();
	_addToFrameHistory(exactFrameDuration);
	
	// get filtered frame time
	frameTime = _getFilteredFrameDuration();
	currentTime += frameTime;
	frameNumber ++;
		
	// get through the list of connected observers and notify them
	ObserverList::iterator it;
	for (it = observers.begin(); it != observers.end(); ++it){
		(*it)->notifyTimeObserver();
	}
	
	// OK			
	return NR_OK;
	
}


/**
 * Returns current time since clock start ticking.
 **/
double nrCClock::getTime() const{
	return currentTime;
}


/**
 * Returns filtered interval time between two or more frames.
 */
float nrCClock::getFrameInterval() const{
	return (float)frameTime;
}


/**
 * Returns number of frames drawn since clock start ticking
 **/
int nrCClock::getFrameNumber() const{
	return frameNumber;
}


/**
 * Returns FPS
 **/
float nrCClock::getFrameRate() const{
	return 1.0f/(float)(frameTime);
}


/**
 * Add new time observer to our list of observers. Each observer will
 * be notified on each frame after clock calculations are done. With this
 * system we can separate our timers from game loop, but they still be
 * updated on each frame.
 * @param timeObserver smart pointers showing to observer
 * @return internal ID of observer to allow of removing them from the list
 * @note Each observer has to implement the nrITimeObserver interface
 **/
int nrCClock::addObserver(shared_ptr<nrITimeObserver> timeObserver){
	
	int id = 0;
	
	if (timeObserver != NULL){
		observers.push_back (timeObserver);
		id = observers.size();
		observerIDList[string(DEFAULT_OBSERVER_NAME) 
									+ lexical_cast<string>(id)] = id;
		observers[observers.size()-1]->_observerID = id;
	}
	
	return id;
}

/**
 * Removes an observer from the list by given ID
 * @param observerID id of the observer returned by addObserver()
 * @return either NR_OK or:
 *		- NR_CLOCK_OBSERVER_NOT_FOUND if such id is not valid
 **/
nrResult nrCClock::removeObserver(int observerID){
	
	int id = observerID - 1;
	if (id <= 0 || id+1 >= static_cast<int>(observers.size())
			|| observerIDList.find(string(DEFAULT_OBSERVER_NAME) + 
						lexical_cast<string>(observerID)) == observerIDList.end()){
		return NR_CLOCK_OBSERVER_NOT_FOUND;
	}
	
	// remove it from the list
	observers.erase(observers.begin() + id);
	
	// also clean the map enty for this id
	observerIDList.erase(observerIDList.find(string(DEFAULT_OBSERVER_NAME) + 
			lexical_cast<string>(observerID) ));
			
	// OK
	return NR_OK;
}


/**
 * Add an time observer by a name. So you can delete them also by using
 * this name.
 * @param obsName name of the observer
 * @param timeObserver smart pointer pointing to the observer
 * @return either NR_OK or:
 *			- NR_CLOCK_OBSERVER_ALREADY_ADDED if such is already added
 **/
nrResult nrCClock::addObserver(const string& obsName, 
											shared_ptr<nrITimeObserver> timeObserver){
							

	// check whenever such an observer already exists
	if (observerIDList[obsName]){
		return NR_CLOCK_OBSERVER_ALREADY_ADDED;
	}
	
	// add observer	
	if (timeObserver != NULL && obsName.length() > 0){
		observers.push_back (timeObserver);
		observerIDList[obsName] = observers.size(); 
		observers[observers.size()-1]->_observerID = observers.size();
	}
	
	return NR_OK;
				
}


/**
 * Removes an observer by using his name.
 * @param obsName name of the observer
 * @return either NR_OK or:
 *			- NR_CLOCK_OBSERVER_NOT_FOUND if such an observer not exists
 **/
nrResult nrCClock::removeObserver(const string& obsName){

	if (observerIDList[obsName]){
		int id = observerIDList[obsName] - 1;
		observers.erase(observers.begin() + id);
	}else{
		return NR_CLOCK_OBSERVER_NOT_FOUND;
	}
	
	return NR_OK;
}


/**
 * Set the count of frames to be computed to calculate frame rate/interval.
 * We need this to smooth big peaks on frame rates to allow application
 * run smoother than without this window. Here we just using averaging
 * of the frame intervals during certain number of frames.
 * @param frameCount size of such an frame calculation window
 * @param defaultFrameTime is a frame interval added by default as if there
 *		was a frame rendered with this speed. This allows us to start smooth
 **/
void nrCClock::setFrameWindow(int frameCount, float defaultFrameTime){
	
	frameFilteringWindow = frameCount > 1 ? frameCount : 1;
	frameDefaultTime = defaultFrameTime;
	frameDurationHistory.clear ();
	frameDurationHistory.push_back(frameDefaultTime);
}


/**
 * If you set this to true so fix frame rate will be used. This helps us 
 * to run our application on console or do internal calculations based
 * on fix frame rate like Doom3 does.
 * @param setFixRate if true fix frame rate will be used.
 * @param fixFrameRate frame rate to be used
 **/
void nrCClock::setFixFrameRate(bool setFixRate, float fixFrameRate){
	useFixRate = setFixRate;
	this->fixFrameRate = fixFrameRate;
}



double nrCClock::_getExactFrameDuration (){
	
	if (useFixRate || timeSource == NULL){
		return 1.0f/(fixFrameRate);
	}
	
	double sourceTime = timeSource->getTime();
	double frameDuration = sourceTime - sourceLastValue;
	
	if (frameDuration > 0.200){
		frameDuration = frameDurationHistory.back();
	}else if (frameDuration < 0){
		frameDuration = 0;
	}
	
	sourceLastValue = sourceTime;
	
	return frameDuration;
	
}


void nrCClock::_addToFrameHistory (double exactFrameDuration){
	frameDurationHistory.push_back (exactFrameDuration);
	if (frameDurationHistory.size () > (unsigned int) frameFilteringWindow)
		frameDurationHistory.pop_front ();
}


double nrCClock::_getFilteredFrameDuration () const{
	
	if (useFixRate){
		return 1.0f/(fixFrameRate);
	}
	
	double totalFrameTime = 0;

	deque<double>::const_iterator it;
	for (it=frameDurationHistory.begin();it != frameDurationHistory.end(); ++it){
		totalFrameTime += *it;
	}
	
	return totalFrameTime/frameDurationHistory.size();
}
