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

#define GCC_3_1
#define EXT_HASH

#include <Ogre.h>
#include <nrEngine/nrEngine.h>

#include "FrameListener.hpp"
//#include "UserInput.hpp"
#include "PhysicsWorld.hpp"

#ifndef _STUNTS_GAME_APPLICATION_HPP_
#define _STUNTS_GAME_APPLICATION_HPP_


using namespace Ogre;

namespace stunts {

  /** The main application of the game.
   */
  class GameApplication {
  public:
    /** Construct a new %GameApplication.
     */
    GameApplication();

    /** Destructor.
     */
    virtual ~GameApplication();

    /** Run the application.
     */
    virtual void run();

    // These internal methods package up the stages in the startup process
    /** Sets up the application - returns false if the user chooses to
  	    abandon configuration. */
    virtual bool initialize();

    /** Show configure dialog
     */
    virtual bool configure();

    /** choose scene manager
     */
    virtual void chooseSceneManager();

    /** create camera
     */
    virtual void createCamera();

    /** create Frame Listener
     */
    virtual void createFrameListener();

    /** create Scene
     */
    virtual void createScene();

    /** destroy Scene
     */
    virtual void destroyScene();

    /** create Viewports
     */
    virtual void createViewports();

    /// Method which will define the source of resources (other than current folder)
    virtual void setupResources();

	/// Optional override method where you can create resource listeners (e.g. for loading screens)
	virtual void createResourceListener();

	/// Optional override method where you can perform resource group loading
	/// Must at least do ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	virtual void loadResources();

	/** Quit signal from nrEngine
	*/
    static void quit(void* p);

  private:
    Root* mRoot;
    Camera* mCamera;
    SceneManager* mSceneMgr;
    RenderWindow* mWindow;
    CFrameListener* mFrameListener;

  };

}	//namespace

#endif

