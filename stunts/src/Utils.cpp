
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

} // namepsace 
