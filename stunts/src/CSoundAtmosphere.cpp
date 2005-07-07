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


#include "CSoundAtmosphere.hpp"

using namespace std;
  
  CSoundAtmosphere::CSoundAtmosphere()  
  { 
    int i = 0;
    DIR *hdir;
    struct dirent *entry;
    string prefix = "../media/music";
    noFiles = true;
    isPlaying = false;
    isPaused = false;
    hdir = opendir("../media/music");
    
    do
    {
        entry = readdir(hdir);
        if (entry)
        {
            
            if (i < 2) {
                  i++;      
            } else {
                  cout << entry->d_name << endl;      
                  noFiles = false;
                  string temp = prefix + entry->d_name;
                  FileNames.push_back(temp);
            }
        }
    } while (entry);
    closedir(hdir);
    NumberOfStreams = FileNames.size()-1;
    ActiveStream = 0;  
}
  

  void CSoundAtmosphere::start(){
      if (!noFiles) {
            if (!isPlaying){
                        SoundStream = FSOUND_Stream_Open(FileNames[ActiveStream].c_str(),FSOUND_NORMAL | FSOUND_MPEGACCURATE,0,0);
                        if (!FSOUND_Stream_Play(FSOUND_FREE,CSoundAtmosphere::SoundStream)){
                        printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
                        }
                        cout << "Music started" << endl;
            }
            isPlaying = true;
      }
  }
  
  void CSoundAtmosphere::updateSound(int multiplier,int gear,int Speed,float position[3]){
  if (isPlaying){
      if (FSOUND_Stream_GetPosition(CSoundAtmosphere::SoundStream) >= FSOUND_Stream_GetLength(CSoundAtmosphere::SoundStream)){
            if (CSoundAtmosphere::ActiveStream == CSoundAtmosphere::NumberOfStreams) CSoundAtmosphere::ActiveStream = 0;
            else CSoundAtmosphere::ActiveStream++;
            
            FSOUND_Stream_Stop(CSoundAtmosphere::SoundStream);
            FSOUND_Stream_Close(CSoundAtmosphere::SoundStream);
            cout << CSoundAtmosphere::ActiveStream << endl;
            CSoundAtmosphere::SoundStream = FSOUND_Stream_Open(FileNames[ActiveStream].c_str(),FSOUND_NORMAL | FSOUND_MPEGACCURATE,0,0);
            
            FSOUND_Stream_Play(FSOUND_FREE,CSoundAtmosphere::SoundStream);
      }
  }
  }

  void CSoundAtmosphere::stop(){
    if (isPlaying){
      FSOUND_Stream_Stop(CSoundAtmosphere::SoundStream);
      FSOUND_Stream_Close(CSoundAtmosphere::SoundStream);
      isPlaying = false;
      cout << "Music stopped" << endl;
    }
  }
 
  void CSoundAtmosphere::pause(){
    if (isPlaying){
      pausePosition = FSOUND_Stream_GetPosition(CSoundAtmosphere::SoundStream);
      FSOUND_Stream_Stop(CSoundAtmosphere::SoundStream);
      isPaused = true;
      cout << "Music paused" << endl;
    }
  }
  
  void CSoundAtmosphere::unpause(){
    if (isPlaying && isPaused){
      FSOUND_Stream_SetPosition(CSoundAtmosphere::SoundStream,pausePosition);
      FSOUND_Stream_PlayEx(FSOUND_FREE,CSoundAtmosphere::SoundStream,0,0);
      isPaused = false;
      cout << "Music unpaused" << endl;
    }
  }
  
  void CSoundAtmosphere::next(){
      if (ActiveStream == NumberOfStreams) ActiveStream = 0;
      else ActiveStream == ActiveStream++;
      FSOUND_Stream_Stop(SoundStream);
      FSOUND_Stream_Close(SoundStream);
      SoundStream = FSOUND_Stream_Open(FileNames[ActiveStream].c_str(),FSOUND_NORMAL | FSOUND_MPEGACCURATE,0,0);
      if (isPaused){
            pausePosition = 0;
      } else if (isPlaying) {
            FSOUND_Stream_Play(FSOUND_FREE,CSoundAtmosphere::SoundStream);
      }
      cout << "next Song" << endl;
      
  }
  
  void CSoundAtmosphere::back(){
      if (ActiveStream == 0) ActiveStream = NumberOfStreams;
      else ActiveStream--;
      FSOUND_Stream_Stop(SoundStream);
      FSOUND_Stream_Close(SoundStream);
      SoundStream = FSOUND_Stream_Open(FileNames[ActiveStream].c_str(),FSOUND_NORMAL | FSOUND_MPEGACCURATE,0,0);
      if (isPaused){
            pausePosition = 0;
      } else if (isPlaying){
            FSOUND_Stream_Play(FSOUND_FREE,CSoundAtmosphere::SoundStream);
      }
      cout << "last Song" << endl;
  }
  
  bool CSoundAtmosphere::isplaying(){
    return false;
  }
  
  void CSoundAtmosphere::setVolume(int Volume){
  
  }
