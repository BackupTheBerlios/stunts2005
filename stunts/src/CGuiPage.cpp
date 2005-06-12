/* CVS $Id: CGuiPage.cpp,v 1.3 2005/06/12 22:32:29 psyborg Exp $ */

/** @file
 *  GUI Page container for Stunts 2005.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.3 $
 *  @date    CVS $Date: 2005/06/12 22:32:29 $
 */


#include "CGuiPage.h"
using std::string;
using std::vector;
using std::map;

#include "OgreTask.hpp"
using namespace stunts;

/** Constructor initializing render context and page name. */
CGuiPage::CGuiPage( int32 Context, string PageName ) {
	mContext = Context;
	mPageName = PageName;
	mMaxId = 0;
	mFocus = 1;
}


/** Destructor. Clean up heap objects. */
CGuiPage::~CGuiPage() {
	vector<CGuiObject*>::iterator i;
	for( i = mObjects.begin(); i != mObjects.end(); i++ )
		delete (*i);
}


/** Actions upon selection of page by task manager. */
void CGuiPage::pageSelect() {
	mFocus = 1; // reset focus
}


/** Draw page and process input.
 *  This function draws the current page and decides which object obtains focus
 *  based on the current mouse position (if the mouse has been moved since the
 *  last frame) or the state of the cursor keys.
 */
void CGuiPage::pageUpdate() {

	/* Cursor key pressed. */
 	/*if( nrFramework.keyPush( NRKEY_DOWN ) ||
		nrFramework.keyPush( NRKEY_RIGHT ) ) {
		if( mFocus >= mMaxId ) {
			mFocus = 1;
		} else {
			mFocus++;
		}
	}

	if( nrFramework.keyPush( NRKEY_UP ) ||
		nrFramework.keyPush( NRKEY_LEFT ) ) {
		if( mFocus <= 1 ) {
			mFocus = mMaxId;
		} else {
			mFocus--;
		}
	}*/

	
	/* Mouse motion detection. */
	static int32 LastMousePos[2] = { 0, 0 }; /* remember in next call */
	int32 MousePos[2] = { 0, 0 };
	//nrFramework.getCursorPosition( &MousePos[0], &MousePos[1] );
	MousePos[0] = COgreTask::GetSingleton().mInputDevice->getMouseAbsX();
	MousePos[1] = COgreTask::GetSingleton().mInputDevice->getMouseAbsY();
	
	bool MouseMoved =
		MousePos[0] != LastMousePos[0] || MousePos[1] != LastMousePos[1];

	if ( MouseMoved ) {
		LastMousePos[0] = MousePos[0];
		LastMousePos[1] = MousePos[1];
		glClearStencil(0);
		glClear( GL_STENCIL_BUFFER_BIT );
	}

	/* All other input will be handled by the objects. */

	/* Activate (draw) all objects. */
	for( unsigned int i = 0; i < mObjects.size(); i++ ) {
		mObjects[i]->objectUpdate( MouseMoved, mFocus );
	}

	/* Detect mouse collision with any object. */
	if ( MouseMoved ) {
		GLint View[4];
		glGetIntegerv( GL_VIEWPORT, View );
		GLfloat StencilValue = 0;

		glFinish();
		glReadBuffer(GL_BACK);
		glReadPixels(MousePos[0], View[3]-MousePos[1], 1, 1,
			GL_STENCIL_INDEX, GL_FLOAT, &StencilValue);

		if (StencilValue != 0)
			mFocus = (unsigned int)StencilValue;
	}

} /* end of void CGuiTask::pageUpdate() */
