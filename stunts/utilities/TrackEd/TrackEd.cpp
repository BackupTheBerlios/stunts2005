#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Tooltip.H>

#include "TrackEdUI.h"

int main (int argc, char *argv[])
{

	InitImg();
	InitTerrain();

	CTrackEdUI TrackEdUI;

	TrackEdUI.show( argc, argv );

//	Fl_Tooltip::enable(false);
	int result = Fl::run();

	FreeImg();
	FreeTerrain();

    return result;
}
