#ifndef __images_h
#define __images_h

#include <string>
#include <map>
#include <FL/Fl_PNG_Image.h>

extern void InitImg();
extern void FreeImg();

extern std::map<std::string, Fl_PNG_Image*> img;

#endif
