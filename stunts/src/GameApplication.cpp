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
#include "LevelManager.hpp"
#indlude "CSoundSystem.hpp"

#include "OgreTimer.hpp"
#include "CGuiTask.h"

using boost::shared_ptr;


namespace stunts
{
	GameApplication::GameApplication()
	{

	}


	GameApplication::~GameApplication()
	{
		CGuiTask::Release();
		CLevelManager::Release();
		COgreTask::Release();
		CSoundSystem::Release();

		// kill all tasks if there are any
		nrKernel.KillAllTasks();

		//delete nrEngine singleton
		nrEngineDelete();
	}


	void GameApplication::run()
	{
		//go into rendering loop
		nrKernel.Execute();
	}


	/**
	 * Initialize the application
	 **/
	bool GameApplication::initialize()
	{
		//create nrEngine singleton
		nrEngineInit();

		//Logging
		nrLog.Init("../log/", "");
		nrLog.Log(NR_LOG_APP, "Application started");

		// Initialize virtual file system
		nrVFS.setPathToVFS("../");
		if (nrVFS.openFileSystem() != NR_OK){
			printf("VFS Error !!!\n");
			return false;
		}

		//create tasks
		COgreTask::Instantiate();

		// Add singletons to the kernel
		COgreTask::GetSingleton().AddToKernel(nrKernel, NR_PRIORITY_LAST);

		// Setup InGame-Clock
		shared_ptr<nrITimeSource> timer (new COgreTimer());
		nrCClock::GetSingleton().setTimeSource(timer);

		// Add clock to the kernel. The priority must be first, because we
		// want use timer before Ogre
		nrCClock::GetSingleton().AddToKernel(nrKernel, NR_PRIORITY_FIRST);

		// Show intro
		showIntro();
		
		// Create Level Manager
		CLevelManager::Instantiate();
		CLevelManager::GetSingleton().loadLevelDescriptions("../media/level/content.xml");
		CLevelManager::GetSingleton().AddToKernel(nrKernel, NR_PRIORITY_LOW);


		// Create SoundSystem
		CSoundSystem::Instantiate();
		CSoundSystem::GetSingleton().AddToKernel(nrKernel, NR_PRIORITY_HIGH);


		// add GUI task
		CGuiTask::Instantiate();
		CGuiTask::GetSingleton().setWindow(COgreTask::GetSingleton().mWindow);
		CGuiTask::GetSingleton().setSceneManager(COgreTask::GetSingleton().mSceneMgr);
		CGuiTask::GetSingleton().AddToKernel(nrKernel, NR_PRIORITY_ULTRA_LOW);

		// this is how you load a page!
		CGuiTask::GetSingleton().addPage( "Main", "" );
		CGuiTask::GetSingleton().addPage( "MainLevel", "" );
		CGuiTask::GetSingleton().addPage( "MainOption", "" );
		
		CGuiTask::GetSingleton().addPage( "InGameOption", "" );
		CGuiTask::GetSingleton().addPage( "InGame", "" );
		CGuiTask::GetSingleton().addPage( "InGameLevel", "" );
		
		CGuiTask::GetSingleton().selectPage( "Main" );
		CGuiTask::GetSingleton().rActive() = true;


		// set level variables. JUST FOR TESTING, this should be done by GUI
		nrSettings.get("level_file") = std::string("The Default Level");
		nrSettings.get("load_level") = std::string("0");

        return true;
	}



	/**
	 * Quit the application
	 **/
	void GameApplication::quit(void* p)
	{
		nrLog.Log(NR_LOG_APP, "Stunts::quit(): Quit the application");
		nrKernel.KillAllTasks();
	}


	/**
	 * Shows the Ogre-Log on the screen for a some time
	 **/
	void GameApplication::showIntro()
	{
		using namespace Ogre;

		Overlay* ov = OverlayManager::getSingleton().getByName("Stunts/Intro");
		if (ov)
		{
			// show the Ogre-logo
			ov->show();

			// now get the material for the logo
			MaterialPtr mat = MaterialManager::getSingleton().getByName("Stunts/IntroMaterial");
			TextureUnitState* state = NULL;
	
			if (mat.get() != NULL){
				state = mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
			}
			
			// create a timer
			Timer* timer = PlatformManager::getSingleton().createTimer();
			if (timer && state)
			{
				// now hold on for some time
				unsigned long stopTime = timer->getMilliseconds() + 800;
				unsigned long startTime = timer->getMilliseconds();

				// Fade IN				
				while (timer->getMilliseconds() < stopTime)
				{
					float value = float(timer->getMilliseconds() - startTime) / float(stopTime - startTime);
					
					state->setAlphaOperation(LBX_MODULATE,LBS_TEXTURE,LBS_MANUAL, 1.0, value);
					COgreTask::GetSingleton().mRoot->renderOneFrame();
				}

				// pause
				stopTime = timer->getMilliseconds() + 1500;
				while (timer->getMilliseconds() < stopTime)
				{
					COgreTask::GetSingleton().mRoot->renderOneFrame();
				}

				// Fade Out
				stopTime = timer->getMilliseconds() + 800;
				startTime = timer->getMilliseconds();
				while (timer->getMilliseconds() < stopTime)
				{
					float value = float(timer->getMilliseconds() - startTime) / float(stopTime - startTime);
					
					state->setAlphaOperation(LBX_MODULATE,LBS_TEXTURE,LBS_MANUAL, 1.0, 1.0f - value);
					COgreTask::GetSingleton().mRoot->renderOneFrame();
				}
				
				// destroy the timer
				PlatformManager::getSingleton().destroyTimer(timer);
				
			}
			// now cleanup data
			OverlayManager::getSingleton().destroy(ov);
			
		}
		
	}

}
