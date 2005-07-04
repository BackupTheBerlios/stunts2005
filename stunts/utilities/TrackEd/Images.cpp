#include "Images.h"

#include <fstream>

std::map<std::string, Fl_PNG_Image*> img;


void InitImg() {

	// load complete index, dirty.

	std::ifstream images( "./icons/index.txt" );
	if( images == NULL ) return;

	while( !images.eof() ) {
		std::string name;
		images >> name;

		std::string path = "./icons/" + name;
		std::string id = name.substr(0, name.size()-4);

		img[ id ] = new Fl_PNG_Image( path.c_str() );
	}

	images.close();
}



void FreeImg() {

	std::map<std::string, Fl_PNG_Image*>::iterator i;
	for( i = img.begin(); i != img.end(); i++ ) {
		delete( i->second );
	}

}
