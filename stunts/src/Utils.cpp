
#include "Utils.hpp"
#include <nrEngine/nrEngine.h>

namespace stunts{

	/**
	 * Detect the path according to the file name
	 **/
	std::string getPathFromFileName(std::string fileName){
	
		std::string path;
		uint32 length = 0;
		
		for (uint32 i = fileName.length() - 1; i >= 0; i--){
			if (fileName[i] != '/') length = i;
			else {
				length = i;
				break;
			}
		}
		
		path = fileName.substr(0, length);
		
		// if no path
		if (path == fileName || path.length() == 0) path = ".";
		return (path + "/");
	}

	/** Create Name */	
	std::string createName(){
		static int counter = 0;
		counter ++;
		
		char name[128];
		sprintf(name, "__Unknown_Name_%d", counter);
		
		return std::string(name);
	}

} // namepsace 
