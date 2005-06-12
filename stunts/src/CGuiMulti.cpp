/* CVS $Id: CGuiMulti.cpp,v 1.2 2005/06/12 19:43:45 elowar Exp $ */

/** @file
 *  Variable graphics object (CGuiObject) for Stunts 2005.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.2 $
 *  @date    CVS $Date: 2005/06/12 19:43:45 $
 */


#include "CGuiMulti.h"
using std::string;
using std::map;


/** Constructor initializing render context. */
CGuiMulti::CGuiMulti( int32 Context ) {
	mContext = Context;
	mPos[0] = mPos[1] = mPos[2] = 0;
}


/** Destructor. Clean up heap objects. */
CGuiMulti::~CGuiMulti() {
	map<string, CGuiGfx*>::iterator j;
	for( j = mGfx.begin(); j != mGfx.end(); j++ );
		delete (j->second);
}


/** (Updating and) drawing. */
void CGuiMulti::objectUpdate( bool MouseMoved, unsigned int Focus ) {

	string Value;

	if( mSetting.size() == 0 )
		Value = "default";
	else
		Value = (string)nrSettings.get( mSetting );

	if( mGfx.find( Value ) != mGfx.end() ) {
		mGfx[ Value ] -> Draw( mPos );
	} else {
		mGfx[ "default" ] -> Draw( mPos );
	}

}

