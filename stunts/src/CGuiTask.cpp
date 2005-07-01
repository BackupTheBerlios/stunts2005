/* CVS $Id: CGuiTask.cpp,v 1.6 2005/07/01 19:38:38 psyborg Exp $ */

/** @file
 *  Main GUI task and manager for Stunts 2005.
 *
 *  @author  Markus Thiele
 *
 *  @version CVS $Revision: 1.6 $
 *  @date    CVS $Date: 2005/07/01 19:38:38 $
 */


#include "CGuiTask.h"
using std::istream;
using std::map;
using std::string;


#include <CEGUI/CEGUI.h>
#include <OGRE/OgreCEGUIRenderer.h>

#include "GameApplication.hpp"
#include "LevelManager.hpp"

namespace stunts
{
	
	/** Convert Ogre's button id's to CEGUI's one
	*/
	CEGUI::MouseButton convertOgreButtonToCegui(int buttonID)
	{
		switch (buttonID)
		{
		case MouseEvent::BUTTON0_MASK:
			return CEGUI::LeftButton;
		case MouseEvent::BUTTON1_MASK:
			return CEGUI::RightButton;
		case MouseEvent::BUTTON2_MASK:
			return CEGUI::MiddleButton;
		case MouseEvent::BUTTON3_MASK:
			return CEGUI::X1Button;
		default:
			return CEGUI::LeftButton;
		}
	}
	
	
	CGuiTask::CGuiTask() :  mGUIRenderer(0),
							mEventProcessor(0),
							mWindow(0),
							mGUISystem(0),
							mSceneMgr(0)
	{
	
	}
	
	
	/** Destructor. Clean up heap objects. */
	CGuiTask::~CGuiTask() {
		map<string, CGuiPage*>::iterator i;
		for ( i = mPages.begin(); i != mPages.end(); i++ )
			delete ( i->second );

		// delete the event processor
		if (mEventProcessor)
		{
			delete mEventProcessor;
			mEventProcessor = 0;
		}
		
		if(mGUISystem)
		{
			delete mGUISystem;
			mGUISystem = 0;
		}
		if(mGUIRenderer)
		{
			delete mGUIRenderer;
			mGUIRenderer = 0;
		}

	}
	
	
	/** Setup the rendering window **/
	void CGuiTask::setWindow (Ogre::RenderWindow* window)
	{
		mWindow = window;
	}
	
	/** Setup the underlying scene manager **/
	void CGuiTask::setSceneManager (Ogre::SceneManager* mgr)
	{
		mSceneMgr = mgr;
	}
	
	void CGuiTask::mouseMoved (Ogre::MouseEvent *e)
	{
		mGUISystem->injectMouseMove(
				e->getRelX() * mGUIRenderer->getWidth(),
				e->getRelY() * mGUIRenderer->getHeight());
		e->consume();
	}
	
	void CGuiTask::mouseDragged (Ogre::MouseEvent *e)
	{
		mouseMoved(e);
	}
	
	void CGuiTask::mousePressed (Ogre::MouseEvent *e)
	{
		mGUISystem->injectMouseButtonDown(
			convertOgreButtonToCegui(e->getButtonID()));
		e->consume();
	}
	
	void CGuiTask::mouseReleased (Ogre::MouseEvent *e)
	{
		mGUISystem->injectMouseButtonUp(
			convertOgreButtonToCegui(e->getButtonID()));
		e->consume();
	}
	
	void CGuiTask::keyPressed(Ogre::KeyEvent* e)
	{
		if (e->getKey() == KC_ESCAPE)
		{
			e->consume();
			mActive = !mActive;
			return;
		}
	
		mGUISystem->injectKeyDown(e->getKey());
		mGUISystem->injectChar(e->getKeyChar());
		e->consume();
	}
	
	void CGuiTask::keyReleased(Ogre::KeyEvent* e)
	{
		mGUISystem->injectKeyUp(e->getKey());
		e->consume();
	}
	
	void CGuiTask::keyClicked(Ogre::KeyEvent* e)
	{
		// Do nothing
		e->consume();
	}
		
	/** Add new GUI page. */
	bool CGuiTask::addPage( string PageName, string Source ) {
	
		CGuiParser Parser( 0 );
		CGuiPage *NewPage = Parser.ParsePage( PageName, Source );
	
		/* delete old page if neccessary */
		if ( mPages.find( PageName ) != mPages.end() ) {
			delete mPages[ PageName ];
			mPages.erase( PageName );
		}
	
		mPages[ PageName ] = NewPage;
		
		return true;
	}
	
	
	/** Select different GUI page. */
	bool CGuiTask::selectPage( string PageName ) {
		if ( mPages.find( PageName ) == mPages.end() )
			return false;
	
		mCurrent = mPages[ PageName ];
		mCurrent->pageSelect(); /* Inform page that is has been selected. */
        mGUISystem->setGUISheet(mCurrent->getGUISheet());
		
		// deactivate all other pages
		map<std::string, CGuiPage*>::iterator it = mPages.begin();
		while (it != mPages.end())
		{
			if (it->first != PageName)
			{
				it->second->pageDeactivate();
			}
			it ++;
		}
		
		return true;
	}
	
	
	/** Delete GUI page. */
	bool CGuiTask::deletePage( string PageName, bool Force ) {
		if ( mPages.find( PageName ) == mPages.end() )
			return false;
	
		if ( mPages[ PageName ] == mCurrent ) {
			if ( Force ) {
				mCurrent = NULL;
			} else {
				return false;
			}
		}
	
		delete mPages[ PageName ];
		mPages.erase( PageName );
	
		return true;	
	}
	
