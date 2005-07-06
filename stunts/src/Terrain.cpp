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
#include "OgreTask.hpp"

#include <boost/shared_ptr.hpp>
#include <nrEngine/nrEngine.h>


using namespace boost;

namespace stunts
{

	//----------------------------------------------------------------------
	CTerrain::CTerrain(CLevel* level)
	{
		//mSceneMgr = COgreTask::GetSingleton().mSceneMgr;
		mLevel = level;
		mRaySceneQuery = boost::shared_ptr< Ogre::RaySceneQuery>
			(COgreTask::GetSingleton().mSceneMgr->createRayQuery(
				Ogre::Ray(Ogre::Vector3(0.0f, 0.0f, 0.0f),
				Ogre::Vector3::NEGATIVE_UNIT_Y))
            );

		mWidthX = 0.0f;
		mWidthZ = 0.0f;

		//reset physics members
		mFriction = 18.0f;

		// Init pointers
		mReflectCam = NULL;
		mWaterNode = NULL;
		mWaterPlane = NULL;
		mWaterPlaneEnt = NULL;
		mWaterHeight = 0;
		mWaterRttiTexUnit = NULL;
	}

	//----------------------------------------------------------------------
	CTerrain::~CTerrain()
	{
		// remove water surface
		if (mWaterNode)
		{
			mWaterNode->detachAllObjects();
			COgreTask::GetSingleton().mSceneMgr->getRootSceneNode()->removeAndDestroyChild(mWaterNode->getName());
		}

		if (mWaterPlaneEnt)
		{
			COgreTask::GetSingleton().mSceneMgr->removeEntity(mWaterPlaneEnt);
			mWaterPlaneEnt = NULL;
		}

		if (mWaterPlane)
		{
			delete mWaterPlane;
			mWaterPlane = NULL;
		}

		// remove render texture
		Ogre::RenderTarget* t = COgreTask::GetSingleton().mRenderer->getRenderTarget("Terrain_WaterRttTex");
		if (t)
		{
			t->removeAllListeners();
			t->removeAllViewports();
			COgreTask::GetSingleton().mRenderer->destroyRenderTexture("Terrain_WaterRttTex");
		}

		// remove reflection camera
		if (mReflectCam)
		{
			COgreTask::GetSingleton().mSceneMgr->removeCamera(mReflectCam);
			mReflectCam = NULL;
		}

		// now remove the texture unit from the resource, so we can load
		// the resource later again
		if (mWaterRttiTexUnit)
		{
			MaterialPtr mat = MaterialManager::getSingleton().getByName(mWaterMaterial);
			Ogre::Pass* pass = mat->getTechnique(0)->getPass(0);
			
			for (unsigned short i = 0; i < pass->getNumTextureUnitStates(); i++)
			{
				Ogre::TextureUnitState* t = pass->getTextureUnitState(i);
				if (t->getTextureName() == mWaterRttiTexUnit->getTextureName())
				{
					pass->removeTextureUnitState(i);
				}
			}
			
		}
		
		// remove terrain objects
		mRaySceneQuery.reset();
		
		mTerrain.reset();
		
	}

	//----------------------------------------------------------------------
	void CTerrain::Init(std::string terrainFile)
	{
		//collision functionality
		mRaySceneQuery = boost::shared_ptr< Ogre::RaySceneQuery>
			(COgreTask::GetSingleton().mSceneMgr->createRayQuery(
				Ogre::Ray(Ogre::Vector3(0.0f, 0.0f, 0.0f),
				Ogre::Vector3::NEGATIVE_UNIT_Y))
            );

		//OgreODE terrain
		mTerrain.reset(new OgreOde::TerrainGeometry(terrainFile, mLevel->
			PhysicsWorld()->getDefaultSpace()));


		mTerrain->setHeightListener(this);

		// Get some engine objects
		Ogre::SceneManager*	mSceneMgr = COgreTask::GetSingleton().mSceneMgr;
		Ogre::Camera*		mCamera = COgreTask::GetSingleton().mCamera;
		Ogre::RenderWindow*	mWindow = COgreTask::GetSingleton().mWindow;

		nrLog.Log(NR_LOG_APP, "CTerrain: Initialize water surface");
		
		try
		{

			// Create water surface
			mWaterPlane = new MovablePlane("Terrain_WaterReflectPlane");
			mWaterPlane->d = 0;
			mWaterPlane->normal = Vector3::UNIT_Y;
			MeshManager::getSingleton().createPlane("Terrain_WaterReflectionPlane",
				ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
				*mWaterPlane, 5000, 5000,
				1, 1, true, 1, 1, 1, Vector3::UNIT_Z);
			mWaterPlaneEnt = mSceneMgr->createEntity( "Terrain_WaterPlane", "Terrain_WaterReflectionPlane" );
			mWaterPlaneEnt->setCastShadows(false);

			// Attach to the scene manager
			mWaterNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
			
			// Attach both the plane entity, and the plane definition
			mWaterNode->attachObject(mWaterPlaneEnt);
			mWaterNode->attachObject(mWaterPlane);
			mWaterNode->translate(0, mWaterHeight, 0);
	
			// Create reflection on the water surface
			RenderTexture* rttTex = COgreTask::GetSingleton().mRenderer->createRenderTexture( "Terrain_WaterRttTex", 512, 512, TEX_TYPE_2D, PF_R8G8B8 );
			{

				mReflectCam = mSceneMgr->createCamera("Terrain_ReflectCam");
				mReflectCam->setNearClipDistance(mCamera->getNearClipDistance());
				mReflectCam->setFarClipDistance(mCamera->getFarClipDistance());
				mReflectCam->setAspectRatio(
					(Real)mWindow->getViewport(0)->getActualWidth() /
					(Real)mWindow->getViewport(0)->getActualHeight());
				
				Viewport *v = rttTex->addViewport( mReflectCam );
				v->setClearEveryFrame( true );
				v->setBackgroundColour( ColourValue::Black );

				// get the material for water surface
				MaterialPtr mat = MaterialManager::getSingleton().getByName(mWaterMaterial);
				mWaterRttiTexUnit = mat->getTechnique(0)->getPass(0)->createTextureUnitState("Terrain_WaterRttTex");
				if (mWaterRttiTexUnit == NULL)
				{
					nrLog.Log(NR_LOG_APP, "CTerrain: Check if the water reflection material file is correct");
					return;
				}
				
				// Blend with base texture
				mWaterRttiTexUnit->setProjectiveTexturing(true, mReflectCam);
				rttTex->addListener(this);

				// set up linked reflection
				mReflectCam->enableReflection(mWaterPlane);
				// Also clip
				mReflectCam->enableCustomNearClipPlane(mWaterPlane);
			}
			
			// Give the plane a texture
			mWaterPlaneEnt->setMaterialName(mWaterMaterial.c_str());
	
			nrLog.Log(NR_LOG_APP, "CTerrain: Water was initialized");
		
		}catch (Ogre::Exception& excp){
			nrLog.Log(NR_LOG_APP, "CTerrain: Water surface could not be initilized. %s",
					excp.getFullDescription().c_str());
		}

	}

