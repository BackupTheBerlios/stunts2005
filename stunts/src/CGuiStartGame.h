/* CVS $Id: CGuiStartGame.h,v 1.3 2005/06/27 01:19:18 psyborg Exp $ */

/** @file
 *  Start game action for Stunts 2005 GUI objects.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.3 $
 *  @date    CVS $Date: 2005/06/27 01:19:18 $
 */


#ifndef __C_GUI_START_GAME_H
#define __C_GUI_START_GAME_H


#include <nrEngine/nrEngine.h>

#include <string>

#include "CGuiAction.h"

namespace stunts
{
	
	class CGuiParser; // for friend declaration below
	
	
	/** GUI add integer action class.
	*  This action deactivates the GUI task.
	*/
	
	class CGuiStartGame : public CGuiAction {
	
	public:
	
		/** Constructor initializing render context.
		*  @param  Context  Render context to be used by GUI.
		*/
		CGuiStartGame( int32 Context ) { mContext = Context; }
	
		/** Destructor. */
		~CGuiStartGame() { }
	
		/* The following functions are required by the CGuiAction interface. */
		virtual void Perform();
	
	private:
	
		int32 mContext; /**< render context handle */
	
	/* @note Slightly unclean method to make the parser's life easier. */
	friend class CGuiParser;
	
	};
	
};

#endif
