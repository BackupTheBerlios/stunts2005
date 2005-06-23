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


#ifndef _C_BASEOBJECT_H
#define _C_BASEOBJECT_H

//------------------------------------------------------------------------------
//--- predeclare this class
//------------------------------------------------------------------------------

namespace stunts 
{
	class CBaseObject;
}


#include "BaseControl.hpp"
#include "Event.hpp"
#include "Level.hpp"

#include <queue>
#include <vector>

#include <OGRE/Ogre.h>

#include "External/tinyxml/tinyxml.h"

using namespace Ogre;

namespace stunts {

	class CBaseObject
	{
		public:
			//! The level is a friend, because it can load the objects and so it needs full access to the object
			friend class CLevel;
			
			/**
			 * Default constructor
			 **/
			CBaseObject();
			
			/**
			* Constructor of BaseObject
			*
			* @param char* xmlSettingsString, String with settings in XML format
			* @param xmlPath path where the XML file was found (needed for import tags)
			* 
			* @return nothing
			*/
			CBaseObject(char* xmlSettingsString, const std::string& xmlPath);
							
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
			* @param xmlPath path where the XML file was found (needed for import tags)
			* @return false if the reading was successfull
			**/
			virtual bool importFromFile(const char* fileName, const std::string& xmlPath);
			bool importFromFile(const std::string fileName, const std::string& xmlPath){ return importFromFile(fileName.c_str(), xmlPath); }
			
	
			/**
			 * Import the object declaration from a file.
			 * The file contains XML-Data will be parsed and all properties that are
			 * supported by this object will be readed in. in the BaseObject-Version we will read only
			 * "\<object\>" nodes. All derived classes should overwrite this method to allow reading
			 * of data for specified object.
			 * @param fileName Name of the file containing object data
			 * @param xmlPath path where the XML file was found (needed for import tags)
			 * @return false if the reading was successfull
			 **/
			virtual bool importFromWaypointFile(const char* fileName, const std::string& xmlPath);
			bool importFromWaypointFile(const std::string fileName, const std::string& xmlPath){ return importFromWaypointFile(fileName.c_str(), xmlPath); }
			
			
	
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
					
					
			bool parseWaypoints(TiXmlElement* node);
			
			
			/**
			 * To return one waypoints, coordinates ablosute given
			 *
			 * @param none
			 *
			 * @return Vector3
			 */
			Vector3 getWaypoint();
			
			
			/**
			 * Has waypoints?
			 */
			bool hasWaypoints();
			
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
			
	
			static	const char* getObjectTypeSt() 	{ return "base"; }
			virtual const char* getObjectType() 	{ return CBaseObject::getObjectTypeSt(); }
			
			// Functions to get attributes
			inline Vector3	Position() const		{ return m_position; };
			inline Quaternion	Orientation() const	{ return m_orientation; };
			
			inline float	Speed() const			{ return m_speed; };
			inline Vector3	SpeedVector() const		{ return m_speedVector; };
			
			inline float	Mass() const			{ return m_mass; };
			inline Vector3	MassPoint() const		{ return m_masspoint; };
			
			inline float	FrictionCoefficient() const	{ return m_frictionCoefficient; };
			
			inline float	Torque() const			{ return m_torque; };
			inline Vector3	TorqueAxis() const		{ return m_torqueAxis; };
			
			// Functions to set object attributes
			void	setPosition		(Vector3 pos);
			void	setOrientation	(Quaternion orient);
			void	setMass			(float mass);
			void	setMassPoint	(Vector3 massPoint);
			void	setFriction		(float coeff);
			
			/**
			* Get event from queue
			*
			* @params none
			*
			* @return CEvent, Instance of the first item of the queue
			*/
			boost::shared_ptr<CEvent>	getEvent();
						
			/**
			* Add event to queue
			*
			* @params CEvent Add new item to queue
			*
			* @return nothing
			*/	
			void	addEvent(boost::shared_ptr<CEvent> element);
			
			/**
			* Process event from queue
			*
			* @params none
			*
			* @return 0: ok, -1:Error: No further elements in queue, aso;
			*/
			int	process();
			
			
		protected:
			//------------------ Methods --------------------------------------
			
			/**
			 * Parse settings of the object by defining the root node of an XML-Tree.
			 * Using here the tinyxml-parser.
			 * @param rootElem Pointer to the tinyxml-Element node
			 * @param xmlPath path where the XML file was found (needed for import tags)
			 * @return false if reading was successful
			 **/	
			virtual bool parseSettings(TiXmlElement* rootElem, const std::string& xmlPath);
			
			/**
			 * This will load the geometry node from the settings.
			 * The geometry node does contain information, about the used .mesh
			 * file and also some transformation information (e.g. stretching)
			 * @param geomElem Pointer to the tinyxml-Element containing geometry node from the XML-tree
			 * @param xmlPath path where the XML file was found (needed for import tags)
			 * @return true if an error occurs
			 **/
			virtual bool loadGeometry(TiXmlElement* geomElem, const std::string& xmlPath);

			/**
			 * This method calculates the difference between object's local origin and global
			 * position. After that this method will translate the object's scene node according
			 * to calculated value, so that the origin of the object lies int the center of all
			 * local object's axis. This method must be called after the geometry is loaded.
			 * Otehrwise the object's transforming behavior is not intuitive
			 **/
			void correctObjectsOrigin();
			
			/**
			 * Scale the object according to the given proprtion value and the axis.
			 * First we get the bounding box of the object (mesh must be already loaded).
			 * Then we scale the object so that it's given axis does have th elength 1m.
			 * Then we scale the object up to the given proportion value. This can be defined
			 * in gridUnits, so it will be converted to meters before scaling
			 * @param axis Char representing the axis ('x', 'y', 'z')
			 * @param value Proportion value
			 * @param useGrid if true the given value will be interpreted as grid unit
			 **/
			void scaleObjectProportionaly(char axis, float32 value, bool useGrid);
			
			//------------------ Variables --------------------------------------
			Ogre::Entity*		mEntity;
			Ogre::SceneNode*	mObjNode;
			
			// TODO: These Classes must be included, then
			// the 2 lines can be uncommentated
	
			// CVectors of sound and geometry objects
			//vector<CSound>	m_sound;
			//vector<CGeometry>	m_geometry;
			
			// Position and direction of the object
			Vector3 		m_position;
			Quaternion		m_orientation;
			
			// Speed value and speed vector
			float			m_speed;
			Vector3			m_speedVector;
			
			// Mass value and masspoint vector
			float			m_mass;
			Vector3			m_masspoint;
	
			// Friction Coefficient (dt: Reibungskoeffizient)		
			float			m_frictionCoefficient;
			
			// Torque value and torqueAxis vector
			float			m_torque;
			Vector3			m_torqueAxis;
			
			// Scale factor
			Vector3			m_scale;
			
			// CVector of queue items
			std::queue<boost::shared_ptr<CEvent> >	m_eventQueue;	
	
			//! String storing the name
			std::string		mName;
				
			//! Controller of this object
			CBaseControl*		mControl;
			
			//! Parent level for the objects
			CLevel*			mLevel;
			
			// Vector for storing waypoints
			std::vector<Vector3>		m_waypoints;
			
	};
};

#endif
