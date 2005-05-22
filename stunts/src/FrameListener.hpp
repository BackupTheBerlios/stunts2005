/* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup, 
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Christian Morbach <GameDevelopment@paratronic.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, 
 * USA.
 */

#define GCC_3_1
#define EXT_HASH

#include <Ogre.h>
#include <OgreKeyEvent.h>
#include <OgreEventListeners.h>
#include <OgreStringConverter.h>
#include <OgreException.h>

#include <nrEngine/nrEngine.h>

#ifndef _CFRAMELISTENER_H_
#define _CFRAMELISTENER_H_

using namespace Ogre;

namespace stunts
{

	class CFrameListener : public FrameListener, public KeyListener
	{
	public:
		CFrameListener(RenderWindow* win, Camera* cam,
			bool useBufferedInputKeys = false,
			bool useBufferedInputMouse = false);
		~CFrameListener();
		CFrameListener();

		bool processUnbufferedKeyInput(const FrameEvent& evt);
		bool processUnbufferedMouseInput(const FrameEvent& evt);
		void moveCamera();
		void showDebugOverlay(bool show);

		bool frameStarted(const FrameEvent& evt);
		bool updateWorld();
		bool frameEnded(const FrameEvent& evt);

		void switchMouseMode();
		void switchKeyMode();
		void keyClicked(KeyEvent* e);
		void keyPressed(KeyEvent* e);
		void keyReleased(KeyEvent* e);

		
		
	protected:
	
		int mSceneDetailIndex ;
		Real mMoveSpeed;
		Degree mRotateSpeed;
		Overlay* mDebugOverlay;
	
		EventProcessor* mEventProcessor;
		InputReader* mInputDevice;
		Camera* mCamera;
	
		Vector3 mTranslateVector;
		RenderWindow* mWindow;
		bool mStatsOn;
		bool mUseBufferedInputKeys, mUseBufferedInputMouse, mInputTypeSwitchingOn;
		unsigned int mNumScreenShots;
		float mMoveScale;
		Degree mRotScale;
		// just to stop toggles flipping too fast
		Real mTimeUntilNextToggle;
		Radian mRotX, mRotY;
		TextureFilterOptions mFiltering;
		int mAniso;

   		void updateStats();

		
		RaySceneQuery* mRaySceneQuery;

		friend class COgreTask;
	};
}	//namespace stunts

#endif	//_CUSERINPUT_H_
