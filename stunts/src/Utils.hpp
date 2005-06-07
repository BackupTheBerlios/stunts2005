
#ifndef __STUNTS_UTILS_H
#define __STUNTS_UTILS_H

#include <string>

namespace stunts{

	/**
	 * Detect the path according to the file name
	 **/
	std::string getPathFromFileName(std::string fileName);
	
	/**
	 * Create random name, that are free
	 **/
	std::string createName();
	
	
} // namepsace 

#endif

