/***************************************************************************
 * working main (for my personal use only)                                 *
 ***************************************************************************/

#define NR_PATH_TO_VFS "./"

#include <iostream>
#include <string>

typedef unsigned char 	BYTE;
typedef unsigned char 	byte;
typedef unsigned char 	uchar;

#include <assert.h>
#include <nrEngine/nrEngine.h>

// my task!
#include "CGui/CGuiTask.h"

// testing tasks
#include "cscreentask.h"
#include "cbgtask.h"

// Game-Mode Values
#define	NR_MAIN_LOOP		0
#define	NR_PAUSE			1
#define	NR_LOADING			2
#define	NR_NOT_STARTED		3

void initializeGL(int width, int height); // see below

void quit(void* p) {
	nrLog.Log(NR_LOG_APP, "quit(): Quit the application");
	nrKernel.KillAllTasks();	
}

#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600
#define WINDOW_DEPTH	32

#define PROGRAMM_STRING "CGui Test"

int main(int argc, char *argv[]){

	// for convenience
	int32	hRC;
	nrResult ret = NR_OK;

	nrEngineInit();

	nrLog.Init("./log/", "./log/msgs.log");
	nrLog.Log(NR_LOG_APP, "Application started");

	// Initialize virtual file system 
	nrVFS.setPathToVFS(NR_PATH_TO_VFS);
  	ret = nrVFS.openFileSystem();
  	if (ret != NR_OK){
		printf("VFS Error !!!\n");
		return (NR_APP_ERROR | NR_VFS_CANNOT_OPEN);		
  	}

	// Create our window, we pass caption for the window, the width, height and video flags required
  	nrFrameworkInit();
	SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8);
	ret = nrFramework.createRenderContext (&hRC,
		WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_DEPTH);
	if (ret != NR_OK){
		return (NR_RC_ERROR);
	}
	nrFramework.getRC()->changeWindowTitle(PROGRAMM_STRING);
	nrFramework.setQuitCallback(quit, NULL);

	//initializeGL(nrFrameWork.getScreenWidth(),nrFrameWork.getScreenHeight());
	initializeGL(WINDOW_WIDTH,WINDOW_HEIGHT);

	// set echo logging, that means the log engine will echo the log messages
	// to other log targets 
	nrLog.echoLogging(NR_LOG_ENGINE, NR_LOG_CONSOLE);
	nrLog.echoLogging(NR_LOG_APP, NR_LOG_CONSOLE);

  	nrSettings.useProfile("eLowar");

	// add background task
	boost::shared_ptr<nrITask> bgTask(new CBgTask(hRC));
	bgTask->setTaskPriority(NR_PRIORITY_HIGH);
	nrKernel.AddTask(bgTask);

	// add GUI task
	boost::shared_ptr<nrITask> guiTask(new CGuiTask(hRC));
	guiTask->setTaskPriority(NR_PRIORITY_NORMAL);
	nrKernel.AddTask(guiTask);

	// this is how you load a page!
	((CGuiTask*)guiTask.get()) -> addPage( "dummy", "" );
	((CGuiTask*)guiTask.get()) -> selectPage( "dummy" );
	((CGuiTask*)guiTask.get()) -> rActive() = true;

	// Create framework class to guarantee, that we have input update
	nrFramework.AddToKernel(nrKernel, NR_PRIORITY_LAST);

	// add cScreenTask for testing
	boost::shared_ptr<nrITask> scrTask(new cScreenTask(hRC));
	scrTask->setTaskPriority(NR_PRIORITY_LAST);
	nrKernel.AddTask(scrTask);

  	nrSettings.setGameMode(NR_MAIN_LOOP);

	nrKernel.Execute();  // ========== START THE SHOW ==========
	
	// kill all tasks if there is one 
	nrKernel.KillAllTasks();
	
	// Store current profile on the disk
	nrSettings.storeProfile();
	  
	// delete all stuff allocated for our game engine
	nrLog.Log(NR_LOG_APP, "Application stopped");
	nrEngineDelete();

	nrFrameworkDelete();	

	return 0;
}




void initializeGL(int width, int height) {

	// init Stuff
	glShadeModel (GL_SMOOTH);
	glClearDepth (1.0f);
	glClearStencil (0);
	glClearColor (0,0,1,1);
	
	// Depth Buffer enablen
	nrOnDepthRead();
	nrOnDepthWrite();	
	glDepthFunc (GL_LEQUAL);
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	// enable lighting and material colors
	nrOnLit();
	nrOnColorMaterial();                        // Allow color
	
	//highest lighting quality
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	
}

