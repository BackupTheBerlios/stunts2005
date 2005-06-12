/* CVS $Id: CGuiExitGui.cpp,v 1.2 2005/06/12 19:43:45 elowar Exp $ */

/** @file
 *  Exit GUI action for Stunts 2005 GUI objects.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.2 $
 *  @date    CVS $Date: 2005/06/12 19:43:45 $
 */


#include "CGuiExitGui.h"
using boost::shared_ptr;
using std::string;


/** Perform add integer action. */
void CGuiExitGui::Perform() {
	boost::shared_ptr<nrITask> GuiTask;
	GuiTask = nrKernel.getTaskByName( string( "CGuiTask" ) );
	((CGuiTask*)GuiTask.get()) -> rActive() = false;
}

