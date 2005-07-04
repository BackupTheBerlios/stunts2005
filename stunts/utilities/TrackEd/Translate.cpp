#include "translate.h"

#include <Fl/Fl_Button.H>

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

struct tentry {
	float OffX;
	float OffZ;
	std::string RotY;
	std::string ScaleAxis;
	std::string ScaleVal;
	std::string Shadow;
	std::string MeshName;
	std::string Extra;
};

std::map<std::string, tentry> tmatrix;
std::string tmp;

void transReload() {

	tmp.clear(); {
		std::ifstream in( "./export/track_object_template.xml" );
		while( !in.eof() ) {
			std::string line;
			getline( in, line );
			tmp += line + "\n";
		}
		in.close();
	}

	tmatrix.clear(); {
		std::ifstream in( "./export/translate.txt" );
		{ std::string ignore; getline( in, ignore ); }
		while( !in.eof() ) {
			std::string id;
			tentry cur;

			std::string line;
			getline( in, line );

			std::istringstream lin( line );

			lin >> id;
			lin >> cur.OffX >> cur.OffZ;
			lin >> cur.RotY;
			lin >> cur.ScaleAxis >> cur.ScaleVal;
			lin >> cur.Shadow;
			lin >> cur.MeshName;
			getline( lin, cur.Extra );

			if( id != "" ) 
				tmatrix[id] = cur;
		}
		in.close();
	}

}


void translate( std::ofstream &out, Fl_Button* o, float x, float z ) {

	std::string currentTmp = tmp;	

	std::string id = o->tooltip();

	if( tmatrix.find( id ) == tmatrix.end() ) return;

	const char* _OBJECT_COMMENT_ = "_OBJECT_COMMENT_";
	const char* _ROT_Y_ = "_ROT_Y_";
	const char* _POS_X_ = "_POS_X_";
	const char* _POS_Z_ = "_POS_Z_";
	const char* _SCALE_AXIS_ = "_SCALE_AXIS_";
	const char* _SCALE_VALUE_ = "_SCALE_VALUE_";
	const char* _SHADOW_ = "_SHADOW_";
	const char* _MESH_NAME_ = "_MESH_NAME_";
	const char* _EXTRA_XML_ = "_EXTRA_XML_";

	std::ostringstream posX;
	std::ostringstream posZ;

	posX << (x + tmatrix[id].OffX);
	posZ << (z + tmatrix[id].OffZ);

	currentTmp.replace(
		currentTmp.find(_OBJECT_COMMENT_, 0),
		strlen(_OBJECT_COMMENT_), tmatrix[id].MeshName);
	currentTmp.replace(
		currentTmp.find(_ROT_Y_, 0),
		strlen(_ROT_Y_), tmatrix[id].RotY);
	currentTmp.replace(
		currentTmp.find(_POS_X_, 0),
		strlen(_POS_X_), posX.str().c_str() );
	currentTmp.replace(
		currentTmp.find(_POS_Z_, 0),
		strlen(_POS_Z_), posZ.str().c_str() );
	currentTmp.replace(
		currentTmp.find(_SCALE_AXIS_, 0),
		strlen(_SCALE_AXIS_), tmatrix[id].ScaleAxis);
	currentTmp.replace(
		currentTmp.find(_SCALE_VALUE_, 0),
		strlen(_SCALE_VALUE_), tmatrix[id].ScaleVal);
	currentTmp.replace(
		currentTmp.find(_SHADOW_, 0),
		strlen(_SHADOW_), tmatrix[id].Shadow);
	currentTmp.replace(
		currentTmp.find(_EXTRA_XML_, 0),
		strlen(_EXTRA_XML_), tmatrix[id].Extra);
	for (int i = 0; i < 2; i++)
	{
		currentTmp.replace(
		currentTmp.find(_MESH_NAME_, 0),
		strlen(_MESH_NAME_), tmatrix[id].MeshName);
	};

	out << currentTmp;
}
