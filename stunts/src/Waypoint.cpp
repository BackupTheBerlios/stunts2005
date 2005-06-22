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
 
#include "Waypoint.hpp"

using boost::shared_ptr;

namespace stunts 
{
	// ObjectId is required to search for the nearest waypoint of OTHER objects!
	CWaypoint::CWaypoint(Ogre::Vector3 pos, int objectId)
	{
		this->setVector(pos);
		this->objectId	= objectId;
	};
	
	CWaypoint::~CWaypoint()
	{
	};

	boost::shared_ptr<CWaypoint> CWaypoint::getNext()
	{
		return this->next;
	};
	
	boost::shared_ptr<CWaypoint> CWaypoint::getPrev()
	{
		return this->prev;
	};
	
	boost::shared_ptr<CWaypoint> CWaypoint::getFirst()
	{
		return this->first;
	};
	
	boost::shared_ptr<CWaypoint> CWaypoint::getLast()
	{
		return this->last;
	};
	
	void CWaypoint::setVector(Ogre::Vector3 pos)
	{
		this->pos = pos;
	};
	
	Ogre::Vector3 CWaypoint::getVector()
	{
		return this->pos;
	};
	
	int CWaypoint::getObjectId()
	{
		return this->objectId;
	};
	
	void CWaypoint::setNext(boost::shared_ptr<CWaypoint> waypoint)
	{
		this->next = waypoint;
	}
	
	void CWaypoint::setPrev(boost::shared_ptr<CWaypoint> waypoint)
	{
		this->prev = waypoint;
	}
	
	void CWaypoint::setFirst(boost::shared_ptr<CWaypoint> waypoint)
	{
		this->first = waypoint;
	}
	
	void CWaypoint::setLast(boost::shared_ptr<CWaypoint> waypoint)
	{
		this->last = waypoint;
	}
};
