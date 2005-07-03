/* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup,
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Art Tevs <tevs@mpi-sb.mpg.de>
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

#include "Atmosphere.hpp"
#include "OgreTask.hpp"
#include <nrEngine/nrEngine.h>

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

namespace stunts
{
	//--------------------------------------------------------------------------
	CAtmosphere::CAtmosphere()
	{
		mSunBillboardSet = 0;
		mSunBillboard = 0;
	}

	//--------------------------------------------------------------------------
	CAtmosphere::~CAtmosphere()
	{
		
		// delete all nodes
		for (unsigned int i=0; i < mLightNode.size(); i++)
			if (mLightNode[i])
			{
				mLightNode[i]->detachAllObjects();				
				COgreTask::GetSingleton().mSceneMgr->getRootSceneNode()->removeAndDestroyChild(mLightNode[i]->getName());
				
				mLightNode[i] = NULL;
			}
		
		mLightNode.clear();
		
		// now delete all lights
		for (unsigned int i=0; i < mLights.size(); i++)
			if (mLights[i])
			{
				COgreTask::GetSingleton().mSceneMgr->removeLight(mLights[i]);
				mLights[i] = NULL;
			}
			
		mLights.clear();
		
			
		// delete the billboards
		if (mSunBillboardSet)
		{
			mSunBillboardSet->clear();
			COgreTask::GetSingleton().mSceneMgr->removeBillboardSet(mSunBillboardSet);
		}
		
	}

	
	//--------------------------------------------------------------------------
	bool CAtmosphere::parseSettings(TiXmlElement* rootElem, const std::string& xmlPath)
	{
		nrLog.Log(NR_LOG_APP, "CAtmosphere::parseSettings(): Start parsing the settings");

		TiXmlElement* elem = NULL;
		
		// read Sky Box definitions
		elem = rootElem->FirstChildElement("skybox");
		if (elem)
		{
			const char* matname = elem->Attribute("material");
			const char* dists = elem->Attribute("distance");
			
			float dist = dists ? lexical_cast<float>(dists) : 5000.0f;
			try {
				COgreTask::GetSingleton().mSceneMgr->setSkyBox(true, matname, dist);
			}catch (...){
				nrLog.Log(NR_LOG_APP, "CAtmosphere::parseSettings(): Cannot setup the skybox %s", matname);
			}
		}
		
		
		// Read lights
		elem = rootElem->FirstChildElement("lights");
		if (elem)
			readLights(elem);

		// read Ambient light settings
		elem = rootElem->FirstChildElement("ambient");
		if (elem)
		{
			const char* x = elem->Attribute("r");
			const char* y = elem->Attribute("g");
			const char* z = elem->Attribute("b");

			float fx = x ? lexical_cast<float>(x) : 1.0f;
			float fy = y ? lexical_cast<float>(y) : 1.0f;
			float fz = z ? lexical_cast<float>(z) : 1.0f;

			COgreTask::GetSingleton().mSceneMgr->setAmbientLight(ColourValue(fx,fy,fz));
		}
			
		nrLog.Log(NR_LOG_APP, "CAtmosphere::parseSettings(): Stop parsing the settings");
		return false;
	}
	
	//--------------------------------------------------------------------------
	void CAtmosphere::readLights(TiXmlElement* elem)
	{
		if (elem == NULL) return;
		
		// Scan for all elements in this node
		for (TiXmlElement* smElem = elem->FirstChildElement("light"); smElem != 0; 
					smElem = smElem->NextSiblingElement("light"))
        {
			try{
				TiXmlElement* sElem = NULL;
				
				// read the name of the light and create the light
				sElem = smElem->FirstChildElement("name");
				const char* namestr = sElem->GetText();
				Light* light = COgreTask::GetSingleton().mSceneMgr->createLight(namestr);
				
				
				// read the type of the light
				const char* typestr = smElem->Attribute("type");
				Light::LightTypes	type = Light::LT_POINT;
				if (!strcasecmp(typestr, "directional")) type = Light::LT_DIRECTIONAL;
				if (!strcasecmp(typestr, "spot")) type = Light::LT_SPOTLIGHT;
				light->setType(type);
				
				// read the direction
				sElem = smElem->FirstChildElement("direction");
				if (sElem)
				{
					const char* x = sElem->Attribute("x");
					const char* y = sElem->Attribute("y");
					const char* z = sElem->Attribute("z");
					
					float fx = x ? lexical_cast<float>(x) : 1.0f;
					float fy = y ? lexical_cast<float>(y) : 1.0f;
					float fz = z ? lexical_cast<float>(z) : 1.0f;
					
					light->setDirection(fx, fy, fz);
				}
				
				// read position
				Vector3 position (0,0,0);
				sElem = smElem->FirstChildElement("pos");
				if (sElem)
				{
					const char* x = sElem->Attribute("x");
					const char* y = sElem->Attribute("y");
					const char* z = sElem->Attribute("z");
					
					float fx = x ? lexical_cast<float>(x) : 1.0f;
					float fy = y ? lexical_cast<float>(y) : 1.0f;
					float fz = z ? lexical_cast<float>(z) : 1.0f;
					
					light->setPosition(fx, fy, fz);
					
					position.x = fx;
					position.y = fy;
					position.z = fz;
				}
				
				// read diffuse
				ColourValue diffuse(1,1,1,1);
				sElem = smElem->FirstChildElement("diffuse");
				if (sElem)
				{
					const char* x = sElem->Attribute("r");
					const char* y = sElem->Attribute("g");
					const char* z = sElem->Attribute("b");
					
					float fx = x ? lexical_cast<float>(x) : 1.0f;
					float fy = y ? lexical_cast<float>(y) : 1.0f;
					float fz = z ? lexical_cast<float>(z) : 1.0f;
					
					light->setDiffuseColour(fx, fy, fz);
					
					ColourValue d(fx,fy,fz,1.0f);
					diffuse = d;
				}
				
				// read specular
				sElem = smElem->FirstChildElement("specular");
				if (sElem)
				{
					const char* x = sElem->Attribute("r");
					const char* y = sElem->Attribute("g");
					const char* z = sElem->Attribute("b");
					
					float fx = x ? lexical_cast<float>(x) : 1.0f;
					float fy = y ? lexical_cast<float>(y) : 1.0f;
					float fz = z ? lexical_cast<float>(z) : 1.0f;
					
					light->setSpecularColour(fx, fy, fz);
				}
				
				// read attenuation
				sElem = smElem->FirstChildElement("attenuation");
				if (sElem)
				{
				
					const char* x = sElem->Attribute("range");
					const char* y = sElem->Attribute("constant");
					const char* z = sElem->Attribute("linear");
					const char* w = sElem->Attribute("quadratic");
					
					float fx = x ? lexical_cast<float>(x) : 1.0f;
					float fy = y ? lexical_cast<float>(y) : 1.0f;
					float fz = z ? lexical_cast<float>(z) : 1.0f;
					float fw = w ? lexical_cast<float>(w) : 1.0f;
					
					light->setAttenuation(fx, fy, fz, fw);
				}
				
				// read spot light properties
				// <spot inner="10.0" outer="100.0" falloff="1.0" />
				sElem = smElem->FirstChildElement("spot");
				if (sElem && type == Light::LT_SPOTLIGHT)
				{
				
					const char* x = sElem->Attribute("inner");
					const char* y = sElem->Attribute("outer");
					const char* z = sElem->Attribute("falloff");
					
					float fx = x ? lexical_cast<float>(x) : 1.0f;
					float fy = y ? lexical_cast<float>(y) : 1.0f;
					float fz = z ? lexical_cast<float>(z) : 1.0f;
					
					light->setSpotlightRange(Radian(Degree(Real(fx))),
											Radian(Degree(Real(fy))),
											fz);
				}

				// checkj whenever light can cast shadows
				sElem = smElem->FirstChildElement("shadow");
				if (sElem)
				{
					const char* x = sElem->Attribute("cause");

					bool can = x ? (bool)lexical_cast<int>(x) : false;

					light->setCastShadows(can);
				}

				
				// read the billboard value
				sElem = smElem->FirstChildElement("billboard");
				if (sElem)
				{
					const char* matname = sElem->Attribute("material");
					if (matname)
					{
						// first create a node containing the light source
						SceneNode* mLightNode = COgreTask::GetSingleton().mSceneMgr->getRootSceneNode()->createChildSceneNode(std::string(namestr) + "_node");
						mLightNode->attachObject(light);
						mLightNode->setPosition(position);
						
						// create billboard set
						mSunBillboardSet = COgreTask::GetSingleton().mSceneMgr->createBillboardSet(std::string(namestr) + "_billboards", 1);
						mSunBillboardSet->setMaterialName(matname);
						mSunBillboard = mSunBillboardSet->createBillboard(0.0f,0.0f,0.0f, diffuse);

						// get size of teh billboard
						const char* width = sElem->Attribute("width");
						const char* height = sElem->Attribute("height");

						float widthf = width ? lexical_cast<float>(width) : 1.0f;
						float heightf = height ? lexical_cast<float>(height) : 1.0f;

						mSunBillboard->setDimensions(widthf, heightf);
						
						// attach
						mLightNode->attachObject(mSunBillboardSet);
						
						// Now copy the node to the list of light nodes
						this->mLightNode.push_back(mLightNode);
					}
				}
				
				// store the light
				mLights.push_back(light);
				
			} catch(...){
				nrLog.Log(NR_LOG_APP, "CAtmosphere::readLights(): Can not create/setup light");
			}
		}
		
	}
	
		
	//--------------------------------------------------------------------------
	bool CAtmosphere::importFromFile(const char* fileName, const std::string& xmlPath){
				
		// we open the file for parsing.
		// Later we can open the file through the virtual file system if we had got more time 
		// for development
		shared_ptr<TiXmlDocument> mLevelDoc (new TiXmlDocument(fileName));
		if (!mLevelDoc->LoadFile())
		{
			nrLog.Log(NR_LOG_APP, "CAtmosphere::importFromFile(): There is an error occurs by loading atmosphere specification file \"%s\"", fileName);
			return true;
		}

		// Load elements form the level file and handle with them in according way		
		TiXmlElement* elem = NULL;
		TiXmlElement* rootElem;
		rootElem = mLevelDoc->FirstChildElement("atmosphere");
		
		if (!rootElem)
		{
			nrLog.Log(NR_LOG_APP, "CAtmosphere::importFromFile(): The atmosphere file is corrupted - No <atmosphere> Tag was found !");
			return true;
		}
		
		return parseSettings(rootElem, xmlPath);
	}

	//--------------------------------------------------------------------------
	bool CAtmosphere::importFromString(const char* xmlSettings, const std::string& xmlPath)
	{
		// load the xml document
		shared_ptr<TiXmlDocument> mDoc (new TiXmlDocument());
		mDoc->Parse(xmlSettings);
		if (mDoc->Error())
		{
			nrLog.Log(NR_LOG_APP, "CAtmosphere::importFromString(): Can not parse string. %s", mDoc->ErrorDesc());
			return true;
		}

		// Get the root element from the file		
		TiXmlElement* rootElem = mDoc->FirstChildElement("atmosphere");
		
		if (!rootElem)
		{
			nrLog.Log(NR_LOG_APP, "CAtmosphere::importFromString(): The atmosphere file is corrupted - No <atmosphere> Tag was found !");
			return true;
		}
				
		
		return parseSettings(rootElem, xmlPath);
	}
			
}	//namespace stunts


















