#ifndef __TERRAIN_H
#define __TERRAIN_H

#include <string>


// cw = total cells width
// ch = total cells height
// t = threshold
void InitTerrain( std::string file, int cw, int ch, int t );


// x = cell x position
// y = cell y position
bool mountain( int x, int y );


#endif
