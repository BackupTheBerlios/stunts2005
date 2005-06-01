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

namespace stunts
{
	CTerrain::CTerrain(boost::shared_ptr< CLevel > level)
	{
		mLevel = level;

		if (!(mLevel->OgreTask()))
		{
			nrLog.Log(NR_LOG_APP, "CTerrain::CTerrain(): Error in getting\
				OgreTask");

			#ifdef ENABLE_DEBUG_MESSAGES
				std::cout << "CTerrain::CTerrain(): Error in getting\
				OgreTask" << std::endl;
			#endif

			nrKernel.KillAllTasks();
		}
		else
		{
			mCamera = mLevel->OgreTask()->mCamera;
			mSceneMgr = mLevel->OgreTask()->mSceneMgr;
		}

		mRaySceneQuery = boost::shared_ptr< Ogre::RaySceneQuery>
			(mSceneMgr->createRayQuery(
              Ogre::Ray(mCamera->getPosition(), Ogre::Vector3::NEGATIVE_UNIT_Y))
            );
	}

	CTerrain::~CTerrain()
	{
	}

	bool CTerrain::getHeight(Ogre::Vector3& pos)
	{
        // clamp to terrain
        static Ogre::Ray updateRay;
        updateRay.setOrigin(mCamera->getPosition());
        updateRay.setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);
        mRaySceneQuery->setRay(updateRay);
        Ogre::RaySceneQueryResult& qryResult = mRaySceneQuery->execute();
        Ogre::RaySceneQueryResult::iterator i = qryResult.begin();
        if (i != qryResult.end() && i->worldFragment)
        {
            //SceneQuery::WorldFragment* wf = i->worldFragment;
            mCamera->setPosition(mCamera->getPosition().x,
                i->worldFragment->singleIntersection.y + 10,
                mCamera->getPosition().z);
        }

        return true;
	}

}	//namespace stunts


















