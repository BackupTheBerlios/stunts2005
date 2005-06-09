/** @file
 *  Dummy graphics object for testing. Inline definitions, ahoy.
 */

#ifndef __C_GUI_DUMMY
#define __C_GUI_DUMMY


#include "CGuiGfx.h"

class CGuiDummy : public CGuiGfx {

	public:

		CGuiDummy( int width = 128, int height = 64, float shade=1 ) {
			mWidth = width;
			mHeight = height;
			mShade = shade;
		}

		void Draw( unsigned int Pos[3] ) {

			int x = Pos[0], y = Pos[1], z = Pos[2];    
	
			// read screen size
			GLint view[4];
			glGetIntegerv( GL_VIEWPORT, view );
	
			// change OpenGL-State
			nrPushOnBlend();
			nrPushOffLit();
			nrPushOffDepthRead();
			nrPushOffDepthWrite();
			nrOrthoMode( 0,0,view[2],view[3] );
			nrPushOffCullFace();
	
			// draw background
			// glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glColor4f( mShade, mShade, mShade, 1.0 );
			
			glBegin(GL_QUADS);
				glVertex2i(x-5       ,y-5        );
				glVertex2i(x+mWidth  ,y          );
				glVertex2i(x+mWidth+5,y+mHeight+5);
				glVertex2i(x         ,y+mHeight);
			glEnd();
	
			// restore old OpenGL-State	
			nrPopCullFace();
			nrPerspectiveMode();
			nrPopDepthWrite();
			nrPopDepthRead();
			nrPopLit();
			nrPopBlend();
		}

	private:
		int mWidth;
		int mHeight;
		float mShade;

};


#endif
