/* CVS $Id: CGuiTask.cpp,v 1.3 2005/06/12 22:32:29 psyborg Exp $ */

/** @file
 *  Main GUI task and manager for Stunts 2005.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.3 $
 *  @date    CVS $Date: 2005/06/12 22:32:29 $
 */


#include "CGuiTask.h"
using std::istream;
using std::map;
using std::string;


/** Destructor. Clean up heap objects. */
CGuiTask::~CGuiTask() {
	map<string, CGuiPage*>::iterator i;
	for ( i = mPages.begin(); i != mPages.end(); i++ )
		delete ( i->second );
}


/** Add new GUI page. */
bool CGuiTask::addPage( string PageName, string Source ) {

	CGuiParser Parser( mContext );
	CGuiPage *NewPage = Parser.ParsePage( PageName, Source );

	/* delete old page if neccessary */
	if ( mPages.find( PageName ) != mPages.end() ) {
		delete mPages[ PageName ];
		mPages.erase( PageName );
	}

	mPages[ PageName ] = NewPage;
	
	return true;
}


/** Select different GUI page. */
bool CGuiTask::selectPage( string PageName ) {
	if ( mPages.find( PageName ) == mPages.end() )
		return false;

	mCurrent = mPages[ PageName ];
	mCurrent->pageSelect(); /* Inform page that is has been selected. */

	return true;
}


/** Delete GUI page. */
bool CGuiTask::deletePage( string PageName, bool Force ) {
	if ( mPages.find( PageName ) == mPages.end() )
		return false;

	if ( mPages[ PageName ] == mCurrent ) {
		if ( Force ) {
			mCurrent = NULL;
		} else {
			return false;
		}
	}

	delete mPages[ PageName ];
	mPages.erase( PageName );

	return true;	
}


/** Actions upon initialization of task by engine. */
nrResult CGuiTask::taskInit() {
	mActive = false; /* deactivate GUI manager by default */
	mCurrent = NULL;
	return NR_OK;
}


/** Actions upon starting of task by engine. */
nrResult CGuiTask::taskStart() {
	return NR_OK;
}


/** Actions performed in every frame when GUI is active.
 *  This method activates the currently selected GUI page if the GUI manager
 *  is active.
 */
nrResult CGuiTask::taskUpdate() {

	if( mActive && mCurrent != NULL )
		mCurrent->pageUpdate();
	
	return NR_OK;
}


/** Actions upon stopping of task by engine. */
nrResult CGuiTask::taskStop() {
	mActive = false;
	return NR_OK;
}
