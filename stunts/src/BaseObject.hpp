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


#ifndef CBASEOBJECT_H
#define CBASEOBJECT_H

#include "BaseControl.hpp"
#include "Event.hpp"
#include <queue>

namespace stunts {

	class CBaseObject
	{
		public:
			/**
			* Constructor of BaseObject
			*
			* @param char* xmlSettingsString, String with settings in XML format
			*
			* @return nothing
			*/
			CBaseObject(char* xmlSettingsString);
			
	
	
			
			// Deconstructor
			virtual ~CBaseObject();
		
	
	
		
			/**
			* This will create an object by its type name. Each object used in the game is derived
			* from this class. So we provide here a function with which one you do not have to
			* know which class name you should use to create an object. Simply give me the name
			* of the object type and I will return you the interface.
			* @param objType Unique object type name
			* @return Interfacve of the object according to the given name or NULL if no such
			* object type is supported
			**/
			static CBaseObject*	createInstance(std::string objType);
	
	
	
	
			/**
			* Set the name for this object. Each object must have unique name.
			* @param name Unique name for the object
			**/
			void setName(const char* name);
	
	
				
				
			/**
			* Import the object declaration from a file.
			* The file contains XML-Data will be parsed and all properties that are
			* supported by this object will be readed in. in the BaseObject-Version we will read only
			* "\<object\>" nodes. All derived classes should overwrite this method to allow reading
			* of data for specified object.
			* @param fileName Name of the file containing object data
			* @return false if the reading was successfull
			**/
			virtual bool importFromFile(const char* fileName);
				bool importFromFile(const std::string fileName){ return importFromFile(fileName.c_str()); }
				
	
	
				
			/**
			* Create a new controller that does control this object. For human driven/interactive objects, like
			* cars, camera or something else, the appropriate controller will be used.
			* If the object is AI driven so controller with AI interface will be used to control the
			* object. You should specify here the name of the control for using. The given
			* controller name should be supported by controller class to allow creating interface from it.
			* If the name is not valid, so false will be returned.
			* @param controllerName Name of the controller
			* @return false if such controller could not be bound or if such one does not exists
			**/
			virtual bool bindController(const char* name);
			
	
	
			
			/**
			* Get the object type name.
			*/
			static const char* getObjectType() { return "base"; }
	
	
	
			
			// Functions to get attributes and attribute-references
			inline vec3	Position() const		{ return m_position; };
			inline vec3&	rPosition() 			{ return m_position; };
			inline vec3	Direction() const		{ return m_direction; };
			inline vec3&	rDirection() 			{ return m_direction; };
			
			inline float	Speed() const			{ return m_speed; };
			inline float&	rSpeed()			{ return m_speed; };
			inline vec3	SpeedVector() const		{ return m_speedVector; };
			inline vec3&	rSpeedVector() 			{ return m_speedVector; };
			
			inline float	Mass() const			{ return m_mass; };
			inline float& 	rMass() 			{ return m_mass; };
			inline vec3	MassPoint() const		{ return m_masspoint; };
			inline vec3&	rMassPoint() 			{ return m_masspoint; };
			
			inline float	FrictionCoefficient() const	{ return m_frictionCoefficient; };
			inline float& 	rFrictionCoefficient() 		{ return m_frictionCoefficient; };
			
			inline float	Torque() const			{ return m_torque; };
			inline float& 	rTorque() 			{ return m_torque; };
			inline vec3	TorqueAxis() const		{ return m_torqueAxis; };
			inline vec3&	TorqueAxis() 			{ return m_torqueAxis; };
			
	
	
			
			/**
			* Get event from queue
			*
			* @params none
			*
			* @return &CEvent, Reference on the first item of the queue
			*/
			CEvent	getEvent();
	
			
	
			
			/**
			* Add event to queue
			*
			* @params CEvent Add new item to queue
			*
			* @return nothing
			*/	
			void	addEvent(CEvent element);
			
	
	
			
			/**
			* Process event from queue
			*
			* @params none
			*
			* @return 0: ok, -1:Error: No further elements in queue, aso;
			*/
			int	process();
			
	
		protected:
			// TODO: These Classes must be included, then
			// the 2 lines can be uncommentated
	
			// CVectors of sound and geometry objects
			//vector<CSound>		m_sound;
			//vector<CGeometry>	m_geometry;
			
			// Position and direction of the object
			vec3 			m_position;
			vec3 			m_direction;
			
			// Speed value and speed vector
			float			m_speed;
			vec3			m_speedVector;
			
			// Mass value and masspoint vector
			float			m_mass;
			vec3			m_masspoint;
	
			// Friction Coefficient (dt: Reibungskoeffizient)		
			float			m_frictionCoefficient;
			
			// Torque value and torqueAxis vector
			float			m_torque;
			vec3			m_torqueAxis;
			
			// CVector of queue items
			std::queue<CEvent>	m_eventQueue;	
	
			//! String storing the name
			std::string		mName;
				
			//! Controller of this object
			CBaseControl*		mControl;
			
	};
};

#endif
