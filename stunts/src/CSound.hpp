#ifndef CSOUND_HPP_
#define CSOUND_HPP_
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
//#include "CSoundSystem.hpp"



using namespace std;

class CSound {
  
 private:
  /*  string name;
    bool isPlaying;
    bool isPause;
    bool isLoop;
    int volume;
    int speed;    
    FSOUND_SAMPLE* CSoundSample;
    FMUSIC_MODULE* CMusicMod;
    int channel,channelNumber;
   */
 public:
    

    //~CSound();
    virtual void start() = 0;
    virtual void updateSound(int rpm,int Gear,int Speed,float position[3]) = 0;
    virtual void stop() = 0;
    virtual void pause() = 0;
    virtual void unpause() = 0;
    virtual void next() = 0;
    virtual void back() = 0;
    virtual bool isplaying() = 0;
    virtual void setVolume(int Volume) = 0;
    
    /*virtual void setVolume(int volume);
    virtual int getVolume();
    virtual void setSpeed(int speed);
    virtual int getSpeed();
   */
   
};

#endif


