/* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup,
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Andreas Maurer <andi@andile.de
 *                    Art Tevs <tevs@mpi-sb.mpg.de>
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
	CCarObject::CCarObject():CBaseObject()
	{
// TODO please load the settings from the XML file
		//mVehicle.reset(new OgreOde_Prefab::Vehicle("Jeep"));
		//mVehicle->load("jeep_ode.xml");
	}
	//--------------------------------------------------------------------------
	CCarObject::CCarObject(char* xmlSettingsString, const std::string& xmlPath) :
				CBaseObject(xmlSettingsString, xmlPath)
	{
		// We do not need to specify here more, bacause the base object will call the
		// appropriate overrided parseSettings mehtod, where we are defining how to
		// parse the settings
	}

	//--------------------------------------------------------------------------
	CCarObject::~CCarObject()
	{
		// RemoveObject from memory
		// TODO
	}

	//--------------------------------------------------------------------------
	bool CCarObject::parseSettings(TiXmlElement* rootElem, const std::string& xmlPath)
	{

		if (rootElem == NULL){
			nrLog.Log(NR_LOG_APP, "CCarObject::parseSettings(): Not valid XML-Element given");
			return true;
		}

		// variables
		TiXmlElement* elem = NULL;
		TiXmlElement* smElem = NULL;

		// Ok now let the base object parse it's settings
		return CBaseObject::parseSettings(rootElem, xmlPath);
	}
	bool CCarObject::brake(float brakePedal)
	{
		this->m_brake = brakePedal;
	};


	bool CCarObject::steer(float steer)
	{

	};

};
