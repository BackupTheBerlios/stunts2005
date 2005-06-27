/* CVS $Id: CGuiParser.h,v 1.5 2005/06/27 01:19:18 psyborg Exp $ */

/** @file
 *  GUI parser for Stunts 2005.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.5 $
 *  @date    CVS $Date: 2005/06/27 01:19:18 $
 */


#ifndef __C_GUI_PARSER_H
#define __C_GUI_PARSER_H

#include <string>
#include <map>

#include "CGuiPage.h"
#include "GuiActions.h"
#include "GuiObjects.h"
#include "GuiGfx.h"

namespace stunts
{
	
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
	
};

#endif
