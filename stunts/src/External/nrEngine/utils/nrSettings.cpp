/***************************************************************************
 *                                                                         *
 *   (c) Art Tevs, MPI Informatik Saarbruecken                             *
 *       mailto: <tevs@mpi-sb.mpg.de>                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "nrSettings.h"

#ifdef _WIN32

#else

# include <sys/stat.h>
# include <sys/types.h>

#endif

//
// delete empty
//
std::string trim(const std::string& str){
	std::string res;
	
	for (unsigned int i=0; i < str.length();i++){
		if(str[i] != ' ' && str[i] != '\n'){
			res += str[i];
		}
	}
	
	return res;
}

//
// Creates directory on the disk. This function use system, so it should be 
// written for every plattform we are using
//
void createDirectory(const std::string& dir){

	nrLog.Log(NR_LOG_ENGINE, "Create directory %s", dir.c_str());
	
#ifdef _WIN32

#else
	//XXX: possibility for argument injection vulnerability:
	//system ((std::string("mkdir ") + dir).c_str());

	// it is better to provide an argument with mode?
	if (mkdir(dir.c_str(), /*mode*/0700) == 0) {
	    // return NR_OK;
	} else {
	    // TODO: this method should have nrResult return type,
	    // check return code and return appropriately
	    // Should we log success or failure??
	}
#endif

}

#define _SET_MAP std::map<string, _nrData>

/*--------------------------------------------------------------------------
 Constructor
 -------------------------------------------------------------------------*/
nrCSettings::nrCSettings(){
	
	// Set default values
	multiTexCount = 0;
	
	// initialize default settings
	initSettings();	
}

/*--------------------------------------------------------------------------
 Destructor
 -------------------------------------------------------------------------*/
nrCSettings::~nrCSettings(){

}


/**
 * Register and set default variables
 * Default variables are that variables, which are not registered from outside
 **/
nrResult nrCSettings::registerDefaults(){
	
	// clear all variables
	settings.clear();
	
	return NR_OK;
}


/**
 * Register variable to allow using of it
 * @param name - name of the variable
 * @param var - smart pointer of nrIDator-Interface to our variable
 * @return either NR_OK or:
 * 		- NR_SETTING_VAR_ALREADY_REGISTERD if such a variabe already registered
 **/
nrResult nrCSettings::registerVariable(const string &name, shared_ptr<nrIDator> var){
	
	if (isRegistered(name)) return NR_SETTINGS_VAR_ALREADY_REGISTERED;
	
	settings[name] = var;

	string _str (name);	
	nrLog.Log(NR_LOG_KERNEL, "Register Variable %s", _str.c_str());
	
	return NR_OK;
}


/**
 * Deregister variable from settings manager
 * @param name - name of the variable
 * @return either NR_OK or:
 * 		- NR_SETTING_ERROR if such a variabe was not found
 **/
nrResult nrCSettings::deregisterVariable(const string &name){
	
	if (!isRegistered(name)) return NR_SETTINGS_VAR_NOT_REGISTERED;
	
	_SET_MAP::iterator it = settings.find(name);
	if (it == settings.end()){
		return NR_SETTINGS_ERROR;	
	}
	
	// erase file pointer from our map
	settings.erase(it);

	string _str (name);	
	nrLog.Log(NR_LOG_KERNEL, "Delete Variable %s", _str.c_str());
	
	return NR_OK;
}


/**
 * Here we initialize our settings to default values
 **/
void nrCSettings::initSettings(){
	
	// Now set some default values to our settings
	registerDefaults();
	
}


/**
 * Parse settings from given string
 * @param str is a string containing one line from settings file. It should look like "variable_name=34.5" 
 * @return either NR_OK or:
 * 		- NR_UNKNOWN_ERROR if an error occurs
 * 
 * @note Before you parse the settings all variables must be registered
 **/
nrResult nrCSettings::parseSettings(string str){

	try{
			
		// local variables
		int bias=0; 
		string name, value;
		
		//test for bias
		if((str[0]=='+')||(str[0]=='-')){
			bias=((str[0]=='+')*2)-1; //+ maps to 1*2-1=1, - maps to 0*2-1=-1
			str = str.substr(1); //remove the first character from the string
		}
		
		//test for '='
		int eqPos=str.find('=');
		if(eqPos!=-1){
			//there's an = sign in there
			//so split either side of it
			name=trim(str.substr(0,eqPos));
			value=trim(str.substr(eqPos+1));
				
		}else if (str.length() > 1){
			// check whenever this is allowed line (no comments)
			if (str.find('/') < str.length() || 
				str.find('#') < str.length()){
				return NR_OK;
			}				
			
			//there's no equal sign
			//we use the bias to construct a boolean value
			//so that flags can be +flag (mapping to flag=1) or -flag (mapping to flag=0)
			name=str;
			char szBuf[64];
			sprintf(szBuf,"%i",(bias+1)/2);
			value=szBuf;
		}
	
		//set the variable
		return set(name, value);

	}catch(...){
		return NR_UNKNOWN_ERROR;
	}
}

