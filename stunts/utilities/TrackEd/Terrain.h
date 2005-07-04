#ifndef __TERRAIN_H
#define __TERRAIN_H

#include <FL/Fl_PNG_Image.H>

#include <map>
#include <vector>
#include <string>


#define WATER    -1
#define EMPTY     0
#define MOUNTAIN  1



struct tdata {
	std::string file;
	double tl;
	double th;
};



void InitTerrain( );
void FreeTerrain( );


// cw = total cells width
// ch = total cells height
// tl = low threshold
// th = high threshold
void LoadTerrain( std::string name, int cw, int ch );


// x = cell x position
// y = cell y position
int tertype( int x, int y );


extern std::string curTerrain;

extern std::map<std::string, tdata> ters;


#endif
