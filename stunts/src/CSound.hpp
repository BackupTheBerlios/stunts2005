#ifndef CSound_h
#define CSound_h
/*******************************
*  $Revision: 
*  $Date: 
*
*******************************/

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#if defined(WIN32) || defined(__WATCOMC__) || defined(_WIN64)
    #include <conio.h>
    #include <windows.h>
#else
    #include "./External/FMOD/inc/wincompat.h"
#endif
#include "CSoundSystem.hpp"


using namespace std;

class CSound {
  
 private:
    string name;
    bool isPlaying;
    bool isPause;
    bool isLoop;
    int volume;
    float speed;    
    FSOUND_SAMPLE* CSoundSample;
    FMUSIC_MODULE* CMusicMod;
    int channel,channelNumber;

 public:
    CSound(string name,bool isLoop,int volume);
    //~CSound();
    void start();
    void stop();
    void pause();
    void unpause();
    void setVolume(int volume);
    int getVolume();
    void setSpeed(float speed);
    float getSpeed();
   
};

#endif


