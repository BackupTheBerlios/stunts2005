#include "CSoundSystem.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include <boost/shared_ptr.hpp>

using boost::shared_ptr;
using namespace std;
using namespace Ogre;

namespace std{

CSoundSystem::CSoundSystem(){

    
    // FMOD init
    if (FSOUND_GetVersion() < FMOD_VERSION)
    {
        printf("Error : You are using the wrong DLL version!  You should be using FMOD %.02f\n", FMOD_VERSION);
        
    }
    
    if (!FSOUND_Init(44100, 32, 0))
    {
        printf("Init: %s\n", FMOD_ErrorString(FSOUND_GetError()));
        
    }
    cout << "FMOD initialized\n";
    FSOUND_SPEAKERMODE_SURROUND;
  

    
    
    // CSoundAtmosphere init
    this->SoundAtmosphere = new CSoundAtmosphere();



    vector<std::string> TempFileNames = CSoundSystem::parseSoundFiles("../config/sound.cgf");
    bool isplaying[10] = {true,false,false,true,true,false,false,false,true,false};
    vector<std::string> temp;
    
    //front
    for (int i = 0;i < 6; i++){
        
        temp.push_back(TempFileNames[i]);
    }
    CSoundSystem::Sounds.push_back(new CSoundObjectsMotor(temp,"front",false));
    CSoundSystem::isPlaying.push_back(false);
    CSoundSystem::toUpdate.push_back(true);
    
    
     
    //rear 
    vector<std::string> rear;
    for (int i = 6;i < 12; i++){
        
        rear.push_back(TempFileNames[i]);
    }
    CSoundSystem::Sounds.push_back(new CSoundObjectsMotor(rear,"rear",false));
    CSoundSystem::isPlaying.push_back(false);
    CSoundSystem::toUpdate.push_back(true);
    
    vector<std::string> aifront;
    for (int i = 12;i < 18; i++){
        
        aifront.push_back(TempFileNames[i]);
    }
    CSoundSystem::Sounds.push_back(new CSoundObjectsMotor(aifront,"front",true));
    CSoundSystem::isPlaying.push_back(false);
    CSoundSystem::toUpdate.push_back(true);
  
    vector<std::string> airear;
    for (int i = 18;i < 24; i++){
        
        airear.push_back(TempFileNames[i]);
    }
    CSoundSystem::Sounds.push_back(new CSoundObjectsMotor(airear,"rear",true));
    CSoundSystem::isPlaying.push_back(false);
    CSoundSystem::toUpdate.push_back(true);
    
 
    for (int i = 24;i < TempFileNames.size();i++){
        
        CSoundSystem::Sounds.push_back(new CSoundObjects(TempFileNames[i],isplaying[i-23]));

        CSoundSystem::toUpdate.push_back(isplaying[i-11]);
 
        CSoundSystem::isPlaying.push_back(false);
        
    }



    CSoundSystem::currentGear = 0;
    CSoundSystem::maxGears = 5;
    
   


    
}

    
    
    nrResult CSoundSystem::taskInit()
	{
		// OK
		return NR_OK;

	}

	//--------------------------------------------------------------------------
	nrResult CSoundSystem::taskStart()
	{
		// OK
		 CSoundSystem::SoundAtmosphere->start();
		return NR_OK;

	}

	//--------------------------------------------------------------------------
	nrResult CSoundSystem::taskUpdate()
	{

	    CSoundSystem::SoundAtmosphere->updateSound(0,0,0,position);
		return NR_OK;

	}

