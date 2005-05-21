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

#ifndef _NR_CSETTINGS_H_
#define _NR_CSETTINGS_H_

/*--------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include <string>
#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "../nrTypes.h"
#include "../nrEngine.h"
#include "../nrISingleton.h"
#include "../nrDator.h"

using boost::shared_ptr;
using boost::weak_ptr;
using std::string;
using std::map;

//------------------------------------------------------------------------------
//	Definitions needed not only here
//------------------------------------------------------------------------------
typedef  shared_ptr<nrIDator> _nrData;

/**
 * This is our Settings Manager. You can store here variables (Dators) to access to them from
 * other places in your software. Also the values of registered variables will be stored in
 * config file. nrCSettings supports profiles. So you can gengerate settings for more users.
 * Also there is support to get information about OpenGL-Extensions.
 **/
class nrCSettings : public nrISingleton<nrCSettings> {
private:
	
	// This are global properties, which cannot be changed	
	int	multiTexCount;	
	bool vboSupported;

	// Some maps
	map <string, bool>	extensionMap;
	map <string, _nrData > settings;
	
	// Here we will store our function pointers
	//std::vector<nrSettingsOnChanging>	callbackFuncs;

	// Store the last sucessfully readed line number
	int	_lastReadedLineNr;

	// Store user name to support profiles
	string activatedProfile;

	int	gameMode;

public:
	
	// Constructor & Destructor
	nrCSettings();
	virtual ~nrCSettings();

	// Access methods
	nrResult registerVariable(const string &name, shared_ptr<nrIDator> var);
	nrResult deregisterVariable(const string &name);
	nrResult registerDefaults();

	// Methodes
	void		initSettings();
	nrResult	loadSettings(const char* fileName);
	nrResult	initExtensions(const char* extensionsStr);

	// This methods will helps us to get extension supporting information
	int			getExtensionsCount();
	bool		isExtensionSupported(int i);
	const string& getExtensionName(int i);
	bool		isVBOSupported(){return vboSupported;}
	
	// With help of user name we can support profiles for our game
	nrResult useProfile(const char* userName,const char* profileDirPath = "../config/profiles/");
	nrResult storeProfile(const char* profileDirPath = "../config/profiles/");
	nrResult createProfile(const char* userName,const char* profileDirPath = "../config/profiles/");
	nrResult parseSettings(string str);

	// Getter for global unchangeable values
	int	getMultiTexCount(){return multiTexCount;}
	
	// Get current Game-Mode and set it (to be faster)
	void	setGameMode(int mode){gameMode = mode;}
	int		getGameMode(){return gameMode;}
	
	// Setter and Getter for out changeable settings
	nrIDator& get(const string property);
	nrResult set(const string property,string value, int bias=0);
	
	/**
	 * Is there any variable with this name registered
	 **/
	bool isRegistered(const string& property){
		return settings.find(property) != settings.end();	
	}
		
	std::vector<string> getVariableNames();
	
		
	// Helper methods
	bool	isExtensionSupported(const char* extName);
	bool	isExtensionSupportedDirect(const char* extName);
	
	// Gives you the number of the line, which is readed as last before error or EOF
	int		getLastReadedLine(){return _lastReadedLineNr;}
	
	// direct access variables
	unsigned int	iTrianglesDrawn;
	
};

#endif
