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


#include "CSoundObjectsMotor.hpp"
#include <iostream>
#include <math.h>

using namespace std;
  CSoundObjectsMotor::CSoundObjectsMotor(){
  
  }
  
  CSoundObjectsMotor::CSoundObjectsMotor(vector<string> FileName,string type,bool Type)
  {
      
  
      
      this->isPlayingAll = false;
      this->isPausedAll = false;
      this->Type = Type;
      for (int i = 0; i < FileName.size(); i++){
            const char* tempName = FileName[i].c_str();
            CSoundObjectsMotor::CSoundSample.push_back(FSOUND_Sample_Load(FSOUND_FREE,tempName,FSOUND_HW3D | FSOUND_LOOP_NORMAL,0,0));
            CSoundObjectsMotor::isPlaying.push_back(false);
            CSoundObjectsMotor::isPaused.push_back(false);
            cout << "Created " << FileName[i] << endl;
      }
      this->Channel = -1;
      this->currentPlaying = 0;
      this->shouldPlay = 0;
      this->Factor = 1;
      if (type.compare("front")){
            CSoundObjectsMotor::x = 0.0;
            CSoundObjectsMotor::y = 0.0;
            CSoundObjectsMotor::z = 0.5; 
      } else {
           CSoundObjectsMotor::x = 0.0;
           CSoundObjectsMotor::y = 0.0;
           CSoundObjectsMotor::z = -0.5; 
      }

      
      
      // temporär
      
      this->oldRPM = 1000;
      
  }
  
  void CSoundObjectsMotor::start(){
      if (!CSoundObjectsMotor::isPlayingAll){
            CSoundObjectsMotor::Channel = FSOUND_PlaySound(FSOUND_FREE,CSoundObjectsMotor::CSoundSample[0]);
            float pos[3] = {x,y,z};
            float vel[3] = {0.0,0.0,0.0};
            FSOUND_3D_SetAttributes(CSoundObjectsMotor::Channel,pos,vel);
            FSOUND_Update();
            CSoundObjectsMotor::isPlaying[0] = true;
            CSoundObjectsMotor::isPlayingAll = true;
            CSoundObjectsMotor::currentPlaying = 0;
            
      }
      
  }
  
  void CSoundObjectsMotor::stop(){
      if (CSoundObjectsMotor::isPlayingAll){
            FSOUND_StopSound(CSoundObjectsMotor::Channel);
            CSoundObjectsMotor::isPlayingAll = false;
            CSoundObjectsMotor::isPlaying[CSoundObjectsMotor::currentPlaying] = false;
      }
  }
  
  void CSoundObjectsMotor::pause(){
      if (CSoundObjectsMotor::isPlayingAll){
            FSOUND_SetPaused(CSoundObjectsMotor::Channel,true);
            CSoundObjectsMotor::isPausedAll = true;
            CSoundObjectsMotor::isPaused[CSoundObjectsMotor::currentPlaying] = true;
      }
  
  }
  
  void CSoundObjectsMotor::unpause(){
      if (CSoundObjectsMotor::isPausedAll && CSoundObjectsMotor::isPlayingAll){
            FSOUND_SetPaused(CSoundObjectsMotor::Channel,false);
            CSoundObjectsMotor::isPausedAll = false;
            CSoundObjectsMotor::isPaused[CSoundObjectsMotor::currentPlaying] = false;
      }
  }
  
  void CSoundObjectsMotor::next(){
  
  }
  
  void CSoundObjectsMotor::back(){
  
  }
  
  void CSoundObjectsMotor::updateSound(int rpm,int gear,int Speed,float position[3]){
      if (CSoundObjectsMotor::isPlayingAll){
      int frequency; 
      float pos[3],vel[3];
     
      if (CSoundObjectsMotor::Type){
            pos[0] = position[0] + x;
            pos[1] = position[1] + y;
            pos[2] = position[2] + z;
          /* float timedelta;
            float velx = (pos[0]-x) ;
            float vely = (pos[1]-y) ;
            float velz = (pos[2]-z) ;
            x = position[0];
            y = position[1];
            z = position[2];*/
            vel[0] = 0;
            vel[1] = 0;
            vel[2] = 0;
            cout << "x: " << pos[0] << endl;
            cout << "y: " << pos[1] << endl;
            cout << "z: " << pos[2] << endl;
      } else {
            cout << "usercar" << endl;
            pos[0] = x;
            pos[1] = y;
            pos[2] = z;
            vel[0] = 0;
            vel[1] = 0;
            vel[2] = 0;
      }
     
           if (gear == 0) {CSoundObjectsMotor::shouldPlay = 0;}
      else if (gear == 1) {CSoundObjectsMotor::shouldPlay = 1;}
      else if (gear == 2) {CSoundObjectsMotor::shouldPlay = 2;}
      else if (gear == 3) {CSoundObjectsMotor::shouldPlay = 3;}
      else if (gear == 4) {CSoundObjectsMotor::shouldPlay = 4;}
      else if (gear == 5) {CSoundObjectsMotor::shouldPlay = 5;}

      if (CSoundObjectsMotor::currentPlaying != CSoundObjectsMotor::shouldPlay)
      {
            FSOUND_StopSound(CSoundObjectsMotor::Channel);
            CSoundObjectsMotor::Channel = FSOUND_PlaySound(FSOUND_FREE,CSoundObjectsMotor::CSoundSample[CSoundObjectsMotor::shouldPlay]);
        //    cout << "frequency from channel: " << FSOUND_GetFrequency(CSoundObjectsMotor::Channel) << endl;
            CSoundObjectsMotor::isPlaying[CSoundObjectsMotor::currentPlaying] = false;
            CSoundObjectsMotor::isPlaying[CSoundObjectsMotor::shouldPlay] = true;
            CSoundObjectsMotor::currentPlaying = CSoundObjectsMotor::shouldPlay;
      } 
      {
       
            if (gear==1)
            { 
                        frequency = 13000;
                        frequency = frequency + rpm*9;
            }
            if (gear==2){
           
                        frequency = 11000;
                        frequency = frequency + rpm*5;
            
            }
            if (gear==3){
           
                        frequency = 11000;
                        frequency = frequency + rpm*4;
            
            }
            if (gear==4){
           
                        frequency = 11000;
                        frequency = frequency + rpm*3;
            
            }
            if (gear==5){
           
                        frequency = 11000;
                        frequency = frequency + rpm*3;
            
            }
            
                 
                           int newRPM = rpm - CSoundObjectsMotor::oldRPM;
        //    cout << "RPM: " << rpm << endl;
        //    cout << "newRPM: " << newRPM << endl;
        //    cout << "oldRPM: " << CSoundObjectsMotor::oldRPM << endl;
            
            
//            frequency = ((rpm*20)/CSoundObjectsMotor::Factor)+32000;
        //    cout << "Frquenc: " << frequency << endl;
            FSOUND_SetFrequency(CSoundObjectsMotor::Channel,frequency);
      }
      FSOUND_3D_SetAttributes(CSoundObjectsMotor::Channel,pos,vel);
      FSOUND_Update();      
      CSoundObjectsMotor::oldRPM = rpm;
    }
  }
  
  
  bool CSoundObjectsMotor::isplaying(){
    return CSoundObjectsMotor::isPlayingAll;
  }
  
  void CSoundObjectsMotor::setVolume(int volume){
    
  }
