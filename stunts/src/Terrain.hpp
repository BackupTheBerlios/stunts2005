/* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup,
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Christian Morbach <GameDevelopment@paratronic.de>
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


#ifndef _TERRAIN_H_
#define _TERRAIN_H_

//------------------------------------------------------------------------------
//--- predeclare this class
//------------------------------------------------------------------------------
namespace stunts
{
	class CTerrain;
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

#include "Level.hpp"

//------------------------------------------------------------------------------
//--- namespace stunts
//------------------------------------------------------------------------------
namespace stunts
{
	class CTerrain : public OgreOde::TerrainGeometryHeightListener
	{

	public:

		CTerrain(CLevel* level);

		virtual ~CTerrain();

		bool getHeight(Ogre::Vector3& position);

		Ogre::Real heightAt(const Ogre::Vector3& position);


		/**
		 * Import the terrain from a file.
		 * @param fileName Name of the file containing terrain data.
		 * @param rootNode Name of the root node in the file. If no such defined, so default will be used
		 * @return true if error occurs otherwise false
		 **/
		bool importFromFile(const char* fileName, const char* rootNode = "terrain");

		/**
		 * Return the width in x-direction of terrain in meters
		 **/
		float getWidthX();

		/**
		 * Return the width in z-direction of terrain in meters
		 **/
		float getWidthZ();

		/**
		 * Return the coulomb friction of the terrain
		*/
		inline float32 getFriction(){return mFriction;};

	protected:
		void Init(std::string terrainFile);

		boost::shared_ptr< Ogre::RaySceneQuery> mRaySceneQuery;

		//boost::shared_ptr< Ogre::SceneManager >	mSceneMgr;
		//boost::shared_ptr< CLevel > mLevel;
		CLevel*							mLevel;
		
		float mWidthX;
		float mWidthZ;


		// OgreODE
		boost::shared_ptr<OgreOde::TerrainGeometry> mTerrain;
		float32 mFriction;
	};
}

#endif //_TERRAIN_H_




