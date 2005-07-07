/* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup, 
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Michael Raber <charlie@aufderwei.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, 
 * USA.
 */


#ifndef __CSOUNDOBJECTSMOTOR_HPP_
#define __CSOUNDOBJECTSMOTOR_HPP_
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
#include <string>
#include <vector>
#include "CSound.hpp"
#include <iostream>

using namespace std;

  
  /** An example class. This file is a template for header files in
   * this project. Note that the GPL header above contains a
   * maintainer name.  If you create a new file, you might also want
   * to enter your name as maintainer.
   */
  class CSoundObjectsMotor: public CSound{
  public:
    /** Default constructor.
     */
    CSoundObjectsMotor();
    CSoundObjectsMotor(vector<string> FileName,string type,bool Type);
    void start();
    void stop();
    void pause();
    void unpause();
    void next();
    void back();
    void updateSound(int rpm,int gear,int Speed,float position[3]);
    bool isplaying();
    void setVolume(int volume);
    /** Get Something.
     */
    
    
  private:
    string name;
    bool isPlayingAll;
    bool isPausedAll;
      
    bool Type;
    int volume;
    int speed;    
    int currentPlaying;
    int shouldPlay;
    int oldRPM;
    int Factor;
    float x,y,z;
    vector<FSOUND_SAMPLE*> CSoundSample;
    vector<bool> isPlaying;
    vector<bool> isPaused;
    int Channel;
      
  };
  


#endif
