/* CVS $Id: CGuiAction.h,v 1.1 2005/06/09 15:35:48 elowar Exp $ */

/** @file
 *  Abstract base class for GUI actions for Stunts 2005.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.1 $
 *  @date    CVS $Date: 2005/06/09 15:35:48 $
 */


#ifndef __C_GUI_ACTION_H
#define __C_GUI_ACTION_H


#include <nrEngine/nrEngine.h>

#include <string>

class CGuiParser; // for friend declaration below


/** GUI action abstract base class.
 *  This class specifies a common interface for all GUI actions.
 *  There's not all that much they all have in common.
 */

class CGuiAction {

public:

	/** Perform specific action. */
	virtual void Perform() = 0;

private:

	int32 mContext; /**< render context handle */

/* @note Slightly unclean method to make the parser's life easier. */
friend class CGuiParser;

};


#endif