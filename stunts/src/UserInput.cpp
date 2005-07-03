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
#include "OgreTask.hpp"

#include "CGuiTask.h"

#include "CarObject.hpp"

namespace stunts
{
	//--------------------------------------------------------------------------
	//--- CUserInput()
	//--------------------------------------------------------------------------
	CUserInput::CUserInput(CLevel* level)
	{
		mLevel = level;
		mActivated = false;		//initially this input device is disabled

		mInputDevice = NULL;
		mCamera = NULL;
	}


	//--------------------------------------------------------------------------
	//--- ~CUserInput()
	//--------------------------------------------------------------------------
	CUserInput::~CUserInput()
	{
		mLevel = NULL;
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
		if (!COgreTask::isValid())

		/*
				std::cout << "escape  (init)  "<< (unsigned int)Ogre::KC_ESCAPE <<"\n";
				std::cout << "camRight  (init)  "<< (unsigned int)Ogre::KC_D <<"\n";
		*/

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
			/*
			const char* fileName = "../media/graphics/gui/button/userInput.xml";


			if (  !(importFromFile(fileName))  )
			{
			*/
			if(true)
			{
			mInputDevice = COgreTask::GetSingleton().mInputDevice;
			mCamera = COgreTask::GetSingleton().mCamera;
			mTerrain = mLevel->Terrain();


				// Generating default map
				keymap["escape"] = (unsigned int)Ogre::KC_ESCAPE;
				keymap["screenshot"] = (unsigned int)Ogre::KC_C;

				keymap["camLeft"] = (unsigned int)Ogre::KC_A;
				keymap["camRight"] = (unsigned int)Ogre::KC_D;
				keymap["camForward"] = (unsigned int)Ogre::KC_W;
				keymap["camBackward"] = (unsigned int)Ogre::KC_S;

				keymap["camUp"] = (unsigned int)Ogre::KC_PGUP;
				keymap["camDown"] = (unsigned int)Ogre::KC_PGDOWN;

				keymap["yawMinus"] = (unsigned int)Ogre::KC_RIGHT;
				keymap["yawPlus"] = (unsigned int)Ogre::KC_LEFT;

				//car control
				keymap["carAccelerate"] = (unsigned int)Ogre::KC_I;
				keymap["carBrake"] = (unsigned int)Ogre::KC_K;

				keymap["carLeft"] = (unsigned int)Ogre::KC_J;
				keymap["carRight"] = (unsigned int)Ogre::KC_L;
			}
		}

