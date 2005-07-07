#ifndef CSOUNDSYSTEM_h
#define CSOUNDSYSTEM_h

namespace stunts
{
	class CSoundSystem;
}

//------------------------------------------------------------------------------
//--- includes
//------------------------------------------------------------------------------
#include <OGRE/Ogre.h>

#include <nrEngine/nrEngine.h>

#include "External/tinyxml/tinyxml.h"

#include <boost/scoped_ptr.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#if defined(WIN32) || defined(__WATCOMC__) || defined(_WIN64)
    #include <conio.h>
    #include <windows.h>
#else
    #include "./External/FMOD/inc/wincompat.h"
#endif

#include "./External/FMOD/inc/fmod.h"
#include "./External/FMOD/inc/fmod_errors.h"
#include <iostream>
#include <vector>
#include "CSound.hpp"
#include "CSoundAtmosphere.hpp"
#include "CSoundObjects.hpp"
#include "CSoundObjectsMotor.hpp"



namespace stunts {

class CSoundSystem  : public nrISingletonTask<CSoundSystem>{
    private:
        //Music
        CSound* SoundAtmosphere;
        //Sound
        
        vector<CSound*> Sounds;
        vector<bool> isPlaying;
        vector<bool> toUpdate;
        int currentGear;
        int maxGears;
        /*
        CSound* Gear0;
        CSound* Gear1;
        CSound* Gear2;               
        CSound* Gear3;
        CSound* Gear4;
        CSound* Gear5;
        CSound* GearUp;
        CSound* GearDown;
        CSound* Air;
        CSound* Ground;
        CSound* Wheels;
        CSound* HeavyCollission;
        CSound* LightCollission;
        CSound* ScratchCollission;
        CSound* Horn;
    */
        
    public:
        CSoundSystem();
        vector<string> parseSoundFiles(std::string filename);
        //Music
        void startMusic();
        void stopMusic();
        void pauseMusic();
        void unpauseMusic();
        void nextTrack();
        void lastTrack();
        void update(int Gear,int Speed,int rpm,float position[3]);
        void registerObject();


        //-----------------------------------------
			// Task Interface
			//-----------------------------------------
			nrResult taskInit();
			nrResult taskStart();
			nrResult taskStop();
			nrResult taskUpdate();

			const char* taskGetName() { return "CLevelManager"; }
        
        
        //Sound
        void startSound();
        void stopSound();  
        void pause();
        void unpause(); 
        void startHorn();     
}
};



#endif
