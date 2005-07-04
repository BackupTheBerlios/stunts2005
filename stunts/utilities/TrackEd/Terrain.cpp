#include "Terrain.h"

#include <FL/Fl_PNG_Image.H>

#include <vector>
#include <iostream>

#include "TrackEdUI.h" //// TEMP?

std::vector<std::vector<bool> > terrain;


// cw = total cells width
// ch = total cells height
// t = threshold
void InitTerrain( std::string file, int cw, int ch, int t ) {
	Fl_PNG_Image* temp = new Fl_PNG_Image( file.c_str() );

	int w = temp->w() / cw;
	int h = temp->h() / ch;

	// later.

	delete temp;

}


// x = cell x position
// y = cell y position
bool mountain( int x, int y ) {
	
}
