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


#include "CarObject.hpp"
#include "External/tinyxml/tinyxml.h"

#include <nrEngine/nrEngine.h>

using boost::shared_ptr;

namespace stunts {

	//--------------------------------------------------------------------------
	CCarObject::CCarObject(): CBaseObject()
	{
	}
	
	//--------------------------------------------------------------------------
	CCarObject::~CCarObject()
	{
	}
	
	
	
	//--------------------------------------------------------------------------
	bool CCarObject::importFromFile(const char* fileName)
	{
		// load the xml document
		shared_ptr<TiXmlDocument> mDoc (new TiXmlDocument(fileName));
		if (!mDoc->LoadFile())
		{
			nrLog.Log(NR_LOG_APP, "CCarObject::importFromFile(): Can not load the file \"%s\"", fileName);
			return false;	
		}
		
		TiXmlElement* rootElem = mDoc->FirstChildElement(CCarObject::getObjectType());
		TiXmlElement* elem = NULL;
		
		// get the first root element.
		if (rootElem == NULL)
		{
			nrLog.Log(NR_LOG_APP, "CCarObject::importFromFile(): Can not find root node \"%s\"", CCarObject::getObjectType());
			return false;
		}
		
		
		return true;		
	}
		
}