	/** Shows the gui */
	void CGuiTask::showGUI(){
		mActive = true;
	}

	/** Hides the Gui */
	void CGuiTask::hideGUI(){
		mActive = false;
	}

	/** Actions upon initialization of task by engine. */
	nrResult CGuiTask::taskInit() {
		mActive = false; /* deactivate GUI manager by default */
		mCurrent = NULL;
		
		NRASSERT(mWindow != NULL);
	
		nrLog.Log(NR_LOG_APP, "CGuiTask: Initialize CEGUI-SubSystem !");
		
		// setup GUI system
		mGUIRenderer = new CEGUI::OgreCEGUIRenderer(mWindow, Ogre::RENDER_QUEUE_OVERLAY, true, 3000, mSceneMgr);	
		mGUISystem = new CEGUI::System(mGUIRenderer);
		
		CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);
	
		nrLog.Log(NR_LOG_APP, "CGuiTask: Add event listeners to Ogre's Engine!");
	
		// Create the event processor
		// We need this to confirm buffered input
		mEventProcessor = new Ogre::EventProcessor();
		mEventProcessor->initialise(mWindow);
		mEventProcessor->startProcessingEvents();
		mEventProcessor->addKeyListener(this);
		//mInputDevice = mEventProcessor->getInputReader();
	
		// Setup mouse and keyboard listeners
		mEventProcessor->addMouseMotionListener(this);
		mEventProcessor->addMouseListener(this);
		mEventProcessor->addKeyListener(this);
	
	
	//######################################
	// TODO:
	//	Must be done by XML-Files
	
		nrLog.Log(NR_LOG_APP, "CGuiTask: Load default GUI Scheme");
		
		// load scheme and set up defaults
		try{
			CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLook.scheme");
			mGUISystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
			mGUISystem->setDefaultFont((CEGUI::utf8*)"Tahoma-12");
		}catch(...){
			nrLog.Log(NR_LOG_APP, "CGuiTask: Cannot load the default scheme !");
		}	
	
	//
	//#######################################
	
		nrLog.Log(NR_LOG_APP, "CGuiTask: GUI System was initialized");
	

		return NR_OK;
	}
	
	
	/** Actions upon starting of task by engine. */
	nrResult CGuiTask::taskStart() {		
		CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
        wmgr.getWindow((CEGUI::utf8*)"Main/Quit")
			->subscribeEvent(
				CEGUI::PushButton::EventClicked, 
				CEGUI::Event::Subscriber(&CGuiTask::handleQuit, this));

        wmgr.getWindow((CEGUI::utf8*)"Main/Start")
			->subscribeEvent(
				CEGUI::PushButton::EventClicked, 
				CEGUI::Event::Subscriber(&CGuiTask::handleStart, this));

        wmgr.getWindow((CEGUI::utf8*)"Main/Unload")
			->subscribeEvent(
				CEGUI::PushButton::EventClicked, 
				CEGUI::Event::Subscriber(&CGuiTask::handleUnload, this));

        wmgr.getWindow((CEGUI::utf8*)"Main/Level")
			->subscribeEvent(
				CEGUI::PushButton::EventClicked, 
				CEGUI::Event::Subscriber(&CGuiTask::handleLevel, this));

        wmgr.getWindow((CEGUI::utf8*)"Main/Level/OK")
			->subscribeEvent(
				CEGUI::PushButton::EventClicked, 
				CEGUI::Event::Subscriber(&CGuiTask::handleLevelOK, this));

		/*CEGUI::Window* editBox = wmgr.getWindow((CEGUI::utf8*)"Main/Level/LevelFile");
		editBox->setText(CEGUI::String(string(nrSettings.get("level_file")).c_str()));
		*/
		
		return NR_OK;
	}
	
	
	/** Actions performed in every frame when GUI is active.
	*  This method activates the currently selected GUI page if the GUI manager
	*  is active.
	*/
	nrResult CGuiTask::taskUpdate() {
			
		if( mActive && mCurrent != NULL )
			mCurrent->pageUpdate();

		
		CEGUI::Window* guiSheet = mGUISystem->getGUISheet();
		if (guiSheet)
		{
			if (mActive){
				guiSheet->show();
			}else{
				guiSheet->hide();
			}
		}
		
		return NR_OK;
	}
	
	
	/** Actions upon stopping of task by engine. */
	nrResult CGuiTask::taskStop() {
		mActive = false;
		
	
		return NR_OK;
	}

	/** Event handling **/
	bool CGuiTask::handleQuit(const CEGUI::EventArgs& e)
	{
		stunts::GameApplication::GetSingleton().quit(NULL);
		
		return true;
	}


	bool CGuiTask::handleStart(const CEGUI::EventArgs& e)
	{
		nrSettings.get("load_level") = std::string("1");
		hideGUI();
		
		return true;
	}

	bool CGuiTask::handleUnload(const CEGUI::EventArgs& e)
	{
		CLevelManager::GetSingleton().unload();
				
		return true;
	}

	bool CGuiTask::handleLevel(const CEGUI::EventArgs& e)
	{
		selectPage("MainLevel");
		
		return true;
	}
	
	bool CGuiTask::handleLevelOK(const CEGUI::EventArgs& e)
	{
		CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
		/*CEGUI::Window* editBox = wmgr.getWindow((CEGUI::utf8*)"Main/Level/LevelFile");
		nrSettings.get("level_file") = std::string(editBox->getText().c_str());
		*/
		selectPage("Main");
		
		return true;
	}
	
};

