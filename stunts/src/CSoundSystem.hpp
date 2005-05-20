#ifndef CSOUNDSYSTEM_h
#define CSOUNDSYSTEM_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#if defined(WIN32) || defined(__WATCOMC__) || defined(_WIN64)
    #include <conio.h>
    #include <windows.h>
#else
    #include "wincompat.h"
#endif

#include "fmod.h"
#include "fmod_errors.h"
#include <iostream>



using namespace std;

class CSoundSystem {
    private:
       
        
        int CSoundSystemMaxChannels;
    public:
        CSoundSystem();
       
};



#endif
