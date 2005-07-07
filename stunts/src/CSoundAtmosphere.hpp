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


#ifndef __CSOUNDATMOSPHERE_HPP_
#define __CSOUNDATMOSPHERE_HPP_

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
#include "CSound.hpp"
#include "CSoundSystem.hpp"
#include <dirent.h>

using namespace std;


  
  /** An example class. This file is a template for header files in
   * this project. Note that the GPL header above contains a
   * maintainer name.  If you create a new file, you might also want
   * to enter your name as maintainer.
   */
  class CSoundAtmosphere: public CSound{
  public:
    /** Default constructor.
     */
    CSoundAtmosphere();
    void virtual start();
    void virtual stop();
    void virtual pause();
    void virtual unpause();
    void virtual next();
    void virtual back();
    void virtual updateSound(int multiplier,int gear,int Speed,float position[3]);
    bool virtual isplaying();
    void virtual setVolume(int Volume);
    
  private:
    vector<string> FileNames;
    int NumberOfStreams;   
    int ActiveStream;
    bool noFiles,isPlaying,isPaused;
    unsigned int pausePosition;
    FSOUND_STREAM* SoundStream;
  };
  


#endif
