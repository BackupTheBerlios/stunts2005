/* CVS $Id: CGuiParser.cpp,v 1.1 2005/06/09 15:35:48 elowar Exp $ */

/** @file
 *  (Dummy) GUI parser for Stunts 2005.
 *
 *  @todo  Finish actual parser!
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.1 $
 *  @date    CVS $Date: 2005/06/09 15:35:48 $
 */


#include "CGuiParser.h"
using std::string;
using std::map;


CGuiPage* CGuiParser::ParsePage( string PageName, string Data ) {

	CGuiPage* DummyPage = new CGuiPage( mContext, PageName );

	DummyPage->mMaxId = 0;

	// IMPORTANT NOTE:
	// Please note that default (and for buttons highlight) graphics must ALWAYS
	// be generated and inserted. Otherwise you'll be getting seg faults.

	// background image; should probably create seperate object (besides
	// CGuiMulti) here for faster execution when there is only one image.

	CGuiMulti* Background = new CGuiMulti( mContext );

	Background->mGfx[ "default" ] =
		new CGuiTex( mContext, "media/graphics/gui/GuiBackground.png" );
	Background->mPos[0] = 0; Background->mPos[1] = 0;

	// start button

	CGuiButton* Start = new CGuiButton( mContext, 1 );
	DummyPage->mMaxId++;

	Start->mGfx[ "default" ] =
		new CGuiTex( mContext, "media/graphics/gui/GuiStart.png" );
	Start->mGfx[ "highlight" ] =
		new CGuiTex( mContext, "media/graphics/gui/GuiStartHL.png" );
	Start->mActions.push_back( new CGuiStartGame( mContext ) );
	Start->mPos[0] = 100; Start->mPos[1] = 360;

	// exit button

	CGuiButton* Exit = new CGuiButton( mContext, 2 );
	DummyPage->mMaxId++;

	Exit->mGfx[ "default" ] =
		new CGuiTex( mContext, "media/graphics/gui/GuiExit.png" );
	Exit->mGfx[ "highlight" ] =
		new CGuiTex( mContext, "media/graphics/gui/GuiExitHL.png" );
	Exit->mActions.push_back( new CGuiExitGui( mContext ) );
	Exit->mPos[0] = 100; Exit->mPos[1] = 450;

	// insert objects into page

	DummyPage->mObjects.push_back( Background );
	DummyPage->mObjects.push_back( Start );
	DummyPage->mObjects.push_back( Exit );

	return DummyPage;
}


/* Sub-parser for actions. */
CGuiAction* CGuiParser::ParseAction( XMLNode Node ) {
	return NULL;
}


/* Sub-parser for objects. */
CGuiObject* CGuiParser::ParseObject( XMLNode Node ) {
	return NULL;
}


/* Sub-parser for graphics. */
CGuiGfx* CGuiParser::ParseGfx( XMLNode Node ) {
	return NULL;
}
