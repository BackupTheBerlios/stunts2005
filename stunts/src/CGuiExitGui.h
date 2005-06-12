/* CVS $Id: CGuiExitGui.h,v 1.2 2005/06/12 19:43:45 elowar Exp $ */

/** @file
 *  Exit GUI action for Stunts 2005 GUI objects.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.2 $
 *  @date    CVS $Date: 2005/06/12 19:43:45 $
 */


#ifndef __C_GUI_EXIT_GUI_H
#define __C_GUI_EXIT_GUI_H


#include <nrEngine/nrEngine.h>

#include <boost/shared_ptr.hpp> // possibly already included by nrEngine.h

#include <string>

#include "CGuiTask.h"
#include "CGuiAction.h"

class CGuiParser; // for friend declaration below


/** GUI add integer action class.
 *  This action deactivates the GUI task.
 */

class CGuiExitGui : public CGuiAction {

public:

	/** Constructor initializing render context.
	 *  @param  Context  Render context to be used by GUI.
     */
	CGuiExitGui( int32 Context ) { mContext = Context; }

	/** Destructor. */
	~CGuiExitGui() { }

	/* The following functions are required by the CGuiAction interface. */
	virtual void Perform();

private:

	int32 mContext; /**< render context handle */

/* @note Slightly unclean method to make the parser's life easier. */
friend class CGuiParser;

};


#endif
