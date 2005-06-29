/* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup,
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Christian Morbach <GameDevelopment@paratronic.de>
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


#ifndef _LEVEL_HPP_
#define _LEVEL_HPP_


//------------------------------------------------------------------------------
//--- predeclare this class
//------------------------------------------------------------------------------

namespace stunts
{
	class CLevel;
}

//------------------------------------------------------------------------------
//--- includes
//------------------------------------------------------------------------------
#include <OGRE/Ogre.h>
#include <OGRE/OgreKeyEvent.h>
#include <OGRE/OgreEventListeners.h>
#include <OGRE/OgreStringConverter.h>
#include <OGRE/OgreException.h>

#include <nrEngine/nrEngine.h>

#include <OgreOde_Core/OgreOde_Core.h>
#include <OgreOde_Prefab/OgreOde_Prefab.h>

#include "External/tinyxml/tinyxml.h"

#include "OgreTask.hpp"
#include "UserInput.hpp"
#include "Terrain.hpp"
#include "BaseObject.hpp"
#include "Atmosphere.hpp"
#include "Waypoint.hpp"


//------------------------------------------------------------------------------
//--- namespace stunts
//------------------------------------------------------------------------------
namespace stunts
{
	class CLevel : public nrITask, public OgreOde::CollisionListener
	{
		public:
			//only to test here
			boost::shared_ptr<OgreOde_Prefab::Vehicle> mVehicle;




			CLevel();
			virtual ~CLevel();

			//! Register all used variables by the settings manager
			void registerVariables();

			//! Remove all used variables from the settings manager
			void deregisterVariables();

			/**
			 * Loads the level by given XML-String
			 * @param levelFile Filename of file containing XML-Data about the level
			 * @return 0 if no error occurs otherwise 1
			 **/
			bool	loadLevel(const std::string& levelFile);

			/**
			 * Get OgreTask
			 *
			 * @return smart pointer to OgreTask class
			 */
			boost::shared_ptr< COgreTask > 		OgreTask();

			/**
			 * Get UserInput
			 *
			 * @return smart pointer to UserInput class
			 */
			boost::shared_ptr< CUserInput > 	UserInput();


			/**
			 * Get Terrain
			 *
			 * @return smart pointer to Terrain class
			 */
			boost::shared_ptr< CTerrain >	Terrain();


			/**
			 * Get PhysicsExecution
			 *
			 * @return smart pointer to PhysicsExecution class
			 */
			boost::shared_ptr<OgreOde::Stepper> PhysicsExecution();


			/**
			 * Get PhysicsWorld
			 *
			 * @return smart pointer to PhysicsWorld class
			 */
			boost::shared_ptr<OgreOde::World> PhysicsWorld();


			/**
			 * Get Objects
			 *
			 * @return Objects
			 */
			std::vector<boost::shared_ptr<CBaseObject> >& Objects();
 

			/**
			 * Convert given GridUnits into Meters
			 * @param x,z 2D-Grid-Position.
			 * @return Vector containing the position vector. The returned vector has y set to 0
			 * 			so you have to reset the y to right position.
			 **/
			Ogre::Vector3 unitsToMeters(int32 x, int32 z);

			/**
			 * Convert given GridUnit into Meter
			 * @param x Grid unit
			 * @return Value in meters. Assume the terrain is quadratic
			 **/
			Ogre::Real unitToMeter(int32 x);

			/**
			 * Same as unitToMeter, but convert from float unit value, to float meter value.
			 * This can help to prevent roundings errors by converting the values
			 **/
			Ogre::Real unitToMeter(float32 x);

			/**
			 * Put all waypoints of all objects together
			 * @param
			 * @return true=success / false=no success
			 */
			bool buildWaypointPath();
			
			// Find nearest waypoint
			boost::shared_ptr< CWaypoint >  findNearestWaypoint(boost::shared_ptr<CWaypoint> waypointFrom, int ObjectId);

			// Get first waypoint
			boost::shared_ptr<CWaypoint> getFirstWaypoint();

			// Get next i Waypoint
			boost::shared_ptr<CWaypoint> getNextWaypoint(boost::shared_ptr<CWaypoint> waypoint, int nr);

		protected:
			virtual nrResult taskInit();
			virtual nrResult taskStart();
			virtual nrResult taskUpdate();
			virtual nrResult taskStop();
			virtual const char* taskGetName() {return "LevelTask";}

			/**
			 * get all tasks to initialize own member variables.
			 */
			void getEngineTasks();

			//! Parse the gravity value of the level file
			void readGravity(TiXmlElement* elem);

			//! Parse the gridsize value of the level file
			void readGridsize(TiXmlElement* elem);

			//! Parse the terrain node of the xml file
			void readTerrain(TiXmlElement* elem);

			//! Parse teh objects node of the level file
			void readObjects(TiXmlElement* elem);

			//! Parse the atmosphere node of the file
			void readAtmosphere(TiXmlElement* elem);

			//! import file containing data of the track
			void importTrackFile(const char* fileName, const char* root = NULL);

			//member variables
			boost::shared_ptr< COgreTask >			mOgreTask;
			boost::shared_ptr< CUserInput > 		mUserInput;

			boost::shared_ptr< Ogre::InputReader >	mInputDevice;
			boost::shared_ptr< CTerrain > 			mTerrain;
			boost::shared_ptr< CAtmosphere >		mAtmosphere;

			std::vector<boost::shared_ptr<CBaseObject> >	mObjects;

			float32						mGravity;
			std::string					mLevelFileName;
			std::string					mLevelFilePath;

			bool						mIsLoaded;
			bool						mShouldLoadLevel;

			int32 						mGridCountInX;
			int32 						mGridCountInZ;

			std::vector<boost::shared_ptr<CWaypoint> >	mWaypoints;

			//
			boost::shared_ptr<OgreOde::World> mPhysicsWorld;
			boost::shared_ptr<OgreOde::Stepper> mPhysicsExecution;

			// Debuggingfunctions for printing waypoints
			void drawWaypoint(Ogre::Vector3 pos, int i);
			void drawWaypoint(Ogre::Vector3 min, Ogre::Vector3 max, int i);


			// ODE
			bool collision(OgreOde::Contact* contact);

			void InitializeODE();
	};

};
// namespace stunts

#endif //_LEVEL_HPP_



