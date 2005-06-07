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

#include "BaseObject.hpp"
#include "ObjectInstantiator.hpp"

#include "External/tinyxml/tinyxml.h"
#include "Utils.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <nrEngine/nrEngine.h>

#include <OgreTask.hpp>

using boost::shared_ptr;

namespace stunts {

	//--------------------------------------------------------------------------
	// Small Helping function to parse position
	// <node unit="gridUnit">12</node>
	//--------------------------------------------------------------------------
	float parsePosition(TiXmlElement* node, CLevel* level){
	
		if (node == NULL || level == NULL) return 0.0f;
				
		// Use Dators to easy convert between strings and normal types like int,float
		float32 fvalue = 0.0f;
		nrCDator<float32> fval(fvalue);
		
		// Parsing the value and convert from grid units
		const char* str = node->GetText();
		if (str){
			fval = std::string(str);
			if (node->Attribute("unit") != NULL){
				// If we have got grid units, so convert them and store
				if (!strcmp(node->Attribute("unit"), "gridUnits"))
					fvalue = level->unitToMeter((int32)fval);
			}
			
		}
	
		return fvalue;
	}
		
	//--------------------------------------------------------------------------
	CBaseObject::CBaseObject()
	{
		mLevel = NULL;
		mSceneNode = NULL;
		mEntity = NULL;
		setName(createName().c_str());
	}
		
	//--------------------------------------------------------------------------
	CBaseObject::CBaseObject(char* xmlSettingsString, const std::string& xmlPath)
	{
		mLevel = NULL;
		mSceneNode = NULL;
		mEntity = NULL;
		setName(createName().c_str());
		
		// call the import from string function which will call the right
		// overwrited parseSettings method
		importFromString(xmlSettingsString, xmlPath);
	}
	
	
	//--------------------------------------------------------------------------
	CBaseObject::~CBaseObject()
	{
		// RemoveObject from memory
		// TODO
	}
	
	
	//--------------------------------------------------------------------------
	bool CBaseObject::parseSettings(TiXmlElement* rootElem, const std::string& xmlPath)
	{
		nrLog.Log(NR_LOG_APP, "CBaseObject::parseSettings(): Start parsing the settings");

		if (rootElem == NULL){
			nrLog.Log(NR_LOG_APP, "CBaseObject::parseSettings(): Not valid XML-Element given");
			return true;
		}
		
		// variables
		TiXmlElement* elem = NULL;
		
		
		// get the name of the object
		elem = rootElem->FirstChildElement("name");
		if (elem){
			const char* name = elem->GetText();
			if (name){
				nrLog.Log(NR_LOG_APP, "CBaseObject::parseSettings(): Set name to \"%s\"", name);
				setName(name);
			}
		}

		
		// find if we want to import a file
		elem = rootElem->FirstChildElement("import");
		if (elem)
			importFromFile(xmlPath + elem->Attribute("file"), xmlPath);
		
		// Get the geometry of the object
		elem = rootElem->FirstChildElement("geometry");
		if (elem)
			if (loadGeometry(elem, xmlPath)) return true;
	
		// if we found a controller, so bind it
		elem = rootElem->FirstChildElement("control");
		if (elem)
			bindController(elem->Attribute("name"));

			
		// NOTE: The geometry has to be parsed before, so we can access to created nodes
		if (mSceneNode){
			Ogre::Vector3 pos = mSceneNode->getPosition();
			
			// check whenever such kind of position tag exists
			elem = rootElem->FirstChildElement("posX");
			if (elem)
				pos.x = parsePosition(elem, mLevel);
			
			elem = rootElem->FirstChildElement("posZ");
			if (elem)
				pos.z = parsePosition(elem, mLevel);
									
			elem = rootElem->FirstChildElement("posY");
			if (elem){
				// setup normal y-position
				pos.y = parsePosition(elem, mLevel);
				
				// check whenever we want to setup bottom position
				if (elem->Attribute("type") != NULL){
					if (!strcmp(elem->Attribute("type"), "bottom"))
						pos.y = mLevel->Terrain()->getHeight(pos); 
				}
			}
			
			// Set the position of the node to new position
			mSceneNode->setPosition(pos);
		}
		
		nrLog.Log(NR_LOG_APP, "CBaseObject::parseSettings(): parsing is complete now");
		return false;
		
	}
	
