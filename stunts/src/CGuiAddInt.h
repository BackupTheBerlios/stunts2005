/* CVS $Id: CGuiAddInt.h,v 1.2 2005/06/12 19:43:45 elowar Exp $ */

/** @file
 *  Add integer to setting action for Stunts 2005 GUI objects.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.2 $
 *  @date    CVS $Date: 2005/06/12 19:43:45 $
 */


#ifndef __C_GUI_ADD_INT_H
#define __C_GUI_ADD_INT_H


#include <nrEngine/nrEngine.h>

#include <boost/lexical_cast.hpp> // possibly already included by nrEngine.h

#include <string>

#include "CGuiAction.h"

class CGuiParser; // for friend declaration below


/** GUI add integer action class.
 *  This action adds a specified integer value to an associated setting.
 */

class CGuiAddInt : public CGuiAction {

public:

	/** Constructor initializing values.
	 *  @param  Context  Render context to be used by GUI.
	 *  @param  Setting  Setting to be associated to action.
	 *  @param  Step     Value to be added, can be negative.
	 *  @param  Max      Maximum value (for wrap around).
	 *  @param  Min      Minimum value (for wrap around).
     */
	CGuiAddInt( int32 Context, std::string Setting,
		int Step, int Max, int Min );

	/** Destructor. */
	~CGuiAddInt() { }

	/* The following functions are required by the CGuiAction interface. */
	virtual void Perform();

private:

	std::string mSetting; /**< Name of the associated setting. */

	int mStep; /**< Value to be added, can be negative. */

	int mMax; /**< Maximum value. */
	int mMin; /**< Minimum value. */

	int32 mContext; /**< render context handle */

/* @note Slightly unclean method to make the parser's life easier. */
friend class CGuiParser;

};


#endif
