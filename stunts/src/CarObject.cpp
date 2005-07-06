/* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup,
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Andreas Maurer <andi@andile.de
 *                    Art Tevs <tevs@mpi-sb.mpg.de>
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

#include <stdlib.h>

using boost::shared_ptr;

namespace stunts {

	//--------------------------------------------------------------------------
	CCarObject::CCarObject():CBaseObject()
	{

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
	
	void CCarObject::respawn(Ogre::Vector3 _position, Quaternion * direction = NULL)
	{
		//shift the car in the air
		Ogre::Vector3 _h = Ogre::Vector3(0., 3., 0.);
		_position = _position + _h;
		
		//and teleport it to its new position
		mVehicle->setPosition(_position);
		/*
		if(direction != NULL)
			mVehicle->getBody()->setOrientation(*direction);
		*/
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

		// Look for gearbox
		elem = rootElem->FirstChildElement("gearbox");
		if (elem)
		{
			// Some variables
			TiXmlElement* gear 		= NULL;
			float32 ratio 			= 0.0f;
			std::string name;

			// Set carRatio, if found in XML file this Value will be overwritten
			float 			carRatio 	= 1.0f;

			// 2 vectors to save the gear-ratios in
			std::vector<char> 	names;
			std::vector<float32> 	ratios;

			// Get first element
			gear = elem->FirstChildElement("ratio");

			while (gear)
			{
				// Get ratio
				nrCDator<float32> _ratio(ratio);
				_ratio 	= std::string(gear->Attribute("value"));

				// Get gearname
				name 	= (std::string)(gear->Attribute("name"));

				// Go for next gear
				gear 	= gear->NextSiblingElement("ratio");

				// Save names and ratios
				if (name == "R")
				{
					names.push_back('R');
					ratios.push_back(((Ogre::Real) _ratio) * (-1));
				}
				else if (name == "C")
				{
					carRatio = ((Ogre::Real) _ratio) * (-1);
				}
				else if ((int)name[0] > 0)
				{
					names.push_back(name[0]);
					ratios.push_back((Ogre::Real) _ratio);
				};

			};

			// Multiply carRatio with gear ratios and add to car
			int i = 0;
			while (i < ratios.size())
			{
				this->addGear(carRatio * ratios[i], names[i]);
				nrLog.Log(NR_LOG_APP, "CCarObject::parseSettingsCar(): Added Gear %d %f", names[i], ratios[i]);
				i++;
			};
			
			// Set first gear
			this->setGear('1');

			nrLog.Log(NR_LOG_APP, "CCarObject::parseSettingsCar(): Gearbox loaded!");

			return false;
		}
		
		//stretch the car
		mVehicle->getSceneNode()->scale(m_scale);
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


	//--------------------------------------------------------------------------
	void CCarObject::setInputs(float steer, float throttle, float brake)
	{
		this->mVehicle->setInputs(steer, throttle, brake);
	}


	//--------------------------------------------------------------------------
	void CCarObject::setInputs(bool left, bool right, bool throttle, bool brake)
	{
		if (((this->getGearCode() == 'R') && (throttle == false)) || ((this->getSpeed() < 1) && (brake == true)))
		{
			// Switch to backwardsGear if not already done
			if (this->getGearCode() != 'R') this->setGear('R');
			this->mVehicle->setInputs(left, right, brake, throttle);
		} else
		{
			// Switch to forwardGear if backwardsGear is still in
			if (this->getGearCode() == 'R') this->setGear('1');
			this->mVehicle->setInputs(left, right, throttle, brake);
		}
		//std::cout << "Gang eingelegt: " << getGearCode() << std::endl;
	}


	//--------------------------------------------------------------------------
	void CCarObject::setAutoBox()
	{
		if (this->mAutoBox == false)
		{
			this->mAutoBox = true;
			this->mVehicle->getEngine()->setAutoBox(true);
		} else
		{
			this->mAutoBox = false;
			this->mVehicle->getEngine()->setAutoBox(false);
		};
	}

	//-----------------------------------------------------------------------
	float CCarObject::getRPM(){

		float wheelRPM = mVehicle->getWheel(0)->getRPM();

		float gearRatio = mVehicle->getEngine()->getCurrentGearRatio();

		return wheelRPM * (1.0f/gearRatio);
							
	}
	
};
