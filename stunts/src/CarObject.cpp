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
		mVehicle.reset();

	}

	//--------------------------------------------------------------------------
	bool CCarObject::parseSettings(TiXmlElement* rootElem, const std::string& xmlPath)
	{
		nrLog.Log(NR_LOG_APP, "CCarObject::parseSettings(): Start parsing the settings");

		if (rootElem == NULL){
			nrLog.Log(NR_LOG_APP, "CCarObject::parseSettings(): Not valid XML-Element given");
			return true;
		}

		// Ok now let the base object parse it's settings
		if (CBaseObject::parseSettings(rootElem, xmlPath))
			return true;

		//then parse the car's special settings
		if (parseSettingsCar(rootElem, xmlPath))
			return true;

		return false;
	}

	//--------------------------------------------------------------------------
	bool CCarObject::parseSettingsCar(TiXmlElement* rootElem, const std::string& xmlPath)
	{
		nrLog.Log(NR_LOG_APP, "CCarObject::parseSettingsCar(): Start parsing the settings");

		if (rootElem == NULL){
			nrLog.Log(NR_LOG_APP, "CCarObject::parseSettingsCar(): Not valid XML-Element given");
			return true;
		}

		// variables
		TiXmlElement* elem	= NULL;
		TiXmlElement* elemWp	= NULL;
		TiXmlElement* elemPos	= NULL;


		// find if we want to import a file
		elem = rootElem->FirstChildElement("import");
		if (elem)
			CCarObject::importFromFile(xmlPath + elem->Attribute("file"), xmlPath);


		// get the name of the object
		elem = rootElem->FirstChildElement("ode");
		if (elem)
		{
			const char* odeName = elem->Attribute("name");
			const char* odeFile = elem->Attribute("file");

			if (!odeName || !odeFile)
			{
				nrLog.Log(NR_LOG_APP, "CCarObject::parseSettingsCar(): No name and file attributes");
				return true;
			}

			mVehicle.reset(new OgreOde_Prefab::Vehicle(mName));
			mVehicle->load(odeFile, odeName);
			mVehicle->setPosition(m_position);
		}
	}


	//--------------------------------------------------------------------------
	bool CCarObject::importFromFile(const char* fileName, const std::string& xmlPath)
	{

		// load the xml document
		shared_ptr<TiXmlDocument> mDoc (new TiXmlDocument(fileName));
		if (!mDoc->LoadFile())
		{
			nrLog.Log(NR_LOG_APP, "CCarObject::importFromFile(): Can not load the file \"%s\"", fileName);
			return true;
		}

		// Get the root element from the file
		TiXmlElement* rootElem = mDoc->FirstChildElement(this->getObjectType());

		// get the first root element.
		if (rootElem == NULL)
		{
			nrLog.Log(NR_LOG_APP, "CCarObject::importFromFile(): Can not find root node \"%s\"", this->getObjectType());
			return true;
		}

		return CCarObject::parseSettingsCar(rootElem, xmlPath);
	}
};
