/* CVS $Id: CGuiTex.h,v 1.3 2005/06/27 01:19:19 psyborg Exp $ */

/** @file
 *  Simple 2D texture graphics for Stunts 2005.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.3 $
 *  @date    CVS $Date: 2005/06/27 01:19:19 $
 */


#ifndef __C_GUI_TEX
#define __C_GUI_TEX


#include <nrEngine/nrEngine.h>

#include <string>

#include "CGuiGfx.h"

namespace stunts
{
	
	/** Simple 2D texture graphics class.
	*  This class provides loading and drawing of simple 2D texture images for use
	*  in GUI objects.
	*/
	class CGuiTex : public CGuiGfx {
	
	public:
	
		/** Constructor initializing render context and filename.
		*  @param  Context   Render context to be used by GUI.
		*  @param  Filename  File to load texture from.
		*/
		CGuiTex( int32 Context, std::string FileName );
	
		/** Destructor. */
		~CGuiTex() { }
	
		/* The following function is required by the CGuiGfx interface. */
		void Draw( unsigned int Pos[3] );
	
	private:
	
		std::string mFileName;  /**< Let's remember the filename just in case. */
	
		nrCTexture mTexture;  /**< Our texture. */
	
	};

};

#endif
