/* CVS $Id: CGuiStartGame.cpp,v 1.1 2005/06/09 15:35:48 elowar Exp $ */

/** @file
 *  Start game action for Stunts 2005 GUI objects.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.1 $
 *  @date    CVS $Date: 2005/06/09 15:35:48 $
 */


#include "CGuiStartGame.h"
using std::string;


/** Perform add integer action. */
void CGuiStartGame::Perform() {
	// does this just load the level or also start the game?
	nrSettings.get( "load_level" ) = string( "1" );
}

