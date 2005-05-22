/*******************************
*  $Revision: 
*  $Date: 
*
*******************************/

#include <string>
#include "CSound.hpp"
#include <iostream>
#include "CSoundSystem.hpp"

using namespace std;

CSound::CSound(string name,bool isLoop,int volume){
  this->name = name;
  this->isPlaying = false;
  this->isPause = false;
  this->isLoop = isLoop;
  this->volume = volume;
  this->speed = 0;
  const char* tempName = name.c_str();

      CSoundSample = FSOUND_Sample_Load(FSOUND_FREE,tempName,FSOUND_HW3D,0,0);
}
/*
CSound::~CSound(){
    FSOUND_Sample_Free(CSoundSample);
}
*/

void CSound::start(){
    if (!this->isPlaying && !this->isPause) 
    {
           this->isPlaying = true;
           channel = FSOUND_PlaySoundEx(FSOUND_FREE,CSoundSample,0,0);
           if (!FSOUND_SetVolume(channel,volume))
           {
                    printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));       
           }
           cout << "Play\n";
    }
      
}

void CSound::stop(){
    if (this->isPlaying || this->isPause){
        this->isPlaying = false;
        this->isPause = false;
        cout << "Stop\n";
        if (!FSOUND_StopSound(channel))
        {
            printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
        } 
    } 
}

void CSound::pause(){
    if (!this->isPause && this->isPlaying){
        this->isPlaying = false;
        this->isPause = true;
        cout << "Pause\n";
        if (!FSOUND_SetPaused(channel,true)){
                printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
        }
    }
}

void CSound::unpause(){
    if (this->isPause){
        this->isPlaying = true;
        this->isPause = false;
        cout << "Unapuse\n";
        if (!FSOUND_SetPaused(channel,false)){
                printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
        }
    }
}

void CSound::setVolume(int volume){
    if (volume >= 0 && volume < 256){
        this->volume = volume;
        if (!FSOUND_SetVolume(channel,volume)){
                printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
        }
        cout << "Volume: " << volume << endl;
    }
}

int CSound::getVolume(){
    return this->volume;
}

void CSound::setSpeed(float speed){
    this->speed = speed;
}

float CSound::getSpeed(){
    return this->speed;
}


