/* CVS $Id: CGuiButton.h,v 1.3 2005/06/27 01:19:17 psyborg Exp $ */

/** @file
 *  Button object (CGuiObject) for Stunts 2005.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.3 $
 *  @date    CVS $Date: 2005/06/27 01:19:17 $
 */


#ifndef __C_GUI_BUTTON_H
#define __C_GUI_BUTTON_H


#include <nrEngine/nrEngine.h>

#include <GL/gl.h> // possibly already included by nrEngine.h

#include <string>
#include <vector>
#include <map>

#include "CGuiObject.h"

#include "CGuiGfx.h"
#include "CGuiAction.h"

namespace stunts
{
	
	class CGuiParser; // for friend declration below
	
	
	/** GUI button object.
	*  This is a generic button object (CGuiObject).
	*/
	
	class CGuiButton : public CGuiObject {
	
	public:
	
		/** Constructor initializing render context and ID.
		*  @param  Context   Render context to be used by GUI.
		*  @param  Id        ID to be associated with the object.
		*
		*  @note IMPORTANT: mGfx["default"] and mGfx["highlight"] must always be
		*        initialized by the creator (usually parser).
		*/
		CGuiButton( int32 Context, unsigned int Id );
	
		/** Destructor. */
		~CGuiButton();
	
		/* The following functions are required by the CGuiObject interface. */
		virtual void objectUpdate( bool MouseMoved, unsigned int Focus );
	
	private:
	
		std::string mCaption; /**< Caption of button. */
	
		std::map<std::string, CGuiGfx*> mGfx; /**< Graphics objects for button. */
	
		std::vector<CGuiAction*> mActions; /**< Actions for button to perform. */
	
		unsigned int mId;
	
		unsigned int mPos[3]; /**< Position and Z-index. */
	
		int32 mContext; /**< render context handle */
	
	/* @note Slightly unclean method to make the parser's life easier. */
	friend class CGuiParser;
	
	};
	
};
	
#endif