/**
 * This functions read all supported OpenGL-Extensions and compare the given string to
 * the list of supported by current hardware.
 * @param extensionsStr - list of all extensions that should be supported for our software
 * @return either NR_OK or:
 * 		- NR_EXTENSION_NOT_SUPPORTED if one of the extensions is not supported
 *		- NR_UNKNOWN_ERROR if an error occurs
 **/
nrResult nrCSettings::initExtensions(const char* extensionsStr){

	try {
		std::string str;
		
		// Go through extension list and set exctracted values to false
		for (unsigned int i=0; i < strlen(extensionsStr); i++){
			if ((extensionsStr[i] == ' ' || extensionsStr[i] == '\n' || extensionsStr[i] == ',')
				&& str.length() >= 1) {
				extensionMap[str] = false;
				str = "";
			}else{
				str += extensionsStr[i];
				if (i == strlen(extensionsStr) - 1) {
					// ok we have last char and this is not a delimiter
					extensionMap[str] = false;
				}
			}
		}
		
		// Now we read all supported extensions and set values to true
		const char* extensions = (char*)glGetString(GL_EXTENSIONS);

		for (unsigned int i=0; i < strlen(extensions); i++){
			if (extensions[i] == ' ' || extensions[i] == '\n'){
				extensionMap[str] = true;
				str = "";
			}else{
				str += extensions[i];
			}
		}
		
		// Now we check if we have any false value 
		//  => this extension is not supported but we need it for our application
		std::map<std::string,bool>::const_iterator it = extensionMap.begin();
		while (it != extensionMap.end()) {
			if ((*it).second == false) return NR_EXTENSION_NOT_SUPPORTED;
			it ++;
		}
		
		// Get some other settings
		glGetIntegerv( GL_MAX_TEXTURE_UNITS_ARB, &multiTexCount );
		
		// VBo-Extension
		vboSupported = extensionMap["GL_ARB_vertex_buffer_object"];
	}catch(...){
		return NR_UNKNOWN_ERROR;
	}
	return NR_OK;
}


/**
 * Returns count of supported extensions
 **/
int nrCSettings::getExtensionsCount(){
	return extensionMap.size();
}


/**
 * Says if Extension with index i is supported
 **/
bool nrCSettings::isExtensionSupported(int i){
	std::map<std::string,bool>::iterator it = extensionMap.begin();
  	for (int l=0; l<i; l++) it ++;
	return (*it).second;
}

const std::string& nrCSettings::getExtensionName(int i){
	std::map<std::string,bool>::iterator it = extensionMap.begin();
  	for (int l=0; l<i; l++) it ++;
	return (*it).first;
}

/**
 * This function loads configuration file.
 * @param fileName - name of the file containing settings
 * @return either NR_OK or:
 *		- NR_FILE_NOT_FOUND if file was not found
 *		- NR_UNKNOWN_ERROR
 **/
nrResult nrCSettings::loadSettings(const char* fileName){

	try{
	
		// open file for streaming
		nrLog.Log(NR_LOG_ENGINE, "Open %s file to load settings", fileName);
		std::ifstream in(fileName);
		
		if(!in.is_open()){
			nrLog.Log(NR_LOG_ENGINE, "ERROR: Could not open file !!!", fileName);		
			return NR_FILE_NOT_FOUND;
		}
		
		// read lines out and parse them
		while(!in.eof()){
			char szBuf[1024];
			in.getline(szBuf,1024);
			parseSettings(szBuf);
		}
		
		nrLog.Log(NR_LOG_ENGINE, "Settings are OK");

	} catch (...){
		return NR_UNKNOWN_ERROR;
	}
	
	// OK
	return NR_OK;
	
}


/**
 * Store current settings in the profile file of current user. Settings will be stored under
 * the given directory name + activated profile name.
 * @param profileDirPath - path to profile directory.
 * @return either NR_OK or:
 * 		- NR_PROFILE_NOT_LOADED if no profile is activated until now
 *		- NR_UNKNOWN_ERROR
 **/
nrResult nrCSettings::storeProfile(const char* profileDirPath){

	try{ 
		
		// If no current profile is activated then we got error
		if (activatedProfile.length() == 0)
			return NR_PROFILE_NOT_LOADED;
		
		// Open profile file
		std::string fileName(profileDirPath);
		fileName += activatedProfile;
		fileName += "/config.cfg";
		
		FILE* file = fopen(fileName.c_str(),"w");
		if (file == NULL)return NR_UNKNOWN_ERROR;
		
		nrLog.Log(NR_LOG_ENGINE, "Store settings in %s", fileName.c_str());
		
		// Store data into file
		_SET_MAP::iterator it = settings.begin();
		while (it != settings.end()) {
			string name = (*it).first;
			fwrite(name.c_str(),1,name.length(), file);
			fwrite("=",1,1,file);
				
			char value[256];
			sprintf (value,"%s", (*it).second->str());
			fwrite(&value,1,strlen(value), file);

			fwrite("\n",1,1,file);
			it++;
		}

		fclose(file);
		
		nrLog.Log(NR_LOG_ENGINE,"OK");

	}catch(...){
		return NR_UNKNOWN_ERROR;
	}

	return NR_OK;
}

 
/**
 * Create profile and store default settings in the configuration file of the profile
 * @param userName - name of the profile
 * @param profileDirPath - path to the directory containing profiles
 * @return either NR_OK or:
 *		- NR_PROFILE_ALREADY_EXISTS if such a profile already exists 
 *		- return values from loadSettings()/storeProfile()
 **/
