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


#ifndef _CUSERINPUT_H_
#define _CUSERINPUT_H_

//------------------------------------------------------------------------------
//--- predeclare this class
//------------------------------------------------------------------------------
namespace stunts
{
	class CUserInput;
}

//------------------------------------------------------------------------------
//--- includes
//------------------------------------------------------------------------------

#include <OGRE/Ogre.h>
#include <OGRE/OgreKeyEvent.h>
#include <OGRE/OgreEventListeners.h>
#include <OGRE/OgreStringConverter.h>
#include <OGRE/OgreException.h>

#include <nrEngine/nrEngine.h>

#include "Level.hpp"
#include "Terrain.hpp"

#include <nrEngine/nrEngine.h>

namespace stunts
{

	class CUserInput : public nrITask
	{
		public:
			CUserInput(boost::shared_ptr< CLevel > level);
			virtual ~CUserInput();
			void react(float delaySeconds);
			void activate(bool activated);
			bool isActivated() const;
			virtual const char* taskGetName() {return "UserInput";}


		protected:
			virtual nrResult taskInit();
			virtual nrResult taskStart();
			virtual nrResult taskUpdate();
			virtual nrResult taskStop();


			boost::shared_ptr< CLevel >				mLevel;

			boost::shared_ptr< Ogre::InputReader >	mInputDevice;
			boost::shared_ptr< Ogre::Camera >		mCamera;
			boost::shared_ptr< CTerrain >			mTerrain;

			bool mActivated;

	};
}	//namespace stunts

#endif	//_CUSERINPUT_H_
