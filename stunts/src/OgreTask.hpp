/* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup, 
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Art Tevs
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

#include <OGRE/Ogre.h>
#include <nrEngine/nrEngine.h>
#include "FrameListener.hpp"

#ifndef _OGRE_NRENGINE_TASK_HPP_
#define _OGRE_NRENGINE_TASK_HPP_

using namespace Ogre;


namespace stunts {
  
	/**
	* This is a class derived from engine's task class. This allows us
	* to run the ogre rendering engine
	* as a task in the nrEngine
	*/
	class COgreTask: public nrITask {
	public:

		// Constr & Destr
		COgreTask();
		virtual ~COgreTask();
		
		// Task Functions
		nrResult taskInit();
		nrResult taskStart();
		nrResult taskUpdate();
		nrResult taskStop();
		
		const char* taskGetName(){return "OgreTask";}

	private:
		bool configure();
		void createCamera();
		void createFrameListener();
		void createScene();
		void destroyScene();
		void createViewports();
		void setupResources();

		Root* 			mRoot;
		Camera* 		mCamera;
		SceneManager* 	mSceneMgr;
		RenderWindow* 	mWindow;
		CFrameListener* mFrameListener;
	};
  
}

#endif
