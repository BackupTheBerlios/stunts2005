#ifndef __TRANSLATE_H
#define __TRANSLATE_H

#include <Fl/Fl_Button.H>
#include <fstream>

void transReload();

void translate( std::ofstream &out, Fl_Button* o, float x, float z );



#endif
