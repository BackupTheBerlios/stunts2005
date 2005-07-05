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

namespace stunts {

	using namespace Ogre;

	/**
	* Constructor
	**/
	COgreTask::COgreTask(){
		//reset attributes
    	mRoot = NULL;
    	mCamera = NULL;
    	mSceneMgr = NULL;
    	mInputDevice = NULL;
    	mWindow = NULL;
    	//mFrameListener = NULL;
   	}

	/**
	* Destructor
	**/
	COgreTask::~COgreTask(){

		NR_SAFE_DELETE(mRoot);

	}


	/**
	* Initialize Ogre Engine
	**/
	nrResult COgreTask::taskInit(){

		// Setup Ogre-Engine
        mRoot = new Root();
		if (!configure())
			return NR_UNKNOWN_ERROR;

		// Load resources and
		setupResources();

		// Create camera and viewports
		createCamera();
        createViewports();


		//setup input class
		//mInputDevice.reset(PlatformManager::getSingleton().createInputReader());
		mInputDevice = PlatformManager::getSingleton().createInputReader();
		mInputDevice->initialise(mWindow,true, true);


 		// Create the scene
        createScene();

		// show the logo
		//showIntro();

		// show intro from nrEngine		
		//shownrEngineIntro();
		
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
		//	nrKernel.KillAllTasks();
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

        // Infinite far plane?
        if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(RSC_INFINITE_FAR_PLANE))
        {
            mCamera->setFarClipDistance(0);
        }

        // Set a nice viewpoint
        mCamera->setPosition(707,100,528);
        mCamera->setOrientation(Quaternion(-0.3486, 0.0122, 0.9365, 0.0329));

		//set shadows
		mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
		mSceneMgr->setShadowColour(ColourValue(0.5,0.5,0.5));
		mSceneMgr->setShadowFarDistance(5000.0f);
		mSceneMgr->setShadowUseInfiniteFarPlane(true);

	}


	/**
	 * Destroy the scene
	 **/
	void COgreTask::destroyScene()
	{

		mSceneMgr->clearScene();
		mSceneMgr->removeAllCameras();
		mRoot->getAutoCreatedWindow()->removeAllViewports();

	}


	/**
	 * Configure Ogre-Engine for rendering
	 **/
	bool COgreTask::configure()
	{

		bool ok = mRoot->restoreConfig();

		//attention: plugins.cfg must be in the root folder or
		//	a segfault will occur!
		if (ok)
		{
			mWindow = mRoot->initialise(true, "Stunts2005");
		}
		// Create scene manager
		mSceneMgr = mRoot->getSceneManager(ST_EXTERIOR_CLOSE);

		// Get Render System
		mRenderer = mRoot->getRenderSystem();

		// Setup boundig box padding value
		MeshManager::getSingleton().setBoundsPaddingFactor(0.0f);

		// Set default mipmap level (NB some APIs ignore this)
		TextureManager::getSingleton().setDefaultNumMipmaps(10);

		return ok;
	}


	/**
	 * Create the camera for our player
	 **/
	void COgreTask::createCamera()
	{
	        // Create the camera
	        //mCamera.reset(mSceneMgr->createCamera("PlayerCam"));
	        mCamera = mSceneMgr->createCamera("PlayerCam");

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
	        Viewport* vp = mWindow->addViewport(mCamera);
	        vp->setBackgroundColour(ColourValue(0,0,0));

	        // Alter the camera aspect ratio to match the viewport
	        mCamera->setAspectRatio(
	            Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

			vp->setOverlaysEnabled(true);
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

	/**
	 * Shows the Ogre-Log on the screen for a some time
	 **/
	void COgreTask::showIntro()
	{
		using namespace Ogre;

		Overlay* ov = OverlayManager::getSingleton().getByName("Ogre/Intro");
		if (ov)
		{
			// show the Ogre-logo
			ov->show();

			// now get the material for the logo
			MaterialPtr mat = MaterialManager::getSingleton().getByName("Ogre/IntroMaterial");
			TextureUnitState* state = NULL;
	
			if (mat.get() != NULL){
				state = mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
			}
			
			// create a timer
			Timer* timer = PlatformManager::getSingleton().createTimer();
			if (timer && state)
			{
				// now hold on for some time
				unsigned long stopTime = timer->getMilliseconds() + 700;
				unsigned long startTime = timer->getMilliseconds();

				// Fade IN				
				while (timer->getMilliseconds() < stopTime)
				{
					float value = float(timer->getMilliseconds() - startTime) / float(stopTime - startTime);
					
					state->setAlphaOperation(LBX_MODULATE,LBS_TEXTURE,LBS_MANUAL, 1.0, value);
					mRoot->renderOneFrame();
				}

				// pause
				stopTime = timer->getMilliseconds() + 1500;
				while (timer->getMilliseconds() < stopTime)
				{
					mRoot->renderOneFrame();
				}

				// Fade Out
				stopTime = timer->getMilliseconds() + 700;
				startTime = timer->getMilliseconds();
				while (timer->getMilliseconds() < stopTime)
				{
					float value = float(timer->getMilliseconds() - startTime) / float(stopTime - startTime);
					
					state->setAlphaOperation(LBX_MODULATE,LBS_TEXTURE,LBS_MANUAL, 1.0, 1.0f - value);
					mRoot->renderOneFrame();
				}
				
				// destroy the timer
				PlatformManager::getSingleton().destroyTimer(timer);
				
			}
			// now cleanup data
			OverlayManager::getSingleton().destroy(ov);
			
		}
		
	}


	/**
	 * Shows intro of nrEngine
	 **/
	void COgreTask::shownrEngineIntro()
	{
		using namespace Ogre;

		Overlay* ov = OverlayManager::getSingleton().getByName("nrEngine/Intro");
		if (ov)
		{
			// show the Ogre-logo
			ov->show();

			// now get the material for the logo
			MaterialPtr mat = MaterialManager::getSingleton().getByName("nrEngine/IntroMaterial");
			TextureUnitState* state = NULL;
	
			if (mat.get() != NULL){
				state = mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
			}
			
			// create a timer
			Timer* timer = PlatformManager::getSingleton().createTimer();
			if (timer && state)
			{
				// now hold on for some time
				unsigned long stopTime = timer->getMilliseconds() + 1000;
				unsigned long startTime = timer->getMilliseconds();
				
				while (timer->getMilliseconds() < stopTime)
				{
					float value = float(timer->getMilliseconds() - startTime) / float(stopTime - startTime);
					
					state->setAlphaOperation(LBX_MODULATE,LBS_TEXTURE,LBS_MANUAL, 1.0, value);
					mRoot->renderOneFrame();
				}

				while (timer->getMilliseconds() < stopTime + 1500)
				{
					mRoot->renderOneFrame();
				}
								
				// destroy the timer
				PlatformManager::getSingleton().destroyTimer(timer);
				
			}
			// now cleanup data
			OverlayManager::getSingleton().destroy(ov);
			
		}
		
	}

}

