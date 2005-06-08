/* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup, 
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Art Tevs <tevs@mpi-sb.mpg.de>
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


#ifndef _STUNTS_OGRE_TIME_HPP_
#define _STUNTS_OGRE_TIME_HPP_

#include <OGRE/Ogre.h>
#include <nrEngine/nrEngine.h>

namespace stunts {
  
	/**
	 * This class is a binding between Ogre's Timer and nrEngine's Clock System.
	 * This class provide functionality getting of time method that will be
	 * used by the nrEngine to drive the clock.
	 **/
	class COgreTimer : public nrITimeSource
	{
		private:
			Ogre::Timer*	timer;
			
		public:
			//! Create the timer through Ogre
			COgreTimer()
			{
				timer = Ogre::PlatformManager::getSingleton().createTimer();
				timer->reset();
			}	
			
			//! Release the Ogre's timer
			~COgreTimer()
			{
				Ogre::PlatformManager::getSingleton().destroyTimer(timer);
			}
			
			//! Get the current time in seconds since the system was initialized
			float64 getTime()
			{
				// get the time
				unsigned long time = timer->getMicroseconds();
				
				// convert the time
				float64 ftime = (static_cast<float64>(time) / 1000000.0f);
				
				return ftime;
			}
			
	};
	  
}

#endif