		//return
		return NR_OK;
	}


	//--------------------------------------------------------------------------
	//--- taskUpdate()
	//--------------------------------------------------------------------------
	nrResult CUserInput::taskUpdate()
	{
		if (controlObject == NULL)
		{
			controlObject = mLevel->getCar(CLevel::CAR_HUMAN);
		}

		//react here only in this example application
		//	as no InteractiveObject is running
		this->react(COgreTask::GetSingleton().mTimer->getFrameInterval());

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
		if (!mInputDevice || !mActivated || (bool)nrSettings.get("gui_active"))
			return;

		mInputDevice->capture();

		// app specific
		Ogre::Vector3 mTranslateVector(0.0f, 0.0f, 0.0f);
		Ogre::Vector3 mTranslateVectorTerrain(0.0f, 0.0f, 0.0f);
		float mMoveScale = 100.0f  * delaySeconds;
		Ogre::Degree mRotScale(120.0f * delaySeconds);
		Ogre::Degree mRotX;
		Ogre::Degree mRotY;


		//control car
		if (controlObject != NULL)
		{
			controlObject->ODEVehicle()->setInputs(
				mInputDevice->isKeyDown((Ogre::KeyCode)keymap["carLeft"]),
				mInputDevice->isKeyDown((Ogre::KeyCode)keymap["carRight"]),
				mInputDevice->isKeyDown((Ogre::KeyCode)keymap["carAccelerate"]),
				mInputDevice->isKeyDown((Ogre::KeyCode)keymap["carBrake"])  );
			controlObject->ODEVehicle()->update(delaySeconds);
		}


		if (mInputDevice->isKeyDown((Ogre::KeyCode)keymap["camLeft"]))
        {
            // Move camera left
            mTranslateVector.x = -mMoveScale;
        }

        if (mInputDevice->isKeyDown((Ogre::KeyCode)keymap["camRight"]))
        {
            // Move camera RIGHT
            mTranslateVector.x = mMoveScale;
        }

        /* Move camera forward by keypress. */
        if (mInputDevice->isKeyDown((Ogre::KeyCode)Ogre::KC_UP) ||
        	mInputDevice->isKeyDown((Ogre::KeyCode)keymap["camForward"]) )
        {
            mTranslateVector.z = -mMoveScale;
        }

        /* Move camera backward by keypress. */
        if (mInputDevice->isKeyDown((Ogre::KeyCode)Ogre::KC_DOWN) ||
        	mInputDevice->isKeyDown((Ogre::KeyCode)keymap["camBackward"]) )
        {
            mTranslateVector.z = mMoveScale;
        }

        if (mInputDevice->isKeyDown((Ogre::KeyCode)keymap["camUp"]))
        {
            // Move camera up
            mTranslateVector.y = mMoveScale;
        }

        if (mInputDevice->isKeyDown((Ogre::KeyCode)keymap["camDown"]))
        {
            // Move camera down
            mTranslateVector.y = -mMoveScale;
        }

        if (mInputDevice->isKeyDown((Ogre::KeyCode)keymap["yawMinus"]))
        {
            mCamera->yaw(-mRotScale);
        }

        if (mInputDevice->isKeyDown((Ogre::KeyCode)keymap["yawPlus"]))
        {
            mCamera->yaw(mRotScale);
        }

/*
        if (mInputDevice->isKeyDown((Ogre::KeyCode)Ogre::KC_F))
        {
        }

        if (mInputDevice->isKeyDown((Ogre::KeyCode)Ogre::KC_T))
        {
        }
*/
        if (mInputDevice->isKeyDown((Ogre::KeyCode)keymap["screenshot"]))
        {
			static int mNumScreenShots=0;
			char tmp[20];
			sprintf(tmp, "screenshot_%d.png", ++mNumScreenShots);
			COgreTask::GetSingleton().mWindow->writeContentsToFile(tmp);
			COgreTask::GetSingleton().mWindow->setDebugText(String("Wrote ") + tmp);
        }
/*
		if (mInputDevice->isKeyDown((Ogre::KeyCode)Ogre::KC_R))
		{
		}

        if (mInputDevice->isKeyDown((Ogre::KeyCode)Ogre::KC_P))
        {
        }
*/


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

		mTranslateVectorTerrain = mCamera->getPosition();

		bool rval;

		if (mTerrain != NULL)
			rval = mTerrain->getHeight(mTranslateVectorTerrain);
		else
		{
			mTerrain = mLevel->Terrain();
			rval = false;
		}

		if (rval && (mCamera->getPosition().y < (mTranslateVectorTerrain.y + 10)))
		{
			mCamera->setPosition(mTranslateVectorTerrain.x,
				mTranslateVectorTerrain.y + 10,
				mTranslateVectorTerrain.z);
		}

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

	bool CUserInput::parseSettings(TiXmlElement* rootElem)
	{

		nrLog.Log(NR_LOG_APP, "CUserInput::parseSettings(): Start parsing the configuration");

		TiXmlElement *elem 	= NULL;
		TiXmlElement *subElem 	= NULL;

		unsigned int keyCommand;

		// read game specific keyboard settings
		elem = rootElem->FirstChildElement("gameInput");



		if (elem)
		{
			// configuring map for special game input
			subElem = elem->FirstChildElement("escape");
			const char *escapestr = subElem->GetText();

			keyCommand = escapestr ? axtoi( (escapestr)) : (unsigned int)Ogre::KC_ESCAPE;
			/*
				std::cout << "escape  (parse)  "<< keyCommand <<"\n";
			*/

			keymap["escape"] = keyCommand;

			subElem = elem->FirstChildElement("screenshot");
			const char *screenshotstr = subElem->GetText();
			keyCommand = screenshotstr ? axtoi( (screenshotstr)) : (unsigned int)Ogre::KC_C;


			keymap["screenshot"] = keyCommand;
		}
		else return false;


		// read keyboard steering settings
		elem = rootElem->FirstChildElement("steeringInput");
		if (elem)
		{
			// configuring map for keyborad steering settings
			subElem = elem->FirstChildElement("camLeft");
			const char *camLeftstr = subElem->GetText();
			keyCommand = camLeftstr ? axtoi( (camLeftstr)) : (unsigned int)Ogre::KC_A;

			keymap["camLeft"] = keyCommand;

			subElem = elem->FirstChildElement("camRight");
			const char *camRightstr = subElem->GetText();
			keyCommand = camRightstr ? axtoi( (camRightstr)) : (unsigned int)Ogre::KC_D;
			/*
				std::cout << "camRightstr  (parse)  "<< std::atoi(camRightstr) <<"\n";
			*/
			keymap["camRight"] = keyCommand;

			subElem = elem->FirstChildElement("camForward");
			const char *camForwardstr = subElem->GetText();
			keyCommand = camForwardstr ? axtoi( (camForwardstr)) : (unsigned int)Ogre::KC_W;

			keymap["camForward"] = keyCommand;

			subElem = elem->FirstChildElement("camBackward");
			const char *camBackwardstr = subElem->GetText();
			keyCommand = camBackwardstr ? axtoi( (camBackwardstr)) : (unsigned int)Ogre::KC_S;

			keymap["camBackward"] = keyCommand;

			subElem = elem->FirstChildElement("camUp");
			const char *camUpstr = subElem->GetText();
			keyCommand = camUpstr ? axtoi( (camUpstr)) : (unsigned int)Ogre::KC_PGUP;

			keymap["camUp"] = keyCommand;

			subElem = elem->FirstChildElement("camDown");
			const char *camDownstr = subElem->GetText();
			keyCommand = camDownstr ? axtoi( (camDownstr)) : (unsigned int)Ogre::KC_PGDOWN;

			keymap["camDown"] = keyCommand;

			subElem = elem->FirstChildElement("yawMinus");
			const char *yawMinusstr = subElem->GetText();
			keyCommand = yawMinusstr ? axtoi( (yawMinusstr)) : (unsigned int)Ogre::KC_RIGHT;

			keymap["yawMinus"] = keyCommand;

			subElem = elem->FirstChildElement("yawPlus");
			const char *yawPlusstr = subElem->GetText();
			keyCommand = yawPlusstr ? axtoi( (yawPlusstr)) : (unsigned int)Ogre::KC_LEFT;

			keymap["yawPlus"] = keyCommand;


			//car control
			subElem = elem->FirstChildElement("carAccelerate");
			const char *carAcceleratestr = subElem->GetText();
			keyCommand = carAcceleratestr ? axtoi( (carAcceleratestr)) : (unsigned int)Ogre::KC_I;

			keymap["carAccelerate"] = keyCommand;


			subElem = elem->FirstChildElement("carBrake");
			const char *carBrakestr = subElem->GetText();
			keyCommand = carBrakestr ? axtoi( (carBrakestr)) : (unsigned int)Ogre::KC_K;

			keymap["carBrake"] = keyCommand;


			subElem = elem->FirstChildElement("carLeft");
			const char *carLeftstr = subElem->GetText();
			keyCommand = carLeftstr ? axtoi( (carLeftstr)) : (unsigned int)Ogre::KC_J;

			keymap["carLeft"] = keyCommand;


			subElem = elem->FirstChildElement("carRight");
			const char *carRightstr = subElem->GetText();
			keyCommand = carRightstr ? axtoi( (carRightstr)) : (unsigned int)Ogre::KC_L;

			keymap["carRight"] = keyCommand;

		}
		else return false;


		// read mouse input settings
		elem = rootElem->FirstChildElement("mouseInput");
		if (elem)
		{
			// configuring map for mouse input
		}
		else return false;


		nrLog.Log(NR_LOG_APP, "CUserInput::parseSettings(): Stop parsing the configuration");
		return true;
	}

	bool CUserInput::importFromFile(const char* fileName)
	{

		// we open the file for parsing.
		// Later we can open the file through the virtual file system if we had got more time
		// for development
		boost::shared_ptr<TiXmlDocument> mLevelDoc (new TiXmlDocument(fileName));
		if (!mLevelDoc->LoadFile())
		{
			nrLog.Log(NR_LOG_APP, "CUserInput::importFromFile(): There is an error occurs by loading the configuration file \"%s\"", fileName);
			return false;
		}

		// Load elements form the config file and handle with them in according way
		TiXmlElement* elem = NULL;
		TiXmlElement* rootElem;
		rootElem = mLevelDoc->FirstChildElement("userInput");

		if (!rootElem)
		{
			nrLog.Log(NR_LOG_APP, "CUserInput::importFromFile(): The configuration file is corrupted - No <userInput> Tag was found !");
			return false;
		}
		return parseSettings(rootElem);
	}



	unsigned int CUserInput::axtoi(const char *hexStg)
	{
		int n = 2;         // position in string
		int m = 0;         // position in digit[] to shift
		int count;         // loop index

		unsigned int intValue = 0;  // integer value of hex string

		int digit[5];      // hold values to convert

		while (n < 4)
		{
			 if (hexStg[n]=='\0')
				break;
			 if (hexStg[n] > 0x29 && hexStg[n] < 0x40 ) //if 0 to 9
				digit[n] = hexStg[n] & 0x0f;            //convert to int
			 else if (hexStg[n] >='a' && hexStg[n] <= 'f') //if a to f
				digit[n] = (hexStg[n] & 0x0f) + 9;      //convert to int
			 else if (hexStg[n] >='A' && hexStg[n] <= 'F') //if A to F
				digit[n] = (hexStg[n] & 0x0f) + 9;      //convert to int
			 else break;
			n++;
		}

		count = n;
		m = n - 1;
		n = 0;

		while(n < count)
		{
			// digit[n] is value of hex digit at position n
			// (m << 2) is the number of positions to shift
			// OR the bits into return value
			intValue = intValue | (digit[n] << (m << 2));
			m--;   // adjust the position to set
			n++;   // next digit to process
		}

		return (intValue);
	}

}	//namespace stunts




