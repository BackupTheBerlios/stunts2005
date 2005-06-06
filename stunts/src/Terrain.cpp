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

#include "Terrain.hpp"
#include "Utils.hpp"

#include <boost/shared_ptr.hpp>
#include <nrEngine/nrEngine.h>

using namespace boost;

namespace stunts
{

	CTerrain::CTerrain(boost::shared_ptr< Ogre::SceneManager > sceneMgr)
	{

		mSceneMgr = sceneMgr;

		mRaySceneQuery = boost::shared_ptr< Ogre::RaySceneQuery>
			(mSceneMgr->createRayQuery(
				Ogre::Ray(Ogre::Vector3(0.0f, 0.0f, 0.0f),
				Ogre::Vector3::NEGATIVE_UNIT_Y))
            );
	}

	CTerrain::~CTerrain()
	{
	}

	void CTerrain::Init()
	{

	}


	bool CTerrain::getHeight(Ogre::Vector3& pos)
	{
        // clamp to terrain
        static Ogre::Ray updateRay;
        updateRay.setOrigin(pos);
        updateRay.setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);
        mRaySceneQuery->setRay(updateRay);
        Ogre::RaySceneQueryResult& qryResult = mRaySceneQuery->execute();
        Ogre::RaySceneQueryResult::iterator i = qryResult.begin();
        if (i != qryResult.end() && i->worldFragment)
        {
        	pos.y = i->worldFragment->singleIntersection.y;
        	return true;
        }

        return false;
	}

	//--------------------------------------------------------------------------
	bool CTerrain::importFromFile(const char* fileName, const char* rootNode){

		nrLog.Log(NR_LOG_APP, "CTerrain::importFromFile(): Start loading of a terrain from file \"%s\"", fileName);

		// we open the file for parsing.
		shared_ptr<TiXmlDocument> mLevelDoc (new TiXmlDocument(fileName));
		if (!mLevelDoc->LoadFile())
		{
			nrLog.Log(NR_LOG_APP, "CTerrain::importFromFile(): There is an error occurs by loading of terrain specification file \"%s\"", fileName);
			return true;
		}

		// Load elements form the level file and handle with them in according way
		TiXmlElement* elem = NULL;
		TiXmlElement* rootElem;
		if (rootNode == NULL)
			rootElem = mLevelDoc->FirstChildElement("terrain");
		else
			rootElem = mLevelDoc->FirstChildElement(rootNode);

		if (!rootElem){
			nrLog.Log(NR_LOG_APP, "CTerrain::importFromFile(): The atmosphere file is corrupted");
			return true;
		}

		// get path containing this file
		std::string mPath = getPathFromFileName(fileName);

		// read out the location of cfg file
		elem = rootElem->FirstChildElement("config");
		if (elem){
			TiXmlText* name = elem->FirstChild()->ToText();
			if (name){
				std::string file = mPath + name->Value();

				// Setup new terrain
				try {
					nrLog.Log(NR_LOG_APP, "CTerrain::importFromFile(): Use \"%s\" as terrain configuration file", file.c_str());
					mSceneMgr -> setWorldGeometry(file);
					Init();
				}catch(...){
					nrLog.Log(NR_LOG_APP, "CTerrain::importFromFile(): \"%s\" file was not found", file.c_str());
					return true;
				}
			}

		}else{
			nrLog.Log(NR_LOG_APP, "CTerrain::importFromFile(): No Config file definition found !!!");
			return true;
		}

		return false;
	}


}	//namespace stunts


















