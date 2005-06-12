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


#include "OgreTask.hpp"

using namespace Ogre;

namespace stunts {

	/**
	* Constructor
	**/
	COgreTask::COgreTask(){
		//reset attributes
    	mRoot = NULL;
    	//mCamera = NULL;
    	//mSceneMgr = NULL;
    	mWindow = NULL;
    	//mFrameListener = NULL;
   	}
	
	/**
	* Destructor
	**/
	COgreTask::~COgreTask(){
	
		//delete OGRE root
		NR_SAFE_DELETE(mRoot);
	
	}
	
	
	/**
	* Initialize Ogre Engine
	**/
	nrResult COgreTask::taskInit(){

		// Setup Ogre-Engine
        mRoot = new Root();
		if (!configure())return NR_UNKNOWN_ERROR;

		// Load resources and 
		setupResources();

		// Create camera and viewports
		createCamera();
        createViewports();


		//setup input class
		mInputDevice  =	
			boost::shared_ptr< Ogre::InputReader >
				(PlatformManager::getSingleton().createInputReader());
		mInputDevice->initialise(mWindow,true, true);


 		// Create the scene
        createScene();
				
		return NR_OK;
	}
	
	
	/**
	* Start the Ogre Engine
	**/
	nrResult COgreTask::taskStart()
	{

		// create the timer interface
		mTimer.reset (new nrCTimer(nrCClock::GetSingleton()));
		nrCClock::GetSingleton().addObserver(mTimer);
		
		// OK
		return NR_OK;
	}
	
	
	/**
	* Update the Ogre Engine
	**/
	nrResult COgreTask::taskUpdate()
	{
		// render one frame and check if Ogre should quit
		if (!mRoot->renderOneFrame()){
			nrKernel.KillAllTasks();
		}

		return NR_OK;
	}
	
	
	/**
	* Stop Ogre
	**/
	nrResult COgreTask::taskStop()
	{

		// clean up
		destroyScene();
				
		return NR_OK;
	}


	/**
	 * Create the scene and load all used config files
	 **/
	void COgreTask::createScene()
	{

        // Set ambient light
        mSceneMgr->setAmbientLight(ColourValue(0.0, 0.0, 0.0));

        // Create a light
        //Light* l = mSceneMgr->createLight("MainLight");
        //l->setPosition(20,80,50);

        // Fog
        //ColourValue fadeColour(0.93, 0.86, 0.76);
        //mSceneMgr->setFog( FOG_LINEAR, fadeColour, .001, 500, 1000);
        //mWindow->getViewport(0)->setBackgroundColour(fadeColour);

		// Load game data
        mSceneMgr -> setWorldGeometry("../config/terrain.cfg");
		
        // Infinite far plane?
        if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(RSC_INFINITE_FAR_PLANE))
        {
            mCamera->setFarClipDistance(0);
        }

        // Set a nice viewpoint
        mCamera->setPosition(707,100,528);
        mCamera->setOrientation(Quaternion(-0.3486, 0.0122, 0.9365, 0.0329));

	}

	
	/**
	 * Destroy the scene
	 **/
	void COgreTask::destroyScene()
	{

	}


	/**
	 * Configure Ogre-Engine for rendering
	 **/
    bool COgreTask::configure()
    {

		bool ok = mRoot->restoreConfig();
		//bool ok = mRoot->showConfigDialog();
		
		//attention: plugins.cfg must be in the root folder or
		//	a segfault will occur!
		if (ok)
        {
			mWindow = mRoot->initialise(true, "Stunts2005");
        }
		// Create scene manager
		mSceneMgr = boost::shared_ptr< Ogre::SceneManager >
			(mRoot->getSceneManager(ST_EXTERIOR_CLOSE));
        // Set default mipmap level (NB some APIs ignore this)
        TextureManager::getSingleton().setDefaultNumMipmaps(5);
		
		// Get Render System
		mRenderer = mRoot->getRenderSystem();

		// Setup Rendering Options
		//mRenderer->setShadingType(SO_GOURAUD);
		//mRenderer->setLightingEnabled(true);
		//mRenderer->setNormaliseNormals(true);

		return ok;
    }


	/**
	 * Create the camera for our player
	 **/
    void COgreTask::createCamera()
    {
        // Create the camera
        mCamera = boost::shared_ptr< Ogre::Camera >
        	(mSceneMgr->createCamera("PlayerCam"));

        // Position it at 500 in Z direction
        mCamera->setPosition(Vector3(128,25,128));
        // Look back along -Z
        mCamera->lookAt(Vector3(0,0,-300));
        mCamera->setNearClipDistance( 1 );
        mCamera->setFarClipDistance( 1000 );
    }




	/**
	 * Create a viewport for our camera
	 **/
	void COgreTask::createViewports()
    {
        // Create one viewport, entire window
        Viewport* vp = mWindow->addViewport(mCamera.get());
        vp->setBackgroundColour(ColourValue(0,0,0));

        // Alter the camera aspect ratio to match the viewport
        mCamera->setAspectRatio(
            Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
    }

	
	/**
	 * Load all resources needed for our game/scene
	 **/
    void COgreTask::setupResources()
    {
        // Load resource paths from config file
        ConfigFile cf;
        cf.load("../config/resources.cfg");

        // Go through all sections & settings in the file
        ConfigFile::SectionIterator seci = cf.getSectionIterator();
        String secName, typeName, archName;
        while (seci.hasMoreElements())
        {
            secName = seci.peekNextKey();
            ConfigFile::SettingsMultiMap *settings = seci.getNext();
            ConfigFile::SettingsMultiMap::iterator i;
            for (i = settings->begin(); i != settings->end(); ++i)
            {
                typeName = i->first;
                archName = i->second;
                ResourceGroupManager::getSingleton().addResourceLocation(
                    archName, typeName, secName);
            }
        }
		
		// Initialise, parse scripts etc
		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    }

}

