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


#ifndef _STUNTS_ATMOSPHERE_H_
#define _STUNTS_ATMOSPHERE_H_

#include <string>
#include "External/tinyxml/tinyxml.h"
#include <vector>
#include <OGRE/Ogre.h>

//------------------------------------------------------------------------------
//--- predeclare this class
//------------------------------------------------------------------------------
namespace stunts
{
	class CAtmosphere;
}

//------------------------------------------------------------------------------
//--- namespace stunts
//------------------------------------------------------------------------------
namespace stunts
{
	class CAtmosphere
	{
		
	public:
		CAtmosphere();
		virtual ~CAtmosphere();
				
		/**
		 * Import the atmosphere from a file.
		 * @param fileName Name of the file containing terrain data.
		 * @param xmlP
		 * @return false if error occurs otherwise true
		 **/
		virtual bool importFromFile(const char* fileName, const std::string& xmlPath);
				bool importFromFile(const std::string fileName, const std::string& xmlPath){ return importFromFile(fileName.c_str(), xmlPath); }
		
		/**
			* Let import the settings of the object from a string. The string must be a readed 
			* XML-File that has got the description of the object. Each derived class should
			* overwrite this function, to be able to read it's own declarations
			* @param xmlSettings String containing XML-Settings (incl. Root-Tag)
			* @param xmlPath path where the XML file was found (needed for import tags)
			* @return false if the reading was succesful
			**/
		virtual bool importFromString(const char* xmlSettings, const std::string& xmlPath);
				bool importFromString(const std::string xmlSettings, const std::string& xmlPath){ return importFromString(xmlSettings.c_str(), xmlPath); }

	protected:
	
		/**
		 * Parse settings from a given XML-Tree.
		 * @param rootElem Root element of the XML-Tree (here: atmosphere - Tag)
		 * @param xmlPath path of this XML-File, for importing
		 * @return true if error occurs
		 **/
		bool parseSettings(TiXmlElement* rootElem, const std::string& xmlPath);

		/**
		 * Read light declarations from the XML-Tree
		 **/
		void readLights(TiXmlElement* elem);

		
		//! Store all lights as a node in the scene graph
		std::vector<Ogre::SceneNode*>	mLightNode;
		std::vector<Ogre::Light*>		mLights;
		
		//! Store the sun image
		Ogre::BillboardSet*			mSunBillboardSet;
		Ogre::Billboard*			mSunBillboard;
		
	};	
}

#endif //_TERRAIN_H_




