#include "CSoundSystem.hpp"
#include <iostream>

using namespace std;

CSoundSystem::CSoundSystem(){

    
   
    if (FSOUND_GetVersion() < FMOD_VERSION)
    {
        printf("Error : You are using the wrong DLL version!  You should be using FMOD %.02f\n", FMOD_VERSION);
        
    }
    
    if (!FSOUND_Init(44100, 32, 0))
    {
        printf("Init: %s\n", FMOD_ErrorString(FSOUND_GetError()));
        
    }
    cout << "FMOD initialized\n";

    CSoundSystemMaxChannels = FSOUND_GetMaxChannels();
    cout << "MaxChannels: " << CSoundSystemMaxChannels << "\n";
    
    
   
 
    

    
}




