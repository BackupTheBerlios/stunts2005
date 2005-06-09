/* CVS $Id: CGuiTask.h,v 1.1 2005/06/09 15:35:48 elowar Exp $ */

/** @file
 *  Main GUI task and manager for Stunts 2005.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.1 $
 *  @date    CVS $Date: 2005/06/09 15:35:48 $
 */


#ifndef __C_GUI_TASK_H
#define __C_GUI_TASK_H


#include <nrEngine/nrEngine.h>

#include "CGuiPage.h"
#include "CGuiParser.h" // parser is actually used here

#include <iostream>

#include <string>
#include <map>


/** Main GUI task and manager.
 *  This class provides functions for loading and activating GUI pages and
 *  additionally implements the nrITask interface for direct use within the 
 *  newReality game engine.
 */

class CGuiTask : public nrITask {

public:
 
	/** Constructor initializing render context.
	 *  @note Please note that all other attributes will not be initialized
	 *        until @c taskInit() is called.
	 *  @param  Context  Render context to be used by GUI.
     */
	CGuiTask( int32 Context ) { mContext = Context; }

	/** Destructor. */
	virtual ~CGuiTask();

	/** Add new GUI page.
	 *  @param  PageName  Name by which new page will be accessible.
	 *  @param  Source    XML string data to generate page from.
	 *  @return @c true if page was successfully loaded, otherwise @c false
	 */
	bool addPage( std::string PageName, std::string Source );

	/** Select different GUI page.
	 *  @param  PageName  Name of page to be selected.
	 *  @return @c true if page existed, otherwise @c false. 
     */
	bool selectPage( std::string PageName );

	/** Delete GUI page.
	 *  @param  PageName  Name of page to be deleted.
	 *  @param  Force     If @c true, delete even if page is active.
	 *  @return @c true if page was deleted, otherwise @c false.
	 */
	bool deletePage( std::string PageName, bool Force = false );
			
	/* The following methods are required by nrITask interface. */
	nrResult taskInit();
	nrResult taskStart();
	nrResult taskUpdate();
	nrResult taskStop();
  	const char* taskGetName() { return "CGuiTask"; }

	/* The following methods provide access to values of and/or references to
     * private class attributes where suitable. */
	bool &rActive() { return mActive; }
	bool   Active() { return mActive; }

private:

	bool mActive; /**< GUI manager activated flag */

	std::map<std::string, CGuiPage*> mPages; /**< Pages accessible by name */
	CGuiPage* mCurrent;                      /**< currently selected GUI page */

	int32 mContext; /**< render context handle */

};


#endif