	//----------------------------------------------------------------------
	void CTerrain::preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
	{
		// Hide plane
		mWaterPlaneEnt->setVisible(false);
		
		mCurrentShadowTechnique = COgreTask::GetSingleton().mSceneMgr->getShadowTechnique();

		if (!(bool)nrSettings.get("use_shadows_in_water"))
		{
			COgreTask::GetSingleton().mSceneMgr->setShadowTechnique(SHADOWTYPE_NONE);
		}
		
	}
	
	//----------------------------------------------------------------------
	void CTerrain::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
	{
		// Show plane
		mWaterPlaneEnt->setVisible(true);

		COgreTask::GetSingleton().mSceneMgr->setShadowTechnique(mCurrentShadowTechnique);		
	}

	//----------------------------------------------------------------------
	void CTerrain::update()
	{
		// if camera and water available
		if (mReflectCam && mWaterPlane)
		{
			// Make sure reflection camera is updated too
			mReflectCam->setOrientation(COgreTask::GetSingleton().mCamera->getOrientation());
			mReflectCam->setPosition(COgreTask::GetSingleton().mCamera->getPosition());
		}
		
	}
	
	//----------------------------------------------------------------------
	bool CTerrain::getHeight(Ogre::Vector3& pos)
	{

        // clamp to terrain
        static Ogre::Ray updateRay;
        updateRay.setOrigin(pos);

		// check in down direction
		updateRay.setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);
        mRaySceneQuery->setRay(updateRay);
        Ogre::RaySceneQueryResult& qryResult = mRaySceneQuery->execute();
        Ogre::RaySceneQueryResult::iterator i = qryResult.begin();
        if (i != qryResult.end() && i->worldFragment)
        {
        	pos.y = i->worldFragment->singleIntersection.y;
        	return true;
        }

		// now check in up direction
		updateRay.setDirection(Ogre::Vector3::UNIT_Y);
        mRaySceneQuery->setRay(updateRay);
        Ogre::RaySceneQueryResult& qryResultUp = mRaySceneQuery->execute();
        i = qryResultUp.begin();
        if (i != qryResultUp.end() && i->worldFragment)
        {
        	pos.y = i->worldFragment->singleIntersection.y;
        	return true;
        }

        return false;
	}


	//handle ODE height query
	Ogre::Real CTerrain::heightAt(const Ogre::Vector3& position)
	{
		Ogre::Vector3 newPosition = position;
		getHeight(newPosition);

		return newPosition.y;
	}


	//--------------------------------------------------------------------------
	float CTerrain::getWidthX()
	{
		return mWidthX;
	}
	//--------------------------------------------------------------------------
	float CTerrain::getWidthZ()
	{
		return mWidthZ;
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

		// read the size of the terrain out
		elem = rootElem->FirstChildElement("size");
		if (elem)
		{
			nrCDator<float> wx (mWidthX);
			nrCDator<float> wz (mWidthZ);

			wx = std::string(elem->Attribute("widthX"));
			wz = std::string(elem->Attribute("widthZ"));
		}

		// read the friction of the terrain out
		elem = rootElem->FirstChildElement("physics");
		if (elem)
		{
			nrCDator<float> friction(this->mFriction);

			friction = std::string(elem->Attribute("friction"));
		}

		// read water definition
		elem = rootElem->FirstChildElement("water");
		if (elem)
		{
			nrCDator<Ogre::Real> height(this->mWaterHeight);

			height = std::string(elem->Attribute("height"));
			mWaterMaterial = std::string(elem->Attribute("material"));

			//MaterialManager::getSingleton().load(mWaterMaterial, "General");
		}

		// read out the location of cfg file
		elem = rootElem->FirstChildElement("config");
		if (elem){
			const char* name = elem->GetText();
			if (name){
				std::string file = mPath + name;

				// Setup new terrain
				try {
					nrLog.Log(NR_LOG_APP, "CTerrain::importFromFile(): Use \"%s\" as terrain configuration file", file.c_str());
					COgreTask::GetSingleton().mSceneMgr -> setWorldGeometry(file);
					Init(file);
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


















