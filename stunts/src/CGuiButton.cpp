/* CVS $Id: CGuiButton.cpp,v 1.4 2005/06/27 01:19:16 psyborg Exp $ */

/** @file
 *  Button object (CGuiObject) for Stunts 2005.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.4 $
 *  @date    CVS $Date: 2005/06/27 01:19:16 $
 */


#include "CGuiButton.h"
using std::string;
using std::vector;
using std::map;

#include "OgreTask.hpp"
namespace stunts
{
	
	/** Constructor initializing render context and ID. */
	CGuiButton::CGuiButton( int32 Context, unsigned int Id ) {
		mContext = Context;
		mId = Id;
		mPos[0] = mPos[1] = mPos[2] = 0;
	}
	
	
	/** Destructor. Clean up heap objects. */
	CGuiButton::~CGuiButton() {
		vector<CGuiAction*>::iterator i;
		for( i = mActions.begin(); i != mActions.end(); i++ )
			delete (*i);
	
		map<string, CGuiGfx*>::iterator j;
		for( j = mGfx.begin(); j != mGfx.end(); j++ );
			delete (j->second);
	}
	
	
	/** Updating and drawing. */
	void CGuiButton::objectUpdate( bool MouseMoved, unsigned int Focus ) {
	
		if( MouseMoved ) {
			glEnable(GL_STENCIL_TEST);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilFunc(GL_ALWAYS, mId, ~0); /* write own ID to stencil */
		}
	
		if( Focus == mId ) {
			mGfx[ "highlight" ] -> Draw( mPos );
		} else {
			mGfx[ "default" ] -> Draw( mPos );
		}
	
		if( MouseMoved ) {
			glDisable(GL_STENCIL_TEST);
		}
	
		/* Keyboard and mouse handling. */
		if( Focus == mId ) { 
			//if( nrFramework.keyPush( NRKEY_RETURN ) ||
			//	nrFramework.getMouseButton( NRBUTTON_LEFT ) == 1 ) {
			if (COgreTask::GetSingleton().mInputDevice->isKeyDown(Ogre::KC_RETURN) ||
				COgreTask::GetSingleton().mInputDevice->getMouseButton(0)){
				
				vector<CGuiAction*>::iterator i;
				for( i = mActions.begin(); i != mActions.end(); i++ ) {
					(*i)->Perform(); /* Activate next associated action. */
	
				}
			}
		}
	
		/* 
		* Note: for some reason nrFramework.getMouseButton contains 224 (probably
		* random number) until the first click. Maybe investigate further later.
		*/
	
	}

};
