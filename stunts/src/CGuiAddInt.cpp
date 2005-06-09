/* CVS $Id: CGuiAddInt.cpp,v 1.1 2005/06/09 15:35:48 elowar Exp $ */

/** @file
 *  Add integer to setting action for Stunts 2005 GUI objects.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.1 $
 *  @date    CVS $Date: 2005/06/09 15:35:48 $
 */


#include "CGuiAddInt.h"
using boost::lexical_cast;
using boost::bad_lexical_cast;
using std::string;


/** Constructor initializing values.
 *  @param  Context  Render context to be used by GUI.
 *  @param  Setting  Setting to be associated to action.
 *  @param  Step     Value to be added, can be negative.
 *  @param  Max      Maximum value (for wrap around).
 *  @param  Min      Minimum value (for wrap around).
 */
CGuiAddInt::CGuiAddInt( int32 Context, string Setting,
	int Step, int Max, int Min ) {

	mContext = Context;
	mSetting = Setting;
	mStep = Step;
	mMax = Max;
	mMin = Min;
}


/** Perform add integer action. */
void CGuiAddInt::Perform() {
	int Range = mMax - mMin;

	int Value; /* Try to get integer value from settings manager. */
	try {
		Value = lexical_cast<int>( (string)nrSettings.get( mSetting ) );
	} catch( bad_lexical_cast & ) {
		nrLog.Log( NR_LOG_APP, /* right place to log ?? */
			"Exception boost::bad_lexical_cast in CGuiAddInt::Peform()!");
		return;
	}

	Value += mStep; /* Increase value. */

	/* Clamp to range. */
	if( Value > mMax ) Value = ( abs( Value-mMin ) % ( mMax-mMin ) ) + mMin;
	if( Value < mMin ) Value = mMax - ( abs( Value-mMin ) % ( mMax-mMin ) );

	nrSettings.get( mSetting ) = lexical_cast<string>( Value ); /* write back */
}