nrResult nrCSettings::createProfile(const char* userName,const char* profileDirPath){

	std::string fileName(profileDirPath);
	fileName += userName;

	std::string directoryName(profileDirPath);
	directoryName = fileName;

	fileName += "/config.cfg";
	
	// Check if such profile already exists
	FILE* file = fopen(fileName.c_str(), "r");
	if (file != NULL){
		fclose (file);
		return NR_PROFILE_ALREADY_EXISTS;		
	}
	
	// Create directory to store the profile
	createDirectory(directoryName);
	
	// Load Default settings
	fileName = profileDirPath;
	fileName += "default.cfg";
	nrResult ret = loadSettings(fileName.c_str());
	if (ret != NR_OK) return ret;
	
	// Set current profile and store it
	activatedProfile = userName;
	return storeProfile(profileDirPath);
	
}

/**
 * Load profile settings and use this
 * @param userName - name of the profile
 * @param profileDirPath - path to directory containing profiles
 * @return either NR_OK or:
 * 		- return values from loadSettings()
 **/
nrResult nrCSettings::useProfile(const char* userName,const char* profileDirPath){

	std::string fileName(profileDirPath);
	fileName += userName;
	fileName += "/config.cfg";
	
	nrResult ret = loadSettings(fileName.c_str());
	
	if (ret == NR_OK){
		activatedProfile = userName;
		return NR_OK;
	}
	return ret;
}


/**
 * Returns Dator according to given variable name
 * @param _str - name of the variable
 * @return reference to Dator for the given variable
 * @throw string error if such a variable does not exists
 **/
nrIDator& nrCSettings::get(const string _str){
	
	if (!isRegistered(_str)){
		nrLog.Log(NR_LOG_ENGINE, "No variable %s registered", _str.c_str());	
		throw(string("Error Occurs. Take a look to Log Files"));
	}
	
	// get dator	
	return *settings[_str];
}

 

/**
 * setProperty - Set new value for a property
 * @param property is the variable name to be set
 * @param value is a string representing new value
 * @param bias tells us what we have to do with multiple values variables
 *		  if it is set to >0 then we add new values, <0 delete it and  =0
 *		  tells us to replace settings with this value
 * @return either NR_OK or:
 * 		- NR_SETTINGS_VAR_NOT_REGISTERED if such a variable does not registered
 *		- NR_UNKNOWN_ERROR
 **/
nrResult nrCSettings::set(const string property,string value, int bias){

	try{
		
		// if property name is empty, then not set
		if (property.length()==0 || value.length() == 0){
			return NR_OK;	
		}
		
		// check whenever such variable exists
		if(!isRegistered(property)){
			nrLog.Log(NR_LOG_ENGINE, "Try to set %s but is not registered", property.c_str());
			return NR_SETTINGS_VAR_NOT_REGISTERED;	
		}
		
		// check whenever we have multiple value variable
		if(settings[property]->hasMultipleValues()){
			std::list<std::string> valueList;
			valueList.clear();

			//check for semicolon-seperated values
			if(value.find(';') != value.length()){
				//split the string into semicolor-seperated chunks
				int first=0, last;
				while((last=value.find(';',first))!=-1){
					valueList.push_back(value.substr(first,last-first));
					first=last+1;
				}
				valueList.push_back(value.substr(first));
			}else{
				valueList.push_back(value);
			}
	
			for(std::list<std::string>::iterator it=valueList.begin(); it!=valueList.end(); it++){
				if(bias>0){
					(*settings[property])+=(*it);
				}else if(bias<0){
					(*settings[property])-=(*it);
				}else{
					(*settings[property])=(*it);
				}
			}
		}else{
			//just assign the value
			*(settings[property])=value;
		}
	
	}catch(...){
		return NR_UNKNOWN_ERROR;
	}	
	
	return NR_OK; 
}
	 

/**
 * Returns vector with registered variable names
 * @return vector<string> containing all registered variable names
 **/
vector<string> nrCSettings::getVariableNames(){

	// local variables
	vector<string> vars;
	
	// go through the map and copy the names out
	_SET_MAP::iterator it = settings.begin();
  	while (it != settings.end()) {
		vars.push_back((*it).first);
		it ++;
	}
	
	return vars;
}

#undef _SET_MAP
