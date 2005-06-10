/* CVS $Id: CGuiObject.h,v 1.1 2005/06/09 15:35:48 elowar Exp $ */

/** @file
 *  Abstract base class for GUI objects (widgets) for Stunts 2005.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.1 $
 *  @date    CVS $Date: 2005/06/09 15:35:48 $
 */


#ifndef __C_GUI_OBJECT_H
#define __C_GUI_OBJECT_H


#include <string>

class CGuiParser; // for friend declaration below


/** GUI object abstract base class.
 *  This class specifies a common interface for all GUI objects (widgets).
 *  There's not all that much they all have in common.
 */

class CGuiObject {

public:

	/** Update and draw object.
	 *  Draw object and take steps for mouse detection if the mouse has moved.
	 *  Also process input and highlight if the object is in focus.
	 *  @param  MouseMoved  Has the mouse been moved since the last frame?
	 *  @param  Focus       ID of the object that currently has focus.
	 */
	virtual void objectUpdate( bool MouseMoved, unsigned int Focus ) = 0;

	/* The following function makes sorting by Z-index possible. */
	bool operator<( CGuiObject &Object ) const {
		return mPos[2] < Object.mPos[2];
	}

private:

	unsigned int mId;

	unsigned int mPos[3]; /**< Position and Z-index. */

	int32 mContext; /**< render context handle */

/* @note Slightly unclean method to make the parser's life easier. */
friend class CGuiParser;

};


#endif
