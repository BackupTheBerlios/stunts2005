/* CVS $Id: CGuiPage.cpp,v 1.5 2005/07/05 23:03:11 psyborg Exp $ */

/** @file
 *  GUI Page container for Stunts 2005.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.5 $
 *  @date    CVS $Date: 2005/07/05 23:03:11 $
 */


#include "CGuiPage.h"
using std::string;
using std::vector;
using std::map;


#include <CEGUI/CEGUI.h>
#include <OGRE/OgreCEGUIRenderer.h>

#include <OGRE/Ogre.h>

namespace stunts{
	
	/** Constructor initializing render context and page name. */
	CGuiPage::CGuiPage( int32 Context, string PageName ) {
		mContext = Context;
		mPageName = PageName;
		mMaxId = 0;
		mFocus = 1;
	
		// do in try catch block to prevent loading exceptions
		try{
			mGUISheet = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)(PageName + ".layout").c_str());
		}catch (...){
			nrLog.Log(NR_LOG_APP, "CGuiPage: The specified page was not found %s. Please check whenever such a page file exists.", PageName.c_str());
			mGUISheet = CEGUI::WindowManager::getSingleton().createWindow
					((CEGUI::utf8*)"DefaultGUISheet",(CEGUI::utf8*)(PageName + "__No_Name").c_str());
		}

		std::string name = "Stunts/Gui/";
		name += PageName;
		
		mBgOverlay = Ogre::OverlayManager::getSingleton().getByName(name.c_str());
	}
	
	
	/** Destructor. Clean up heap objects. */
	CGuiPage::~CGuiPage() {

		// now cleanup data
		if (mBgOverlay)
			Ogre::OverlayManager::getSingleton().destroy(mBgOverlay);

		
		vector<CGuiObject*>::iterator i;
		for( i = mObjects.begin(); i != mObjects.end(); i++ )
			delete (*i);
	
		// delete the sheet
		try{
			CEGUI::WindowManager::getSingleton().destroyWindow(mGUISheet);
		}catch(...){
			nrLog.Log(NR_LOG_APP, "CGuiPage: Could not destroy the page sheet");
		}
	}
	
	
	/** Actions upon selection of page by task manager. */
	void CGuiPage::pageSelect() {
		mFocus = 1; // reset focus
		if (mGUISheet){
			mGUISheet->activate();
			mGUISheet->show();
		//	if (mBgOverlay)mBgOverlay->show();
		}
	}
	
	void CGuiPage::pageDeactivate(){
		mFocus = 0;
		if (mGUISheet){
			mGUISheet->deactivate();
			mGUISheet->hide();
			if (mBgOverlay)mBgOverlay->hide();
		}
	}
	
	/** Draw page and process input.
	*  This function draws the current page and decides which object obtains focus
	*  based on the current mouse position (if the mouse has been moved since the
	*  last frame) or the state of the cursor keys.
	*/
	void CGuiPage::pageUpdate() {
	#if 0
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
	#endif
	
	} /* end of void CGuiTask::pageUpdate() */

};

