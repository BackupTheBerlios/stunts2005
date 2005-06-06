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

#include "UserInput.hpp"

namespace stunts
{
	//--------------------------------------------------------------------------
	//--- CUserInput()
	//--------------------------------------------------------------------------
	CUserInput::CUserInput(boost::shared_ptr< CLevel > level)
	{
		mLevel = level;
		mActivated = false;		//initially this input device is disabled
	}


	//--------------------------------------------------------------------------
	//--- ~CUserInput()
	//--------------------------------------------------------------------------
	CUserInput::~CUserInput()
	{

	}


	//--------------------------------------------------------------------------
	//--- taskInit()
	//--------------------------------------------------------------------------
	nrResult CUserInput::taskInit()
	{
		return NR_OK;
	}


	//--------------------------------------------------------------------------
	//--- taskStart()
	//--------------------------------------------------------------------------
	nrResult CUserInput::taskStart()
	{
		if (!(mLevel->OgreTask()))
		{
			nrLog.Log(NR_LOG_APP, "CUserInput::taskStart(): Error in getting\
				OgreTask");

			#ifdef ENABLE_DEBUG_MESSAGES
				std::cout << "CUserInput::taskStart(): Error in getting\
				OgreTask" << std::endl;
			#endif

			nrKernel.KillAllTasks();
		}
		else
		{
			mInputDevice = mLevel->OgreTask()->mInputDevice;
			mCamera = mLevel->OgreTask()->mCamera;
			mTerrain = mLevel->Terrain();
		}

		//return
		return NR_OK;
	}


	//--------------------------------------------------------------------------
	//--- taskUpdate()
	//--------------------------------------------------------------------------
	nrResult CUserInput::taskUpdate()
	{
		this->react(0.0f);		//react here only in this example application
								//	as no InteractiveObject is running
		return NR_OK;
	}


	//--------------------------------------------------------------------------
	//--- taskStop()
	//--------------------------------------------------------------------------
	nrResult CUserInput::taskStop()
	{
		return NR_OK;
	}


	//--------------------------------------------------------------------------
	//--- react(float delaySeconds)
	//--------------------------------------------------------------------------
	void CUserInput::react(float delaySeconds)
	{
		if (!mInputDevice || !mActivated)
			return;

		mInputDevice->capture();

		// app specific
		Ogre::Vector3 mTranslateVector(0.0f, 0.0f, 0.0f);
		float mMoveScale = 1;		// * delaySeconds in fact!
		Ogre::Degree mRotScale(1);	// * delaySeconds in fact!
		Ogre::Degree mRotX;
		Ogre::Degree mRotY;
		//


		if (mInputDevice->isKeyDown(Ogre::KC_A))
        {
            // Move camera left
            mTranslateVector.x = -mMoveScale;
        }

        if (mInputDevice->isKeyDown(Ogre::KC_D))
        {
            // Move camera RIGHT
            mTranslateVector.x = mMoveScale;
        }

        /* Move camera forward by keypress. */
        if (mInputDevice->isKeyDown(Ogre::KC_UP) ||
        	mInputDevice->isKeyDown(Ogre::KC_W) )
        {
            mTranslateVector.z = -mMoveScale;
        }

        /* Move camera backward by keypress. */
        if (mInputDevice->isKeyDown(Ogre::KC_DOWN) ||
        	mInputDevice->isKeyDown(Ogre::KC_S) )
        {
            mTranslateVector.z = mMoveScale;
        }

        if (mInputDevice->isKeyDown(Ogre::KC_PGUP))
        {
            // Move camera up
            mTranslateVector.y = mMoveScale;
        }

        if (mInputDevice->isKeyDown(Ogre::KC_PGDOWN))
        {
            // Move camera down
            mTranslateVector.y = -mMoveScale;
        }

        if (mInputDevice->isKeyDown(Ogre::KC_RIGHT))
        {
            mCamera->yaw(-mRotScale);
        }

        if (mInputDevice->isKeyDown(Ogre::KC_LEFT))
        {
            mCamera->yaw(mRotScale);
        }

        if( mInputDevice->isKeyDown(Ogre::KC_ESCAPE))
        {
        	//end game
	        nrKernel.KillAllTasks();
        }


        if (mInputDevice->isKeyDown(Ogre::KC_F))
        {
        }

        if (mInputDevice->isKeyDown(Ogre::KC_T))
        {
        }

        if (mInputDevice->isKeyDown(Ogre::KC_SYSRQ))
        {
//			char tmp[20];
//			sprintf(tmp, "screenshot_%d.png", ++mNumScreenShots);
//            mWindow->writeContentsToFile(tmp);
//            mTimeUntilNextToggle = 0.5;
//			mWindow->setDebugText(String("Wrote ") + tmp);
        }

		if (mInputDevice->isKeyDown(Ogre::KC_R))
		{
		}

        if (mInputDevice->isKeyDown(Ogre::KC_P))
        {
        }



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
            mRotX = Ogre::Degree(-mInputDevice->getMouseRelativeX() * 0.13);
            mRotY = Ogre::Degree(-mInputDevice->getMouseRelativeY() * 0.13);
        }


        // app specific
        mCamera->yaw(mRotX);
        mCamera->pitch(mRotY);
        mCamera->moveRelative(mTranslateVector);

		mTerrain->getHeight(mTranslateVector);

		//SceneQuery::WorldFragment* wf = i->worldFragment;
		mCamera->setPosition(mCamera->getPosition().x,
			mTranslateVector.y + 10,
			mCamera->getPosition().z);
        //
	}


	//--------------------------------------------------------------------------
	//--- activate(bool activated)
	//--------------------------------------------------------------------------
	void CUserInput::activate(bool activated)
	{
		mActivated = activated;
	}


	//--------------------------------------------------------------------------
	//--- isActivated() const
	//--------------------------------------------------------------------------
	bool CUserInput::isActivated() const
	{
		return mActivated;
	}


}	//namespace stunts




