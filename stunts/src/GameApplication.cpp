/* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup,
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Florian Winter <fw@graphics.cs.uni-sb.de>
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


#include "GameApplication.hpp"

namespace stunts
{
	GameApplication::GameApplication()
	{
		//reset attributes
        mFrameListener = 0;
		mRoot = 0;
    	mRoot = NULL;
    	mCamera = NULL;
    	mSceneMgr = NULL;
    	mWindow = NULL;
    	mFrameListener = NULL;
	}


	GameApplication::~GameApplication()
	{
		//delete OGRE frame listener
		if (mFrameListener)
			delete mFrameListener;

		//delete OGRE root
		if (mRoot)
			delete mRoot;

		// kill all tasks if there are any
		nrKernel.KillAllTasks();

		//delete nrFramework singleton
		nrFrameworkDelete();

		//delete nrEngine singleton
		nrEngineDelete();
	}


	void GameApplication::run()
	{
		//go into rendering loop
        mRoot->startRendering();

		//run the game engine
		nrKernel.Execute();		//temporarily disabled to test OGRE
std::cout << "### kill !!" << std::endl;

		// clean up
		destroyScene();
	}


	bool GameApplication::initialize()
	{
		//#ifdef FALSE
		//create nrEngine singleton
		nrEngineInit();

		//Logging
		nrLog.Init("../log/","");
		nrLog.Log(NR_LOG_APP, "Application started");

		//create nrFramework singleton
		nrFrameworkInit();
		int handle = 0;
		nrResult ret = nrFramework.createRenderContext (&handle,800,600,32);
		if (ret != NR_OK){
			nrLog.Log(NR_LOG_APP, "Can not create rendering window");
			return false;
		}
		nrFramework.getRC()->changeWindowTitle("Stunts2005");
		nrFramework.setQuitCallback(GameApplication::quit, NULL);

		//add tasks
		boost::shared_ptr<CUserInput> user_input(new CUserInput());
		boost::shared_ptr<CPhysicsWorld> physics_world(new CPhysicsWorld());
		user_input->setTaskPriority(NR_PRIORITY_VERY_HIGH);
		nrKernel.AddTask(user_input);
		nrKernel.AddTask(physics_world);

		// add nrFramework to kernel
		nrFramework.AddToKernel(nrKernel, NR_PRIORITY_LAST);
		//#endif

		
        mRoot = new Root();

		setupResources();

		bool carryOn = configure();
		if (!carryOn)
			return false;

        chooseSceneManager();
        createCamera();
        createViewports();

        // Set default mipmap level (NB some APIs ignore this)
        TextureManager::getSingleton().setDefaultNumMipmaps(5);

		// Create any resource listeners (for loading screens)
		createResourceListener();
		// Load resources
		loadResources();

		// Create the scene
        createScene();

        createFrameListener();

        return true;
	}


	void GameApplication::createScene()
	{

	}


	void GameApplication::destroyScene()
	{

	}


    bool GameApplication::configure()
    {
        // Show the configuration dialog and initialise the system
        // You can skip this and use root.restoreConfig() to load configuration
        // settings if you were sure there are valid ones saved in ogre.cfg
		
		//attention: plugins.cfg must be in the root folder or
		//	a segfault will occur!
        if(mRoot->showConfigDialog())
        {
            // If returned true, user clicked OK so initialise
            // Here we choose to let the system create a default rendering
			// window by passing 'true'
            mWindow = mRoot->initialise(true);
            return true;
        }
        else
        {
            return false;
        }
    }


	void GameApplication::chooseSceneManager()
    {
		// Get the SceneManager, in this case a generic one
		mSceneMgr = mRoot->getSceneManager(ST_GENERIC);
    }


    void GameApplication::createCamera()
    {
        // Create the camera
        mCamera = mSceneMgr->createCamera("PlayerCam");

        // Position it at 500 in Z direction
        mCamera->setPosition(Vector3(0,0,500));
        // Look back along -Z
        mCamera->lookAt(Vector3(0,0,-300));
        mCamera->setNearClipDistance(5);
    }


	void GameApplication::createFrameListener()
    {
        mFrameListener= new CFrameListener(mWindow, mCamera);
        mFrameListener->showDebugOverlay(true);
        mRoot->addFrameListener(mFrameListener);
    }


	void GameApplication::createViewports()
    {
        // Create one viewport, entire window
        Viewport* vp = mWindow->addViewport(mCamera);
        vp->setBackgroundColour(ColourValue(0,0,0));

        // Alter the camera aspect ratio to match the viewport
        mCamera->setAspectRatio(
            Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
    }


    void GameApplication::setupResources()
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
    }


	void GameApplication::loadResources()
	{

	}


	void GameApplication::createResourceListener()
	{

	}


	void GameApplication::quit(void* p)
	{
		nrLog.Log(NR_LOG_APP, "Stunts::quit(): Quit the application");
		nrKernel.KillAllTasks();
	}

}