	//--------------------------------------------------------------------------
	nrResult CSoundSystem::taskStop()
	{
	     CSoundSystem::SoundAtmosphere->stop();
		// OK
		return NR_OK;
	}
    
    
    
    

vector<std::string> CSoundSystem::parseSoundFiles(std::string filename){
    char z;
	std::string SoundFileName;
	bool record = false,adding = false;
	ifstream quelle; quelle.open(filename.c_str(), ios::binary|ios::in);
	vector<std::string> SoundFilesVector;
	while(quelle.get(z))
	{
		if((z == '\n' || z == '\r') && adding) 
		{
		 SoundFilesVector.push_back(SoundFileName);
		 
		 SoundFileName = "";
		 adding = false;
		 record = false;
	
        } 
        else if (z == '#') {
                while (z != '\n') {
                         quelle.get(z);
                }
        }
        else if (z == '=') {
                record = true;
                adding = true;
        }
        else if (record) {
              SoundFileName+=z; 
             
              
        }        
   }
   quelle.close();
   return SoundFilesVector;
}


// BackgroundMusic
void CSoundSystem::startMusic(){
    SoundAtmosphere->start();
}

void CSoundSystem::stopMusic(){
    SoundAtmosphere->stop();
}

void CSoundSystem::pauseMusic(){
    SoundAtmosphere->pause();
}

void CSoundSystem::unpauseMusic(){
    SoundAtmosphere->unpause();
}

void CSoundSystem::nextTrack(){
    SoundAtmosphere->next();
}

void CSoundSystem::lastTrack(){
    SoundAtmosphere->back();
}

void CSoundSystem::update(int Gear,int Speed,int rpm,float position[3]){
    
    SoundAtmosphere->updateSound(0,0,0,position);
    
   
    
    if (Gear >= 0 && CSoundSystem::Sounds[0]->isplaying()){
        if (Gear != CSoundSystem::currentGear){
                if (Gear > CSoundSystem::currentGear) CSoundSystem::Sounds[4]->start();
                else CSoundSystem::Sounds[5]->start();
                CSoundSystem::currentGear = Gear;
        }
    }
  if (Speed > 0){
        if (!CSoundSystem::isPlaying[6]){
              CSoundSystem::Sounds[6]->start();
              CSoundSystem::Sounds[6]->setVolume(50);
              CSoundSystem::Sounds[7]->start();
              CSoundSystem::Sounds[7]->setVolume(50);
              CSoundSystem::isPlaying[6] = true;
              CSoundSystem::isPlaying[7] = true;
        }
    } else {
        if (CSoundSystem::isPlaying[6]){

                CSoundSystem::Sounds[6]->stop();
                CSoundSystem::isPlaying[6] = false;
                CSoundSystem::Sounds[7]->stop();
                CSoundSystem::isPlaying[7] = false;
        }
    }

    for (int i = 0; i < CSoundSystem::toUpdate.size(); i++){
        if (CSoundSystem::toUpdate[i]) CSoundSystem::Sounds[i]->updateSound(rpm,Gear,Speed,position);
    }  
}


// Sounds
void CSoundSystem::startSound(){
    CSoundSystem::Sounds[0]->start();
   CSoundSystem::isPlaying[0] = true;
    CSoundSystem::Sounds[1]->start();
   CSoundSystem::isPlaying[1] = true;
    CSoundSystem::Sounds[2]->start();
    CSoundSystem::isPlaying[2] = true;
    CSoundSystem::Sounds[3]->start();
    CSoundSystem::isPlaying[3] = true;
        
}

void CSoundSystem::stopSound(){
    for (int i = 0; i < CSoundSystem::Sounds.size();i++){
        if (CSoundSystem::isPlaying[i]){
                CSoundSystem::Sounds[i]->stop();
                CSoundSystem::isPlaying[i] = false;
        }
    }
}

void CSoundSystem::pause(){
    for (int i = 0; i < CSoundSystem::Sounds.size();i++){
        CSoundSystem::Sounds[i]->pause();
    }
}

void CSoundSystem::unpause(){
    for (int i = 0; i < CSoundSystem::Sounds.size();i++){
        CSoundSystem::Sounds[i]->unpause();
    }
}

void CSoundSystem::startHorn(){
    CSoundSystem::Sounds[12]->start();
}


void CSoundSystem::registerObject(){
    
}

}
