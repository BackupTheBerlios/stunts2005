/* CVS $Id: CGuiParser.h,v 1.4 2005/06/25 00:59:10 psyborg Exp $ */

/** @file
 *  GUI parser for Stunts 2005.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.4 $
 *  @date    CVS $Date: 2005/06/25 00:59:10 $
 */


#ifndef __C_GUI_PARSER_H
#define __C_GUI_PARSER_H


#include <string>
#include <map>

#include "CGuiPage.h"
#include "GuiActions.h"
#include "GuiObjects.h"
#include "GuiGfx.h"

#include <OGRE/Ogre.h>

#if 0
class CGuiOgreElementFactory : public Ogre::OverlayElementFactory
{
public:
	Ogre::OverlayElement* createOverlayElement (const Ogre::String &instanceName)
	{
		if (instanceName == "button")
			return new CGuiButton;
		else if (instanceName == "multi")
			return new CGuiMulti;
		else
			return NULL;
	}
	
	void destroyOverlayElement (Ogre::OverlayElement *pElement)
	{
		delete pElement;
	}

	const Ogre::String & getTypeName (void) const
	{
		return Ogre::String("stunts_gui");
	}

};
#endif

//#include "xmlParser/xmlParser.h"
class XMLNode{

};

/** GUI parser.
 *  This class contains functions to parse XML trees and generate pages, objects
 *  actions and graphics objects from them. It is friend to all relevant objects
 *  and hence can simply modify their private values.
 */

class CGuiParser {

public:

	/** Constructor initializing render context.
	 *  @param  Context  Render context to be used by GUI.
     */
	CGuiParser( int32 Context ) { mContext = Context; }

	/** Parse page XML data from a stream.
	 *  @param   PageName  Page's name to be internally stored.
	 *  @param   Source    Source data to parse.
	 *  @return  Pointer to the new page object in the heap.
	 */
	CGuiPage* ParsePage( std::string PageName, std::string Data ); 

private:

	/* Sub-parsers for internal use only. */
	CGuiAction* ParseAction( XMLNode Node );
	CGuiObject* ParseObject( XMLNode Node );
	CGuiGfx*    ParseGfx   ( XMLNode Node );

	std::map<std::string, CGuiGfx*> mGfx; /**< Internal graphics index. */

	int32 mContext; /**< render context handle */

};


#endif
