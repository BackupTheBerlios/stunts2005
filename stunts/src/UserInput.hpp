/* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup,
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Christian Morbach <GameDevelopment@paratronic.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
 * USA.
 */


#define GCC_3_1
#define EXT_HASH

#include <OGRE/Ogre.h>
#include <nrEngine/nrEngine.h>

#ifndef _CUSERINPUT_H_
#define _CUSERINPUT_H_

namespace stunts
{

	class CUserInput : public nrITask
	{
		public:
			CUserInput();
			virtual ~CUserInput();

			// CUserInput interface

			// TODO: add member function declarations...

		protected:
			virtual nrResult taskInit();
			virtual nrResult taskStart();
			virtual nrResult taskUpdate();
			virtual nrResult taskStop();
			virtual const char* taskGetName(){return "UserInput";}

			// CUserInput variables

			// TODO: add member variables...

	};
}	//namespace stunts

#endif	//_CUSERINPUT_H_
