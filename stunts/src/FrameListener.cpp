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
 
#include "FrameListener.hpp"


namespace stunts
{
	CFrameListener::CFrameListener(RenderWindow* win, Camera* cam,
		bool useBufferedInputKeys,
		bool useBufferedInputMouse)
    {
        mDebugOverlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");
        mUseBufferedInputKeys = useBufferedInputKeys;
		mUseBufferedInputMouse = useBufferedInputMouse;
		mInputTypeSwitchingOn = mUseBufferedInputKeys || mUseBufferedInputMouse;
        mRotateSpeed = 36;
        mMoveSpeed = 100;

		if (mInputTypeSwitchingOn)
		{
            mEventProcessor = new EventProcessor();
			mEventProcessor->initialise(win);
			mEventProcessor->startProcessingEvents();
			mEventProcessor->addKeyListener(this);
			mInputDevice = mEventProcessor->getInputReader();

		}
        else
        {
            mInputDevice = PlatformManager::getSingleton().createInputReader();
            mInputDevice->initialise(win,true, true);
        }

        mCamera = cam;
        mWindow = win;
        mStatsOn = true;
		mNumScreenShots = 0;
		mTimeUntilNextToggle = 0;
        mSceneDetailIndex = 0;
        mMoveScale = 0.0f;
        mRotScale = 0.0f;
	    mTranslateVector = Vector3::ZERO;
        mAniso = 1;
        mFiltering = TFO_BILINEAR;

        showDebugOverlay(true);
	}
	
	
	CFrameListener::~CFrameListener()
	{
		if (mInputTypeSwitchingOn)
		{
            delete mEventProcessor;
		}
        else
        {
            PlatformManager::getSingleton().destroyInputReader( mInputDevice );
        }
	}
	
	
    void CFrameListener::updateStats()
    {
        static String currFps = "Current FPS: ";
        static String avgFps = "Average FPS: ";
        static String bestFps = "Best FPS: ";
        static String worstFps = "Worst FPS: ";
        static String tris = "Triangle Count: ";

        // update stats when necessary
        try {
            OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
            OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
            OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
            OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");

            const RenderTarget::FrameStats& stats = mWindow->getStatistics();

            guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));
            guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
            guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS)
                +" "+StringConverter::toString(stats.bestFrameTime)+" ms");
            guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS)
                +" "+StringConverter::toString(stats.worstFrameTime)+" ms");

            OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
            guiTris->setCaption(tris + StringConverter::toString(stats.triangleCount));

            OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
            guiDbg->setCaption(mWindow->getDebugText());
        }
        catch(...)
        {
            // ignore
        }
    }



    bool CFrameListener::processUnbufferedKeyInput(const FrameEvent& evt)
    {
        if (mInputDevice->isKeyDown(KC_A))
        {
            // Move camera left
            mTranslateVector.x = -mMoveScale;
        }

        if (mInputDevice->isKeyDown(KC_D))
        {
            // Move camera RIGHT
            mTranslateVector.x = mMoveScale;
        }

        /* Move camera forward by keypress. */
        if (mInputDevice->isKeyDown(KC_UP) || mInputDevice->isKeyDown(KC_W) )
        {
            mTranslateVector.z = -mMoveScale;
        }

        /* Move camera backward by keypress. */
        if (mInputDevice->isKeyDown(KC_DOWN) || mInputDevice->isKeyDown(KC_S) )
        {
            mTranslateVector.z = mMoveScale;
        }

        if (mInputDevice->isKeyDown(KC_PGUP))
        {
            // Move camera up
            mTranslateVector.y = mMoveScale;
        }

        if (mInputDevice->isKeyDown(KC_PGDOWN))
        {
            // Move camera down
            mTranslateVector.y = -mMoveScale;
        }

        if (mInputDevice->isKeyDown(KC_RIGHT))
        {
            mCamera->yaw(-mRotScale);
        }
		
        if (mInputDevice->isKeyDown(KC_LEFT))
        {
            mCamera->yaw(mRotScale);
        }

        if( mInputDevice->isKeyDown( KC_ESCAPE) )
        {            
            return false;
        }

		// see if switching is on, and you want to toggle 
        if (mInputTypeSwitchingOn && mInputDevice->isKeyDown(KC_M) && mTimeUntilNextToggle <= 0)
        {
			switchMouseMode();
            mTimeUntilNextToggle = 1;
        }

        if (mInputTypeSwitchingOn && mInputDevice->isKeyDown(KC_K) && mTimeUntilNextToggle <= 0)
        {
			// must be going from immediate keyboard to buffered keyboard
			switchKeyMode();
            mTimeUntilNextToggle = 1;
        }
        if (mInputDevice->isKeyDown(KC_F) && mTimeUntilNextToggle <= 0)
        {
            mStatsOn = !mStatsOn;
            showDebugOverlay(mStatsOn);

            mTimeUntilNextToggle = 1;
        }
        if (mInputDevice->isKeyDown(KC_T) && mTimeUntilNextToggle <= 0)
        {
            switch(mFiltering)
            {
            case TFO_BILINEAR:
                mFiltering = TFO_TRILINEAR;
                mAniso = 1;
                break;
            case TFO_TRILINEAR:
                mFiltering = TFO_ANISOTROPIC;
                mAniso = 8;
                break;
            case TFO_ANISOTROPIC:
                mFiltering = TFO_BILINEAR;
                mAniso = 1;
                break;
            default:
                break;
            }
            MaterialManager::getSingleton().setDefaultTextureFiltering(mFiltering);
            MaterialManager::getSingleton().setDefaultAnisotropy(mAniso);


            showDebugOverlay(mStatsOn);

            mTimeUntilNextToggle = 1;
        }

        if (mInputDevice->isKeyDown(KC_SYSRQ) && mTimeUntilNextToggle <= 0)
        {
			char tmp[20];
			sprintf(tmp, "screenshot_%d.png", ++mNumScreenShots);
            mWindow->writeContentsToFile(tmp);
            mTimeUntilNextToggle = 0.5;
			mWindow->setDebugText(String("Wrote ") + tmp);
        }
		
		if (mInputDevice->isKeyDown(KC_R) && mTimeUntilNextToggle <=0)
		{
			mSceneDetailIndex = (mSceneDetailIndex+1)%3 ;
			switch(mSceneDetailIndex) {
				case 0 : mCamera->setDetailLevel(SDL_SOLID) ; break ;
				case 1 : mCamera->setDetailLevel(SDL_WIREFRAME) ; break ;
				case 2 : mCamera->setDetailLevel(SDL_POINTS) ; break ;
			}
			mTimeUntilNextToggle = 0.5;
		}

        static bool displayCameraDetails = false;
        if (mInputDevice->isKeyDown(KC_P) && mTimeUntilNextToggle <= 0)
        {
            displayCameraDetails = !displayCameraDetails;
            mTimeUntilNextToggle = 0.5;
            if (!displayCameraDetails)
                mWindow->setDebugText("");
        }
        if (displayCameraDetails)
        {
            // Print camera details
            mWindow->setDebugText("P: " + StringConverter::toString(mCamera->getDerivedPosition()) + " " + 
                "O: " + StringConverter::toString(mCamera->getDerivedOrientation()));
        }

        // Return true to continue rendering
        return true;
    }

    bool CFrameListener::processUnbufferedMouseInput(const FrameEvent& evt)
    {
        /* Rotation factors, may not be used if the second mouse button is pressed. */

        /* If the second mouse button is pressed, then the mouse movement results in 
           sliding the camera, otherwise we rotate. */
        if( mInputDevice->getMouseButton( 1 ) )
        {
            mTranslateVector.x += mInputDevice->getMouseRelativeX() * 0.13;
            mTranslateVector.y -= mInputDevice->getMouseRelativeY() * 0.13;
        }
        else
        {
            mRotX = Degree(-mInputDevice->getMouseRelativeX() * 0.13);
            mRotY = Degree(-mInputDevice->getMouseRelativeY() * 0.13);
        }


		return true;
	}

	void CFrameListener::moveCamera()
	{

        // Make all the changes to the camera
        // Note that YAW direction is around a fixed axis (freelook style) rather than a natural YAW (e.g. airplane)
        mCamera->yaw(mRotX);
        mCamera->pitch(mRotY);
        mCamera->moveRelative(mTranslateVector);


	}

    void CFrameListener::showDebugOverlay(bool show)
    {
        if (mDebugOverlay)
        {
            if (show)
            {
                mDebugOverlay->show();
            }
            else
            {
                mDebugOverlay->hide();
            }
        }
    }

    // Override frameStarted event to process that (don't care about frameEnded)
    bool CFrameListener::frameStarted(const FrameEvent& evt)
    {
        if(mWindow->isClosed())
            return false;

        if (!mInputTypeSwitchingOn)
    	{
            mInputDevice->capture();
        }


		if ( !mUseBufferedInputMouse || !mUseBufferedInputKeys)
		{
			// one of the input modes is immediate, so setup what is needed for immediate mouse/key movement
			if (mTimeUntilNextToggle >= 0) 
				mTimeUntilNextToggle -= evt.timeSinceLastFrame;

			// If this is the first frame, pick a speed
			if (evt.timeSinceLastFrame == 0)
			{
				mMoveScale = 1;
				mRotScale = 0.1;
			}
			// Otherwise scale movement units by time passed since last frame
			else
			{
				// Move about 100 units per second,
				mMoveScale = mMoveSpeed * evt.timeSinceLastFrame;
				// Take about 10 seconds for full rotation
				mRotScale = mRotateSpeed * evt.timeSinceLastFrame;
			}
			mRotX = 0;
            mRotY = 0;
	        mTranslateVector = Vector3::ZERO;
		}

        if (mUseBufferedInputKeys)
        {
            // no need to do any processing here, it is handled by event processor and 
			// you get the results as KeyEvents
        }
        else
        {
            if (processUnbufferedKeyInput(evt) == false)
			{
				return false;
			}
        }
        if (mUseBufferedInputMouse)
        {
            // no need to do any processing here, it is handled by event processor and 
			// you get the results as MouseEvents
        }
        else
        {
            if (processUnbufferedMouseInput(evt) == false)
			{
				return false;
			}
        }

		if ( !mUseBufferedInputMouse || !mUseBufferedInputKeys)
		{
			// one of the input modes is immediate, so update the movement vector

			moveCamera();

		}

		return true;
    }

    bool CFrameListener::frameEnded(const FrameEvent& evt)
    {
        updateStats();
        return true;
    }

	void CFrameListener::switchMouseMode() 
	{
        mUseBufferedInputMouse = !mUseBufferedInputMouse;
		mInputDevice->setBufferedInput(mUseBufferedInputKeys, mUseBufferedInputMouse);
	}
	void CFrameListener::switchKeyMode() 
	{
        mUseBufferedInputKeys = !mUseBufferedInputKeys;
		mInputDevice->setBufferedInput(mUseBufferedInputKeys, mUseBufferedInputMouse);
	}

	void CFrameListener::keyClicked(KeyEvent* e) 
	{
		if (e->getKeyChar() == 'm')
		{
			switchMouseMode();
		}
		else if (e->getKeyChar() == 'k')
		{

			switchKeyMode();
		}

	}
	void CFrameListener::keyPressed(KeyEvent* e) {}
	void CFrameListener::keyReleased(KeyEvent* e) {}
	
	

}	//namespace stunts
