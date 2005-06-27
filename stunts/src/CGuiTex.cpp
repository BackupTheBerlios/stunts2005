/* CVS $Id: CGuiTex.cpp,v 1.3 2005/06/27 01:19:19 psyborg Exp $ */

/** @file
 *  Simple 2D texture graphics for Stunts 2005.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.3 $
 *  @date    CVS $Date: 2005/06/27 01:19:19 $
 */


#include "CGuiTex.h"
using std::string;

namespace stunts
{
	
	/** Constructor initializing render context and filename. */
	CGuiTex::CGuiTex( int32 Context, std::string FileName ) {
		mFileName = FileName;
	
		/* For efficiency better use RECT later when possible. */
		mTexture.loadFromFile( FileName, NR_TEXTURE_2D,
			NR_RGBA | NR_LINEAR | NR_CLAMP );
	}
	
	
	/** Drawing. */
	void CGuiTex::Draw( unsigned int Pos[3] ) {
	
		/* Read screen size. */
		GLint View[4]; glGetIntegerv( GL_VIEWPORT, View );
	
		/* Render texture. */
		nrOrthoMode( 0, 0, View[2], View[3] );
			nrPushOffLit();
			nrPushOffDepthRead();
			nrPushOffDepthWrite();
			nrPushOffBlend();
			glDisable( GL_CULL_FACE );
	
			glColor4f( 1.0, 1.0, 1.0, 1.0 );
			mTexture.enable();
			mTexture.bind();
			mTexture.render( Pos[0], Pos[1],
				Pos[0] + mTexture.getWidth(), Pos[1] + mTexture.getHeight() );
			mTexture.disable();
	
			glEnable( GL_CULL_FACE );
			nrPopBlend();
			nrPopDepthWrite();
			nrPopDepthRead();
			nrPopDepthRead();
			nrPopLit();
		nrPerspectiveMode();
	}
};

