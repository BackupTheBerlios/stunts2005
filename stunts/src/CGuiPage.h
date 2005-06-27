/* CVS $Id: CGuiPage.h,v 1.3 2005/06/27 01:19:17 psyborg Exp $ */

/** @file
 *  GUI Page container for Stunts 2005.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.3 $
 *  @date    CVS $Date: 2005/06/27 01:19:17 $
 */


#ifndef __C_GUI_PAGE_H
#define __C_GUI_PAGE_H

#include <nrEngine/nrEngine.h>

#include <string>
#include <vector>
#include <map>

#include "GuiObjects.h" // include all objects
#include <OGRE/Ogre.h>

/**
 * Declare this class as forward declaration here. I do not include any CEGUI, files
 * here, because of include conflicts with other files. The needed files will be included
 * in *.cpp file.
 **/
namespace CEGUI{
	class Window;
}

/* Maximum object index; Limited by number of stencil buffer bitplanes */
#define GUI_MAX_INDEX = 255

namespace stunts
{
	class CGuiParser;
		
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
		void pageSelect();
	
		/** Draw page and process input. */
		void pageUpdate();

		/** Says to the page, that it was deactivated */
		void pageDeactivate();
		
		/* The following methods provide access to values of and/or references to
		* private class attributes where suitable. */
		std::string PageName() { return mPageName; }
	
		/** Returns the used CEGUI sheet
		*/
		CEGUI::Window*	getGUISheet() { return mGUISheet; }
		
	private:
	
		std::string mPageName; /**< unique name of GUI page */
	
		std::vector<CGuiObject*> mObjects; /**< GUI objects sorted by z-index */
	
		unsigned int mFocus; /**< ID of currently focused object; != 0 */
		unsigned int mMaxId; /**< highest ID used by any active object */
	
		int32 mContext; /**< render context handle */
	
	/* @note Slightly unclean method to make the parser's life easier. */
	friend class CGuiParser;
	
		/* CEGUI - Sheet interface */
		CEGUI::Window*		mGUISheet;
		
	};

};

#endif
