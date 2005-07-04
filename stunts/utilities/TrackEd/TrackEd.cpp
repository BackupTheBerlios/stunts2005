#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Tooltip.H>

#include "TrackEdUI.h"

int main (int argc, char *argv[])
{

	InitImg();

	CTrackEdUI TrackEdUI;

	Fl_PNG_Image *temp = new Fl_PNG_Image( "terrain/terrain1.png" );

	TrackEdUI.debug1 -> value( temp->w() );
	TrackEdUI.debug2 -> value( temp->h() );
	TrackEdUI.debug3 -> value( temp->d() );
	TrackEdUI.debug4 -> value( temp->ld() );
	TrackEdUI.debug5 -> value( temp->count() );

	delete temp;


	TrackEdUI.show( argc, argv );

//	Fl_Tooltip::enable(false);
	int result = Fl::run();

	FreeImg();

    return result;
}
