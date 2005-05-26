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
//	CFrameListener::CFrameListener(RenderWindow* win, Camera* cam)
//    {
//
//        mDebugOverlay = 
//        	OverlayManager::getSingleton().getByName("Core/DebugOverlay");
//
//        mRotateSpeed = 36;
//        mMoveSpeed = 100;
//
//
//        mCamera = cam;
//        mWindow = win;
//        mStatsOn = true;
//		mNumScreenShots = 0;
//		mTimeUntilNextToggle = 0;
//        mSceneDetailIndex = 0;
//        mMoveScale = 0.0f;
//        mRotScale = 0.0f;
//	    mTranslateVector = Vector3::ZERO;
//        mAniso = 1;
//        mFiltering = TFO_BILINEAR;
//
//        //showDebugOverlay(true);
//        showDebugOverlay(false);
//
//		mRaySceneQuery = 0;
//	}
//
//
//	CFrameListener::~CFrameListener()
//	{
//		delete mRaySceneQuery;
//		PlatformManager::getSingleton().destroyInputReader( mInputDevice );
//	}
//
//
//    void CFrameListener::updateStats()
//    {
//		return;
//
//        static String currFps = "Current FPS: ";
//        static String avgFps = "Average FPS: ";
//        static String bestFps = "Best FPS: ";
//        static String worstFps = "Worst FPS: ";
//        static String tris = "Triangle Count: ";
//
//        // update stats when necessary
//        try {
//            OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
//            OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
//            OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
//            OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");
//
//            const RenderTarget::FrameStats& stats = mWindow->getStatistics();
//
//            guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));
//            guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
//            guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS)
//                +" "+StringConverter::toString(stats.bestFrameTime)+" ms");
//            guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS)
//                +" "+StringConverter::toString(stats.worstFrameTime)+" ms");
//
//            OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
//            guiTris->setCaption(tris + StringConverter::toString(stats.triangleCount));
//
//            OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
//            guiDbg->setCaption(mWindow->getDebugText());
//        }
//        catch(...)
//        {
//            // ignore
//        }
//    }
//
//
//
//
//    void CFrameListener::showDebugOverlay(bool show)
//    {
//        if (mDebugOverlay)
//        {
//            if (show)
//            {
//                mDebugOverlay->show();
//            }
//            else
//            {
//                mDebugOverlay->hide();
//            }
//        }
//    }
//
//    // Override frameStarted event to process that (don't care about frameEnded)
//    bool CFrameListener::frameStarted(const FrameEvent& evt)
//    {
//
//		// if window is not active, so stop rendering
//        if(mWindow->isClosed())
//            return false;
//
//		// Capture input device
//
//        mInputDevice->capture();
//        
//
//
//			// one of the input modes is immediate, so setup what is needed for immediate mouse/key movement
//			if (mTimeUntilNextToggle >= 0)
//				mTimeUntilNextToggle -= evt.timeSinceLastFrame;
//
//			// If this is the first frame, pick a speed
//			if (evt.timeSinceLastFrame == 0)
//			{
//				mMoveScale = 1;
//				mRotScale = 0.1;
//			}
//			// Otherwise scale movement units by time passed since last frame
//			else
//			{
//				// Move about 100 units per second,
//				mMoveScale = mMoveSpeed * evt.timeSinceLastFrame;
//				// Take about 10 seconds for full rotation
//				mRotScale = mRotateSpeed * evt.timeSinceLastFrame;
//			}
//			mRotX = 0;
//            mRotY = 0;
//	        mTranslateVector = Vector3::ZERO;
//
//
//
//            if (processUnbufferedKeyInput(evt) == false)
//			{
//				return false;
//			}
//        
//
//
//            if (processUnbufferedMouseInput(evt) == false)
//			{
//				return false;
//			}
//
//
//
//			moveCamera();
//
//
//
//		return updateWorld();
//
//    }
//
//	/**
//	 * Update the world geometry
//	 **/
//	bool CFrameListener::updateWorld()
//	{
//        // clamp to terrain
//        static Ray updateRay;
//        updateRay.setOrigin(mCamera->getPosition());
//        updateRay.setDirection(Vector3::NEGATIVE_UNIT_Y);
//        mRaySceneQuery->setRay(updateRay);
//        RaySceneQueryResult& qryResult = mRaySceneQuery->execute();
//        RaySceneQueryResult::iterator i = qryResult.begin();
//        if (i != qryResult.end() && i->worldFragment)
//        {
//            //SceneQuery::WorldFragment* wf = i->worldFragment;
//            mCamera->setPosition(mCamera->getPosition().x,
//                i->worldFragment->singleIntersection.y + 10,
//                mCamera->getPosition().z);
//        }
//
//        return true;
//	}
//
//
//    bool CFrameListener::frameEnded(const FrameEvent& evt)
//    {
//        updateStats();
//        return true;
//    }

}	//namespace stunts
