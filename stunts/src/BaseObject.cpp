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
#include "CarObject.hpp"
#include "External/tinyxml/tinyxml.h"

#include <boost/shared_ptr.hpp>
#include <nrEngine/nrEngine.h>

using boost::shared_ptr;

namespace stunts {
	
	/**
	* Constructor of BaseObject
	*
	* @param xmlSettingsString String with settings in XML format 
	*
	* @return nothing
	*/
	CBaseObject::CBaseObject(char* xmlSettingsString)
	{
		// Parse given XML settings string, save in attributes
		// TODO, when XML Format is known
	};
	
	
	
	
	/** 
	* Destruktor of BaseObject
	*
	* @param none
	*
	* @return nothing
	*
	*/
	CBaseObject::~CBaseObject()
	{
		// RemoveObject from memory
		// TODO
	};




	//--------------------------------------------------------------------------
	CBaseObject* CBaseObject::createInstance(std::string objType)
	{
		// check whenever we have valid parameter
		if (objType.length() == 0) return NULL;
			
		// check whenever we do support this kind of object type
		if (objType == CBaseObject::getObjectType())
			return new CBaseObject();
		else if (objType == CCarObject::getObjectType())
			return new CCarObject();
		else
		{
			nrLog.Log(NR_LOG_APP, "CBaseObject::createInstance(): Not supported kind of object type \"%s\"", objType.c_str());
			return NULL;
		}
		
	};





	//--------------------------------------------------------------------------
	bool CBaseObject::importFromFile(const char* fileName)
	{
		nrLog.Log(NR_LOG_APP, "CBaseObject::importFromFile(): Import object definition from file \"%s\"", fileName);
		
		// load the xml document
		shared_ptr<TiXmlDocument> mDoc (new TiXmlDocument(fileName));
		if (!mDoc->LoadFile())
		{
			nrLog.Log(NR_LOG_APP, "CBaseObject::importFromFile(): Can not load the file \"%s\"", fileName);
			return true;	
		}
		
		TiXmlElement* rootElem = mDoc->FirstChildElement(CBaseObject::getObjectType());
		TiXmlElement* elem = NULL;
		
		// get the first root element.
		if (rootElem == NULL)
		{
			nrLog.Log(NR_LOG_APP, "CBaseObject::importFromFile(): Can not find root node \"%s\"", CBaseObject::getObjectType());
			return true;
		}
		
		// get the name of the object
		elem = rootElem->FirstChildElement("name");
		if (elem && !elem->NoChildren()){
			TiXmlText* name = elem->FirstChild()->ToText();
			if (name){
				nrLog.Log(NR_LOG_APP, "CBaseObject::importFromFile(): Found object with name=\"%s\"", name->Value());
				setName(name->Value());
			}
		}
		
		
		return false;		
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
	CEvent CBaseObject::getEvent()
	{
		// Check if queue has elements
		if (this->m_eventQueue.empty() == false)
		{
			// Read first element from vector queue
			CEvent firstElement = this->m_eventQueue.front();
	
			// Remove first element from queue
			this->m_eventQueue.pop();
		
			return firstElement;
		};
	};
	
	
	
	
	/**
	* Add event to queue
	*
	* @params CEvent Add new item to queue
	*
	* @return nothing
	*/
	void CBaseObject::addEvent(CEvent element)
	{
		this->m_eventQueue.push(element);
		return;
	};
	
	
	
	
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
		CEvent task = this->getEvent();
		
		// Check what todo
		// TODO: Implementierung der verschiedenen
		// Tasks, die durch das Objekt abgearbeitet
		// werden müssen!
		
		return true;
	};
};
