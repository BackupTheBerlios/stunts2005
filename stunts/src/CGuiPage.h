/* CVS $Id: CGuiPage.h,v 1.2 2005/06/12 19:43:45 elowar Exp $ */

/** @file
 *  GUI Page container for Stunts 2005.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.2 $
 *  @date    CVS $Date: 2005/06/12 19:43:45 $
 */


#ifndef __C_GUI_PAGE_H
#define __C_GUI_PAGE_H


#include <nrEngine/nrEngine.h>

#include <GL/gl.h> // possibly already included by nrEngine.h

#include <string>
#include <vector>
#include <map>

#include "GuiObjects.h" // include all objects

class CGuiParser; // for friend declaration below


/* Maximum object index; Limited by number of stencil buffer bitplanes */
#define GUI_MAX_INDEX = 255


/** GUI Page container.
 *  This class provides functions for managing and activating (when the page
 *  itself is activated) GUI objects. This class decides which object obtains
 *  the focus and forwards input events to it.
 */

class CGuiPage {

public:

	/** Constructor initializing render context and page name.
	 *  @param  Context   Render context to be used by GUI.
	 *  @param  PageName  Unique name of GUI page.
     */
	CGuiPage( int32 Context, std::string PageName );

	/** Destructor. */
	~CGuiPage();

	/** Actions upon selection of page by task manager. */
	void CGuiPage::pageSelect();

	/** Draw page and process input. */
	void pageUpdate();

	/* The following methods provide access to values of and/or references to
     * private class attributes where suitable. */
	std::string PageName() { return mPageName; }

private:

	std::string mPageName; /**< unique name of GUI page */

	std::vector<CGuiObject*> mObjects; /**< GUI objects sorted by z-index */

	unsigned int mFocus; /**< ID of currently focused object; != 0 */
	unsigned int mMaxId; /**< highest ID used by any active object */

	int32 mContext; /**< render context handle */

/* @note Slightly unclean method to make the parser's life easier. */
friend class CGuiParser;

};


#endif
