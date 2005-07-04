#include "Terrain.h"

#include <FL/Fl_PNG_Image.H>

#include <fstream>


std::string curTerrain;
                       
std::vector<std::vector<int> > terrain;

std::map<std::string, tdata> ters;


void InitTerrain( ) {
	std::ifstream in( "./terrain/index.txt" );
	{ std::string ignore; getline( in, ignore ); }
	while( !in.eof() ) {
		std::string name;
		in >> name;
		if( name != "" ) {
			in >> ters[name].file >> ters[name].tl >> ters[name].th;
		}
	}
	in.close();
}


void FreeTerrain( ) {
	std::map<std::string, tdata>::iterator i;
	for( i = ters.begin(); i != ters.end(); i++ ) {
	}
}


// cw = total cells width
// ch = total cells height
// tl = low threshold
// th = high threshold
void LoadTerrain( std::string name, int cw, int ch ) {

	if( ters.find( name ) == ters.end() ) name = "terrain1";

	std::string file = ters[name].file;
	double tl = ters[name].tl;
	double th = ters[name].th;

	curTerrain = name;
	std::string path = std::string("./terrain/") + file;
	Fl_Image* hmap = new Fl_PNG_Image( path.c_str() );

	if( hmap == NULL || hmap->d() > 1 || hmap->ld() > 0 ) {
		delete hmap;
		hmap = new Fl_Image( cw, ch, 1 );
		curTerrain = "error";
	}

	double hw = hmap->w();
	double hh = hmap->h();
	double w = hw / cw;
	double h = hh / ch;

	terrain.clear();

	const unsigned char *data = (const unsigned char*)hmap->data()[0];
	for( int x = 0; x < cw; x++ ) {
		std::vector<int> col;
		for( int y = 0; y < ch; y++ ) {
			int i = (int)(y*w + x);

			double avg = 0;
			for( int ax = (int)(x*w); ax < (int)((x+1)*w); ax++ )
				for( int ay = (int)(y*h); ay < (int)((y+1)*h); ay++ )
					avg += (int)data[(int)(ay*hh+ax)];
			avg /= w*h;

			if( avg < tl ) col.push_back( WATER );
			else if( avg > th ) col.push_back( MOUNTAIN );
			else col.push_back( EMPTY );
		}
		terrain.push_back( col );
	}

	delete hmap;
}


// x = cell x position
// y = cell y position
int tertype( int x, int y ) {
	return terrain[x][y];
}
