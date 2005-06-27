/* CVS $Id: CGuiStartGame.cpp,v 1.3 2005/06/27 01:19:18 psyborg Exp $ */

/** @file
 *  Start game action for Stunts 2005 GUI objects.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.3 $
 *  @date    CVS $Date: 2005/06/27 01:19:18 $
 */


#include "CGuiStartGame.h"
using std::string;

namespace stunts
{
	
	/** Perform add integer action. */
	void CGuiStartGame::Perform() {
		// does this just load the level or also start the game?
		nrSettings.get( "load_level" ) = string( "1" );
	}

};

