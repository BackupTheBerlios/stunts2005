/* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup,
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Christian Morbach <GameDevelopment@paratronic.de>
 *                    Andreas Maurer <andi@andile.de>
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

#include <map>

namespace stunts
{

	class CUserInput : public nrITask
	{
		public:
			CUserInput(CLevel* level);
			virtual ~CUserInput();
			void react(float delaySeconds);
			void activate(bool activated);
			bool isActivated() const;
			virtual const char* taskGetName() {return "UserInput";}

			/**
		 	* Import the atmosphere from a file.
		 	* @param fileName Name of the file containing terrain data.
		 	* @param xmlP
		 	* @return false if error occurs otherwise true
		 	**/
			virtual bool importFromFile(const char* fileName);


		protected:
			virtual nrResult taskInit();
			virtual nrResult taskStart();
			virtual nrResult taskUpdate();
			virtual nrResult taskStop();


			CLevel*				mLevel;
			Ogre::InputReader*		mInputDevice;
			Ogre::Camera*			mCamera;

			boost::shared_ptr< CTerrain >	mTerrain;


			// declare empty container (string -> map)
			typedef std::map<std::string, unsigned int> inputmap;
			// create empty container
			inputmap keymap;


			bool mActivated;


			/**
		 	* Parse config settings from a given XML-Tree.
		 	* @param rootElem Root element of the XML-Tree (here: atmosphere - Tag)
		 	* @param xmlPath path of this XML-File, for importing
		 	* @return true if error occurs
		 	**/
			bool parseSettings(TiXmlElement* rootElem);

			/**
			* Configuring the map object to map the input keys in the .xml configuration file to userinput
			**/
			bool configMap();


			unsigned int axtoi(const char *hexStg);

			//human player car
			boost::shared_ptr<CCarObject>					controlObject;
			

	};
}	//namespace stunts

#endif	//_CUSERINPUT_H_
