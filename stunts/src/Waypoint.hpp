 /* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup,
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Andreas Maurer <andi@andile.de
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

#include <boost/shared_ptr.hpp>
#include <OGRE/Ogre.h>

namespace stunts 
{
	class CWaypoint
	{
	
		public:
			/**
			* Constuctor
			*/
			CWaypoint(Ogre::Vector3 pos, int objectId);
			
			
			
			
			/** 
			* Deconstructor
			*/
			~CWaypoint();
			
			
			// Get pointer to next Waypoint
			boost::shared_ptr<CWaypoint> getNext();
			
			// Get pointer to previous Waypoint
			boost::shared_ptr<CWaypoint> getPrev();
			
			// Get pointer to first Waypoint
			boost::shared_ptr<CWaypoint> getFirst();
			
			// Get pointer to last Waypoint
			boost::shared_ptr<CWaypoint> getLast();
			
			// Set coordinates of Vector
			void setVector(Ogre::Vector3 pos);
			
			// Get coordinates of Vector
			Ogre::Vector3 getVector();
			
			// Return ObjectId
			int getObjectId();
			
			
		//protected:
			// Different waypoint-pointers
			boost::shared_ptr<CWaypoint> 	next;
			boost::shared_ptr<CWaypoint> 	prev;
			boost::shared_ptr<CWaypoint> 	first;
			boost::shared_ptr<CWaypoint> 	last;
			Ogre::Vector3			pos;
			int 				objectId;
	};
};
