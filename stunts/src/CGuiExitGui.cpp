/* CVS $Id: CGuiExitGui.cpp,v 1.3 2005/06/27 01:19:17 psyborg Exp $ */

/** @file
 *  Exit GUI action for Stunts 2005 GUI objects.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.3 $
 *  @date    CVS $Date: 2005/06/27 01:19:17 $
 */


#include "CGuiExitGui.h"
using boost::shared_ptr;
using std::string;

namespace stunts
{
	
	/** Perform add integer action. */
	void CGuiExitGui::Perform() {
		boost::shared_ptr<nrITask> GuiTask;
		GuiTask = nrKernel.getTaskByName( string( "CGuiTask" ) );
		((CGuiTask*)GuiTask.get()) -> rActive() = false;
	}
	
};

