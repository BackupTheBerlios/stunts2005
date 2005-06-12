/* CVS $Id: CGuiStartGame.cpp,v 1.2 2005/06/12 19:43:45 elowar Exp $ */

/** @file
 *  Start game action for Stunts 2005 GUI objects.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.2 $
 *  @date    CVS $Date: 2005/06/12 19:43:45 $
 */


#include "CGuiStartGame.h"
using std::string;


/** Perform add integer action. */
void CGuiStartGame::Perform() {
	// does this just load the level or also start the game?
	nrSettings.get( "load_level" ) = string( "1" );
}