	//--------------------------------------------------------------------------
	bool CBaseObject::loadGeometry(TiXmlElement* geomElem, const std::string& xmlPath){
	
		// Logging
		nrLog.Log(NR_LOG_APP, "CBaseObject::loadGeometry(): Load geometry definition of the object");
		
		if (geomElem == NULL){
			nrLog.Log(NR_LOG_APP, "CBaseObject::loadGeometry(): Not a valid XML-Element given");
			return true;
		}
		
		// variables
		TiXmlElement* elem = NULL;
		
		// get the name of the object
		elem = geomElem->FirstChildElement("file");
		if (elem){
			const char* file = elem->GetText();
			if (file){
				nrLog.Log(NR_LOG_APP, "CBaseObject::loadGeometry(): Load mesh file \"%s\"", file);
			
				try{	
					// Create & Load the entity
					mEntity 	= COgreTask::GetSingleton().mSceneMgr->createEntity(mName, std::string(file));
					mSceneNode 	= COgreTask::GetSingleton().mSceneMgr->getRootSceneNode()->createChildSceneNode();
					mSceneNode->attachObject( mEntity );
				}catch (...){
					nrLog.Log(NR_LOG_APP, "CBaseObject::loadGeometry(): An error occurs by loading of the geometry node");
					return true;
				}
			}
		}
		
		// check for scaling properties
		elem = geomElem->FirstChildElement("stretch");
		if (elem){
			const char* x = elem->Attribute("x");
			const char* y = elem->Attribute("y");
			const char* z = elem->Attribute("z");
			
			float fx = x ? boost::lexical_cast<float>(x) : 1.0f;
			float fy = y ? boost::lexical_cast<float>(y) : 1.0f;
			float fz = z ? boost::lexical_cast<float>(z) : 1.0f;
			
			mSceneNode->setScale(fx, fy, fz);
		}
		
		return false;
	}

	//--------------------------------------------------------------------------
	bool CBaseObject::importFromFile(const char* fileName, const std::string& xmlPath)
	{
		
		// load the xml document
		shared_ptr<TiXmlDocument> mDoc (new TiXmlDocument(fileName));
		if (!mDoc->LoadFile())
		{
			nrLog.Log(NR_LOG_APP, "CBaseObject::importFromFile(): Can not load the file \"%s\"", fileName);
			return true;	
		}

		// Get the root element from the file		
		TiXmlElement* rootElem = mDoc->FirstChildElement(this->getObjectType());
		
		// get the first root element.
		if (rootElem == NULL)
		{
			nrLog.Log(NR_LOG_APP, "CBaseObject::importFromFile(): Can not find root node \"%s\"", this->getObjectType());
			return true;
		}
				
		
		return parseSettings(rootElem, xmlPath);
	}
	
	//--------------------------------------------------------------------------
	bool CBaseObject::importFromString(const char* xmlSettings, const std::string& xmlPath)
	{
		
		// load the xml document
		shared_ptr<TiXmlDocument> mDoc (new TiXmlDocument());
		mDoc->Parse(xmlSettings);
		if (mDoc->Error())
		{
			nrLog.Log(NR_LOG_APP, "CBaseObject::importFromString(): Can not parse string. %s", mDoc->ErrorDesc());
			return true;
		}

		// Get the root element from the file		
		TiXmlElement* rootElem = mDoc->FirstChildElement(this->getObjectType());
		
		// get the first root element.
		if (rootElem == NULL)
		{
			nrLog.Log(NR_LOG_APP, "CBaseObject::importFromString(): Can not find root node \"%s\"", this->getObjectType());
			return true;
		}
				
		
		return parseSettings(rootElem, xmlPath);
	}

	//--------------------------------------------------------------------------
	bool CBaseObject::bindController(const char* name){
		
		// create an instance of controller object
		mControl = CBaseControl::createInstance(name);
		
		// is the interface not valid
		if (mControl == NULL)
			return false;

		return true;
	}

	//--------------------------------------------------------------------------
	void CBaseObject::setName(const char* name)
	{
		this->mName = name;
	}
	
	
	
	/**
	* Get event from queue
	*
	* @params none
	*
	* @return CEvent, the first item of the queue
	*/
	boost::shared_ptr<CEvent> CBaseObject::getEvent()
	{
		// Check if queue has elements
		if (this->m_eventQueue.empty() == false)
		{
			// Read first element from vector queue
			boost::shared_ptr<CEvent> firstElement = this->m_eventQueue.front();
	
			// Remove first element from queue
			this->m_eventQueue.pop();
		
			return firstElement;
		}
	}
	
	
	
	
	/**
	* Add event to queue
	*
	* @params CEvent Add new item to queue
	*
	* @return nothing
	*/
	void CBaseObject::addEvent(boost::shared_ptr<CEvent> element)
	{
		this->m_eventQueue.push(element);
		return;
	}
	
	
	
	
	/**
	* Process event from queue
	*
	* @params none
	*
	* @return 0: ok; -1 Error: No further elements in queue;
	*/
	int CBaseObject::process()
	{
		// If no further element @ queue
		if (this->m_eventQueue.empty() == true) return -1;
		
		// Get first element from queue
		boost::shared_ptr<CEvent> task = this->getEvent();
		
		// Check what todo
		// TODO: Implementierung der verschiedenen
		// Tasks, die durch das Objekt abgearbeitet
		// werden muessen!
		
		return true;
	}
	
};
