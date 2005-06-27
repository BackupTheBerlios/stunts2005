/* CVS $Id: CGuiGfx.h,v 1.3 2005/06/27 01:19:17 psyborg Exp $ */

/** @file
 *  Abstract base class for GUI graphics for Stunts 2005.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.3 $
 *  @date    CVS $Date: 2005/06/27 01:19:17 $
 */


#ifndef __C_GUI_GFX_H
#define __C_GUI_GFX_H


#include <nrEngine/nrEngine.h>

#include <GL/gl.h> // possibly already included by nrEngine.h

namespace stunts
{
	
	class CGuiParser; // for friend declaration below
	
	
	/** GUI graphics abstract base class.
	*  This class specifies a common interface for all GUI graphics objects.
	*  There's not all that much they all have in common.
	*/
	
	class CGuiGfx {
	
	public:
	
		/** Draw current frame (animation, if any, is handled internally).
		*  @param  Pos  x, y and z-position (if applicable) to draw at.
		*/
		virtual void Draw( unsigned int Pos[3] ) = 0;
	
	private:
	
		int32 mContext; /**< render context handle */
	
	/* @note Slightly unclean method to make the parser's life easier. */
	friend class CGuiParser;
	
	};
	
};

#endif
