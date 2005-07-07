/* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup, 
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Firstname Lastname <email@domain.org>
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


#include "CSoundObjects.hpp"

using namespace std;
  CSoundObjects::CSoundObjects(){
  
  }
  
  CSoundObjects::CSoundObjects(string FileName,bool isLoop)
  {
      
      this->isPlaying = false;
      this->isPaused = false;
      this->isLoop = isLoop;
      this->isPaused = false;
      
      const char* tempName = FileName.c_str();
      CSoundObjects::CSoundSample = FSOUND_Sample_Load(FSOUND_FREE,tempName,FSOUND_HW3D,0,0);
      if (this->isLoop) FSOUND_Sample_SetMode(CSoundObjects::CSoundSample,FSOUND_LOOP_NORMAL);
      cout << "Created " << FileName << " " << this->isLoop << endl;
      this->Channel = -1;
      
      
  }
  
  void CSoundObjects::start(){
      if (!CSoundObjects::isPlaying){
            CSoundObjects::Channel = FSOUND_PlaySound(FSOUND_FREE,CSoundObjects::CSoundSample);
            if (CSoundObjects::isLoop) CSoundObjects::isPlaying = true;
            else CSoundObjects::isPlaying = false;
      }
      
  }
  
  void CSoundObjects::stop(){
      if (CSoundObjects::isPlaying){
            FSOUND_StopSound(CSoundObjects::Channel);
            CSoundObjects::isPlaying = false;
      }
  }
  
  void CSoundObjects::pause(){
      if (CSoundObjects::isPlaying){
            FSOUND_SetPaused(CSoundObjects::Channel,true);
            CSoundObjects::isPaused = true;
      }
  
  }
  
  void CSoundObjects::unpause(){
      if (CSoundObjects::isPaused && CSoundObjects::isPlaying){
            FSOUND_SetPaused(CSoundObjects::Channel,false);
            CSoundObjects::isPaused = false;
      }
  }
  
  void CSoundObjects::next(){
  
  }
  
  void CSoundObjects::back(){
  
  }
  
  void CSoundObjects::setVolume(int volume){
      FSOUND_SetVolume(CSoundObjects::Channel,volume);
  }
  
  void CSoundObjects::updateSound(int rpm,int gear,int Speed,float position[3]){
       if (CSoundObjects::isLoop){
            FSOUND_SetFrequency(CSoundObjects::Channel,(Speed*Speed));
            FSOUND_SetVolume(CSoundObjects::Channel,Speed);
       }
       cout << "Speed: " << Speed << endl;
       cout << "Frequ: " << FSOUND_GetFrequency(CSoundObjects::Channel) << endl;
  }
  
  bool CSoundObjects::isplaying(){
    return CSoundObjects::isPlaying;
  }

