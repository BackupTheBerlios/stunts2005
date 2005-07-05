/* CVS $Id: CGuiTask.cpp,v 1.12 2005/07/05 22:17:57 psyborg Exp $ */

/** @file
 *  Main GUI task and manager for Stunts 2005.
 *
 *  @author  Markus Thiele, Art Tevs
 *
 *  @version CVS $Revision: 1.12 $
 *  @date    CVS $Date: 2005/07/05 22:17:57 $
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

		// register game paused mode variable
		nrSettingsRegister(bool, mActive, "gui_active");
	}
	
	
	/** Destructor. Clean up heap objects. */
	CGuiTask::~CGuiTask() {
		nrSettings.deregisterVariable("gui_active");
		
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
		CEGUI::MouseCursor::getSingleton().show();
	}

	/** Hides the Gui */
	void CGuiTask::hideGUI(){
		mActive = false;
		CEGUI::MouseCursor::getSingleton().hide();
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


		//----------------------------------------------------------
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

		
		//----------------------------------------------------------
        wmgr.getWindow((CEGUI::utf8*)"Main/Level")
			->subscribeEvent(
				CEGUI::PushButton::EventClicked, 
				CEGUI::Event::Subscriber(&CGuiTask::handleLevel, this));
        
		wmgr.getWindow((CEGUI::utf8*)"Main/Level/OK")
			->subscribeEvent(
				CEGUI::PushButton::EventClicked, 
				CEGUI::Event::Subscriber(&CGuiTask::handleLevelOK, this));

        
		//----------------------------------------------------------
		wmgr.getWindow((CEGUI::utf8*)"Main/Option")
			->subscribeEvent(
				CEGUI::PushButton::EventClicked, 
				CEGUI::Event::Subscriber(&CGuiTask::handleOption, this));
		
        wmgr.getWindow((CEGUI::utf8*)"Main/Option/OK")
			->subscribeEvent(
				CEGUI::PushButton::EventClicked, 
				CEGUI::Event::Subscriber(&CGuiTask::handleOptionOK, this));

        wmgr.getWindow((CEGUI::utf8*)"Main/Option/UseShadows")
			->subscribeEvent(
				CEGUI::Checkbox::EventCheckStateChanged,
				CEGUI::Event::Subscriber(&CGuiTask::handleOptionChange, this));
		
        wmgr.getWindow((CEGUI::utf8*)"Main/Option/UseShadowsInWater")
			->subscribeEvent(
				CEGUI::Checkbox::EventCheckStateChanged,
				CEGUI::Event::Subscriber(&CGuiTask::handleOptionChange, this));

		// load all screenshot images into the vector
		std::vector<CLevelManager::LevelData>& levels = CLevelManager::GetSingleton().getAllLevelData();
		for (unsigned int i=0; i < levels.size(); i++){
			CLevelManager::LevelData& data = levels[i];
			
			for (unsigned int j=0; j < data.screenshots.size(); j++){
				char imgname[256];
				sprintf(imgname, "%s_image", data.screenshots[j]->getName().c_str());

				// Try to create images for the GUI from the Textures
				try{
					CEGUI::Texture* tex = mGUIRenderer->createTexture(data.screenshots[j]);
					CEGUI::Imageset* imageSet = CEGUI::ImagesetManager::getSingleton().createImageset((CEGUI::utf8*)data.screenshots[j]->getName().c_str(), tex);

	 				imageSet->defineImage((CEGUI::utf8*)imgname, 
						CEGUI::Point(0.0f, 0.0f),
						CEGUI::Size(tex->getWidth(), tex->getHeight()),
						CEGUI::Point(0.0f,0.0f));

					// add the imageset into our internal database
					mImageSets.push_back(imageSet);
				}catch (...){}
				
			}
		}
		
		// add all levels we can load
		CEGUI::Listbox* mList = static_cast<CEGUI::Listbox*>(wmgr.getWindow((CEGUI::utf8*)"Main/Level/LevelList"));
		if (mList)
		{
			std::vector<CLevelManager::LevelData>& list = CLevelManager::GetSingleton().getAllLevelData();
			
			for (unsigned int i=0; i < list.size(); i++)
			{
				CEGUI::ListboxTextItem *listboxitem = new CEGUI::ListboxTextItem (list[i].name.c_str());
				listboxitem->setSelectionBrushImage("TaharezLook", "ListboxSelectionBrush");
				listboxitem->setSelected (i == 0);
				mList->addItem(listboxitem);
			}
			
			// if one was selected, so call this method
			mList->subscribeEvent(
				CEGUI::Listbox::EventSelectionChanged, 
				CEGUI::Event::Subscriber(&CGuiTask::handleLevelSelected, this));
				
			// now initialize the values
			CEGUI::EventArgs temp;
			handleLevelSelected(temp);
		}
		
				
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
				CEGUI::MouseCursor::getSingleton().show();
			}else{
				guiSheet->hide();
				CEGUI::MouseCursor::getSingleton().hide();
			}
		}
		
		// Update the current level name
		CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
		CEGUI::Window* wind = wmgr.getWindow((CEGUI::utf8*)"Menu");
		if (wind)
		{
			wind->setText((std::string("Selected Level: ") + std::string(nrSettings.get("level_file"))).c_str());
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
		CEGUI::Listbox* mList = static_cast<CEGUI::Listbox*>(wmgr.getWindow((CEGUI::utf8*)"Main/Level/LevelList"));
		if (mList)
		{
			nrSettings.set("level_file", std::string(mList->getFirstSelectedItem()->getText().c_str()));		
		}

		selectPage("Main");
		
		return true;
	}
	
	
	bool CGuiTask::handleLevelSelected(const CEGUI::EventArgs& e)
	{

		
		CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
		CEGUI::Listbox* mList = static_cast<CEGUI::Listbox*>(wmgr.getWindow((CEGUI::utf8*)"Main/Level/LevelList"));
		if (mList)
		{
			CEGUI::ListboxItem* item = mList->getFirstSelectedItem();
			if (item){
				CLevelManager::LevelData* data = CLevelManager::GetSingleton().getLevelData(std::string(item->getText().c_str()));
				if (data){
					CEGUI::Window* win = wmgr.getWindow((CEGUI::utf8*)"Menu/Level/LevelDescr");
					if (win){
						char	descr[1024];
						sprintf(descr, "Author: %s\n\nDate: %s", data->author.c_str(), data->datestr.c_str());
						win->setText(descr);
					}
					
					CEGUI::StaticImage* img = static_cast<CEGUI::StaticImage*>(wmgr.getWindow((CEGUI::utf8*)"Menu/Level/LevelScreenshot"));
					
					if (img){
						if (data->screenshots.size()){
							// create the name of the screenshot
							CEGUI::String scrName (data->screenshots[0]->getName().c_str());
							CEGUI::String scrNameImage ((data->screenshots[0]->getName() + "_image").c_str());
						
							try{
								CEGUI::Imageset* set= CEGUI::ImagesetManager::getSingleton().getImageset(scrName);
								if (set){
									img->setImage(&set->getImage(scrNameImage));							
								}
							}catch(...){
								nrLog.Log(NR_LOG_APP, "CGuiTask: Cannot set screenshot %s", scrName.c_str());
							}
						}else{
							img->setImage(NULL);
						}
					}
				}
			}


		}
		
		return true;
	}

	/** Call this to change the current screenshot to the next one**/
	bool CGuiTask::handleLevelChangeScreenshot(const CEGUI::EventArgs& e)
	{

	}

	/** Will be called to setup mouse over events */
	void CGuiTask::setupEnterExitEvents(CEGUI::Window* win)
	{
		win->subscribeEvent(
			CEGUI::Window::EventMouseEnters, 
			CEGUI::Event::Subscriber(&CGuiTask::handleMouseEnters, this));
		win->subscribeEvent(
			CEGUI::Window::EventMouseLeaves, 
			CEGUI::Event::Subscriber(&CGuiTask::handleMouseLeaves, this));
		for (unsigned int i = 0; i < win->getChildCount(); ++i)
		{
			CEGUI::Window* child = win->getChildAtIdx(i);
			setupEnterExitEvents(child);
		}
	}

	/** Call this event if mouse moves over an object **/
	bool CGuiTask::handleMouseEnters(const CEGUI::EventArgs& e)
	{
		/*
		CEGUI::WindowEventArgs& we = ((CEGUI::WindowEventArgs&)e);
		::iterator i = mDescriptionMap.find(we.window->getName());
		if (i != mDescriptionMap.end())
		{
			mTip->setText(i->second);
		}
		else
		{
			mTip->setText((CEGUI::utf8*)"");
		}
		*/
		return true;
	}

	/** Call if mouse is get out from the object **/
	bool CGuiTask::handleMouseLeaves(const CEGUI::EventArgs& e)
	{
		/*
		mTip->setText((CEGUI::utf8*)"");
		*/
		return true;
	}


	bool CGuiTask::handleOption(const CEGUI::EventArgs& e)
	{
		selectPage("MainOption");
		
		return true;
	}
	
	bool CGuiTask::handleOptionOK(const CEGUI::EventArgs& e)
	{

		selectPage("Main");
		
		return true;
	}

	
	/** Activate this event if something changed with checkboxes **/
	bool CGuiTask::handleOptionChange(const CEGUI::EventArgs& e)
	{
		CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();

		// get all checkboxes
		CEGUI::Checkbox* mUseShadows = static_cast<CEGUI::Checkbox*>(wmgr.getWindow((CEGUI::utf8*)"Main/Option/UseShadows"));
		CEGUI::Checkbox* mUseShadowsInWater = static_cast<CEGUI::Checkbox*>(wmgr.getWindow((CEGUI::utf8*)"Main/Option/UseShadowsInWater"));

		// check the state of the checkboxes
		if (mUseShadows->isSelected()){
			nrSettings.get("use_shadows") = std::string("1");
		}else{
			nrSettings.get("use_shadows") = std::string("0");
		}

		if (mUseShadowsInWater->isSelected()){
			nrSettings.get("use_shadows_in_water") = std::string("1");
		}else{
			nrSettings.get("use_shadows_in_water") = std::string("0");
		}
				
		return true;
	}

};

