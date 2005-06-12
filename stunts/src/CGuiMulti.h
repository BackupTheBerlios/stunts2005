/* CVS $Id: CGuiMulti.h,v 1.2 2005/06/12 19:43:45 elowar Exp $ */

/** @file
 *  Variable graphics object (CGuiObject) for Stunts 2005.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.2 $
 *  @date    CVS $Date: 2005/06/12 19:43:45 $
 */


#ifndef __C_GUI_MULTI_H
#define __C_GUI_MULTI_H


#include <nrEngine/nrEngine.h>

#include <GL/gl.h> // possibly already included by nrEngine.h

#include <string>
#include <map>

#include "CGuiObject.h"

#include "CGuiGfx.h"

class CGuiParser; // for friend declration below


/** GUI variable graphics object.
 *  This object (CGuiObject) displays different graphics depending on a setting.
 */

class CGuiMulti : public CGuiObject {

public:

	/** Constructor initializing render context.
	 *  @param  Context   Render context to be used by GUI.
	 *
	 *  @note IMPORTANT: mGfx["default"] must always be initialized by the
	 *        creator (usually parser).
     */
	CGuiMulti( int32 Context );

	/** Destructor. */
	~CGuiMulti();

	/* The following functions are required by the CGuiObject interface. */
	virtual void objectUpdate( bool MouseMoved, unsigned int Focus );

private:

	std::map<std::string, CGuiGfx*> mGfx; /**< Graphics objects. */

	std::string mSetting; /**< Setting choice is based on. */

	unsigned int mId;

	unsigned int mPos[3]; /**< Position and Z-index. */

	int32 mContext; /**< render context handle */

/* @note Slightly unclean method to make the parser's life easier. */
friend class CGuiParser;

};


#endif

