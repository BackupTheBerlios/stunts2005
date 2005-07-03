/* CVS $Id: CGuiTask.h,v 1.7 2005/07/03 11:21:03 psyborg Exp $ */

/** @file
 *  Main GUI task and manager for Stunts 2005.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.7 $
 *  @date    CVS $Date: 2005/07/03 11:21:03 $
 */


#ifndef __C_GUI_TASK_H
#define __C_GUI_TASK_H


#include <nrEngine/nrEngine.h>

#include "CGuiPage.h"
#include "CGuiParser.h" // parser is actually used here

#include <iostream>

#include <string>
#include <map>

#include <OGRE/Ogre.h>
#include <OGRE/OgreKeyEvent.h>
#include <OGRE/OgreEventListeners.h>

/**
 * Declare this class as forward declaration here. I do not include any CEGUI, files
 * here, because of include conflicts with other files. The needed files will be included
 * in *.cpp file.
 **/
namespace CEGUI{
	class Renderer;
	class System;
	class EventArgs;
	class Imageset;
	class OgreCEGUIRenderer;
}


namespace stunts
{

	/** Main GUI task and manager.
	*  This class provides functions for loading and activating GUI pages and
	*  additionally implements the nrITask interface for direct use within the
	*  newReality game engine.
	*
	*  @par This class was also derived from Ogre's Mouse-Listeners, so we can get data about mouse moving
	*/
	class CGuiTask : public nrISingletonTask<CGuiTask>,  public Ogre::MouseMotionListener, public Ogre::MouseListener, public Ogre::KeyListener {
	public:
	
		/** Default constructor for the GuiTask. The task will intialize the underlying GUI-SubSystem
		* by itself
		**/
		CGuiTask();
	
		/** Destructor. */
		virtual ~CGuiTask();
	
		/** Add new GUI page.
		*  @param  PageName  Name by which new page will be accessible.
		*  @param  Source    XML string data to generate page from.
		*  @return @c true if page was successfully loaded, otherwise @c false
		*/
		bool addPage( std::string PageName, std::string Source );
	
		/** Select different GUI page.
		*  @param  PageName  Name of page to be selected.
		*  @return @c true if page existed, otherwise @c false.
		*/
		bool selectPage( std::string PageName );
	
		/** Delete GUI page.
		*  @param  PageName  Name of page to be deleted.
		*  @param  Force     If @c true, delete even if page is active.
		*  @return @c true if page was deleted, otherwise @c false.
		*/
		bool deletePage( std::string PageName, bool Force = false );
				
		/* The following methods are required by nrITask interface. */
		nrResult taskInit();
		nrResult taskStart();
		nrResult taskUpdate();
		nrResult taskStop();
		const char* taskGetName() { return "CGuiTask"; }
	
		/* The following methods provide access to values of and/or references to
		* private class attributes where suitable. */
		bool &rActive() { return mActive; }
		bool   Active() { return mActive; }

		void showGUI();
		void hideGUI();
		
		/* Mouse Listener interface */
		void mouseMoved (Ogre::MouseEvent *e);
		void mouseDragged (Ogre::MouseEvent *e);
	
		void mouseClicked(Ogre::MouseEvent* e) {}
		void mouseEntered(Ogre::MouseEvent* e) {}
		void mouseExited(Ogre::MouseEvent* e) {}
		void mousePressed (Ogre::MouseEvent *e);
		void mouseReleased (Ogre::MouseEvent *e);
	
		/* Keyboard listener interface */
		void keyClicked (Ogre::KeyEvent *e);
		void keyPressed (Ogre::KeyEvent *e);
		void keyReleased (Ogre::KeyEvent *e);
	
		/* Setup methods for Ogre's Engine */
		void setWindow (Ogre::RenderWindow* window);
		void setSceneManager(Ogre::SceneManager* mgr);

		/* Event Handler */
		bool handleQuit(const CEGUI::EventArgs& e);
		bool handleStart(const CEGUI::EventArgs& e);
		bool handleUnload(const CEGUI::EventArgs& e);
		bool handleLevel(const CEGUI::EventArgs& e);
		bool handleLevelOK(const CEGUI::EventArgs& e);
		bool handleLevelSelected(const CEGUI::EventArgs& e);
		
	private:
	
		bool mActive; /**< GUI manager activated flag */
	
		std::map<std::string, CGuiPage*> mPages; /**< Pages accessible by name */
		CGuiPage* mCurrent;                      /**< currently selected GUI page */
	
		
		CEGUI::OgreCEGUIRenderer* 	mGUIRenderer; /**< Gui Renderer from the CEGUI-Library */
		CEGUI::System*	 	mGUISystem;
		
		
		// This objects are needed to get input data from the Ogre-Engine
		Ogre::EventProcessor* 	mEventProcessor;
		Ogre::RenderWindow*		mWindow;
		Ogre::SceneManager*		mSceneMgr;
		
		//! Here we store all used images in our Gui like Screenshots
		std::vector<CEGUI::Imageset*>	mImageSets;
		
	};

};

#endif

