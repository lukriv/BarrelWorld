#include "gterrainmgr.h"

#include <OGRE/Terrain/OgreTerrain.h>
#include <OGRE/Terrain/OgreTerrainGroup.h>

#include <bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

#include "RenderComp/grendercmgr.h"
#include "PhysicsComp/gphyscmgr.h"


GameTerrainManager::GameTerrainManager(GameLogger* pLogger) : m_spLogger(pLogger)
															, m_pRenderMgr(nullptr)
															, m_pPhysicsMgr(nullptr)
															, m_globalTerrainOptions(nullptr)
															, m_pTerrainGroup(nullptr)
{}

GameTerrainManager::~GameTerrainManager()
{
	Uninitialize();
}

GameErrorCode GameTerrainManager::Initialize(RenderCompManager* pRenderMgr, PhysicsCompManager* pPhysMgr)
{
	m_pRenderMgr = pRenderMgr;
	m_pPhysicsMgr = pPhysMgr;
	
	m_gridOrigin[0] = 0;
	m_gridOrigin[1] = 0;
	
	if(m_pRenderMgr)
	{
		
		FWG_RETURN_FAIL(PrepareTerrainGlobalOptions());
	}
	
	
	
	return FWG_NO_ERROR;
	
}

void GameTerrainManager::Uninitialize()
{
	if(m_pTerrainGroup)
	{
		m_pTerrainGroup->removeAllTerrains();
		OGRE_DELETE m_pTerrainGroup;
		m_pTerrainGroup = nullptr;
	}
	
	if(m_globalTerrainOptions)
	{
		OGRE_DELETE m_globalTerrainOptions;
		m_globalTerrainOptions = nullptr;
	}
	
	if(!m_physTerrainGrid.empty()&&(m_pPhysicsMgr))
	{
		TPhysTerrainGrid::iterator iter;
		for( iter = m_physTerrainGrid.begin(); iter != m_physTerrainGrid.end(); ++iter)
		{
			if(iter->m_pRigidBody != nullptr)
			{
				m_pPhysicsMgr->GetDynamicsWorld()->removeRigidBody(iter->m_pRigidBody);
				delete iter->m_pRigidBody;
				iter->m_pRigidBody = nullptr;
			}
			
			if(iter->m_pTerrainData != nullptr)
			{
				delete[] iter->m_pTerrainData;
				iter->m_pTerrainData = nullptr;
			}
		}
		
		m_physTerrainGrid.clear();
	}
}

GameErrorCode GameTerrainManager::CreateTerrainGroup(TerrainDef& terrainDef)
{
	if(m_pTerrainGroup != nullptr)
	{
		// more then one terrain group is not allowed
		return FWG_E_OBJECT_ALREADY_EXISTS_ERROR;
	}
	
	if(m_pRenderMgr)
	{
		
    
		
		// create new terrain group
		m_pTerrainGroup = OGRE_NEW Ogre::TerrainGroup(m_pRenderMgr->GetOgreSceneManager()
													, Ogre::Terrain::ALIGN_X_Z
													, terrainDef.m_mapSize
													, terrainDef.m_worldSize);
		m_pTerrainGroup->setFilenameConvention("testTerrain", "dat");										
		m_pTerrainGroup->setOrigin(Ogre::Vector3::ZERO);
		
		//FWG_RETURN_FAIL(PrepareTerrainGlobalOptions());
		
		// configure terrain defaults
		Ogre::Terrain::ImportData& defaultimp = m_pTerrainGroup->getDefaultImportSettings();
		defaultimp.terrainSize = terrainDef.m_mapSize;
		defaultimp.worldSize = terrainDef.m_worldSize;
		defaultimp.inputScale = 1; // due terrain.png is 8 bpp
		defaultimp.minBatchSize = 33;
		defaultimp.maxBatchSize = 65;
		
		defaultimp.layerList.resize(1);
		defaultimp.layerList[0].worldSize = 100;
		defaultimp.layerList[0].textureNames.push_back("grass.png");
		defaultimp.layerList[0].textureNames.push_back("grassnormal.png");
		//defaultimp.layerList.resize(1);
		//defaultimp.layerList[0].worldSize = 100;
		//defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
		//defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
		//defaultimp.layerList[1].worldSize = 30;
		//defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
		//defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
		//defaultimp.layerList[2].worldSize = 200;
		//defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
		//defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
		
	}
	
	wxVector< RefObjSmPtr<TerrainPage> >::iterator iter;
	for (iter = terrainDef.m_terrainPages.begin(); iter != terrainDef.m_terrainPages.end(); ++iter)
	{
		TerrainPage *pTerrPage = (*iter).In();
		
		wxScopedArray<float> apTerrain;
		float minH = 0.0f;
		float maxH = 0.0f;
				
		FWG_RETURN_FAIL( LoadTerrainData(terrainDef.m_terrainPages[0]->m_filename
						, terrainDef.m_mapSize
						, apTerrain
						, minH
						, maxH) );
		
		if(m_pRenderMgr)
		{
		   // import settings
		   	//Ogre::Image img;
			//img.load(terrainDef.m_terrainPages[0]->m_filename.ToStdString(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

			//m_pTerrainGroup->defineTerrain(pTerrPage->m_pageX, pTerrPage->m_pageY, &img);
			
			//Ogre::Terrain::ImportData impData;
			//
			//
			//
			//impData.terrainSize = terrainDef.m_mapSize;
			//impData.worldSize = terrainDef.m_worldSize;
			//impData.inputScale = 1.0; // due terrain.png
			//impData.inputFloat = pTerrainData;
			
			
			m_pTerrainGroup->defineTerrain(pTerrPage->m_pageX, pTerrPage->m_pageY, apTerrain.get());
			
			
			//m_pTerrainGroup->getTerrain(pTerrPage->m_pageX, pTerrPage->m_pageY)->prepare(impData);
		}
		
		if(m_pPhysicsMgr)		
		{
			float* pTerrainData = new (std::nothrow) float[terrainDef.m_mapSize*terrainDef.m_mapSize];
			for (wxDword i = 0; i < terrainDef.m_mapSize; ++i)
			{
				memcpy( pTerrainData + terrainDef.m_mapSize * i
					, apTerrain.get() + terrainDef.m_mapSize*(terrainDef.m_mapSize-i-1)
					, sizeof(float)*(terrainDef.m_mapSize) );
			}
			
			
			
			
			btHeightfieldTerrainShape *pTerrainShape = new btHeightfieldTerrainShape(terrainDef.m_mapSize,
																				terrainDef.m_mapSize,
																				pTerrainData,
																				1,
																				minH,
																				maxH,
																				1,
																				PHY_FLOAT,
																				false);
			
			btDefaultMotionState *state = new btDefaultMotionState();
			
			btScalar mass = 0;
			btVector3 localInertia(0, 0, 0);
			
			pTerrainShape->calculateLocalInertia(mass, localInertia);
			
			btRigidBody::btRigidBodyConstructionInfo info(mass, state, pTerrainShape, localInertia);
			
			btRigidBody *body = new btRigidBody(info);
			
			body->getWorldTransform().setOrigin(btVector3(0, maxH - minH,0));
			
			PhysicsTerrainData terrData;
			terrData.m_pRigidBody = body;
			terrData.m_pTerrainData = pTerrainData;
			
			m_physTerrainGrid.push_back(terrData);
			m_pPhysicsMgr->GetDynamicsWorld()->addRigidBody(body);
		}
		
		//delete[] pTerrainData;
		
	}
	
	if(m_pTerrainGroup)
	{
		m_pTerrainGroup->loadAllTerrains(true);
		m_pTerrainGroup->freeTemporaryResources();
	}
	
	//{
	//	Ogre::Terrain *pTerrain = m_pTerrainGroup->getTerrain( 0, 0);
	//	wxDword maxSize = pTerrain->getSize() * pTerrain->getSize();
	//	float * pTerrData = pTerrain->getHeightData();
	//	
	//	// initialize min and max
	//	float minHeight = 100000; 
	//	float maxHeight = -100000;
	//	
	//	FWGLOG_DEBUG_FORMAT(wxT("Terrain size = %u"), m_spLogger, pTerrain->getSize(), FWGLOG_ENDVAL);
	//	
	//	for (wxDword i = 0; i < maxSize ; ++i)
	//	{
	//		if (minHeight > pTerrData[i])
	//		{
	//			minHeight = pTerrData[i];
	//		}
	//		
	//		if (maxHeight < pTerrData[i])
	//		{
	//			maxHeight = pTerrData[i];
	//		}
	//		FWGLOG_DEBUG_FORMAT(wxT("TerrainData[%u] = %.4f"), m_spLogger, i, pTerrData[i], FWGLOG_ENDVAL);
	//	}
	//	
	//	FWGLOG_DEBUG_FORMAT(wxT("TerrainData min = %.4f, max = %.4f"), m_spLogger, minHeight, maxHeight, FWGLOG_ENDVAL);
	//}
	
	
	
	return FWG_NO_ERROR;
}


GameErrorCode GameTerrainManager::LoadTerrainData( const wxString& resource, wxDword terrDataSize, wxScopedArray<float> &retData, float &minHeight, float &maxHeight)
{
	Ogre::Image img;
	img.load(resource.ToStdString(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	
	if (( img.getHeight() != terrDataSize) || (img.getWidth() != terrDataSize))
	{
		FWGLOG_ERROR_FORMAT(wxT("Invalid image size: %u x %u (expected %u x %u)"), m_spLogger
				, img.getHeight()
				, img.getWidth()
				, terrDataSize
				, terrDataSize
				, FWGLOG_ENDVAL);
		return FWG_E_INVALID_DATA_ERROR;
	} else {
		FWGLOG_DEBUG_FORMAT(wxT("Image size: %u x %u"), m_spLogger
				, img.getHeight()
				, img.getWidth()
				, FWGLOG_ENDVAL);
	}
	
	Ogre::uchar *pImgData = img.getData();
	
	wxScopedArray<float> apFloat(new (std::nothrow) float[terrDataSize*terrDataSize]);
	
	FWGLOG_DEBUG_FORMAT(wxT("Image size = %u, dataSize = %u"), m_spLogger, img.getSize(), (terrDataSize*terrDataSize), FWGLOG_ENDVAL);
	
	// initialize min and max
	minHeight = 100000; 
	maxHeight = -100000;
	
	if(apFloat.get() == nullptr)
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	
	for (wxDword i = 0; i < img.getSize() ; ++i)
	{
		float temp = (static_cast<float>(pImgData[i])/255.0f) * 100.0f;
		apFloat[i] = temp;
		if (minHeight > temp)
		{
			minHeight = temp;
		}
		
		if (maxHeight < temp)
		{
			maxHeight = temp;
		}
		
		//FWGLOG_DEBUG_FORMAT(wxT("TerrainData[%u] = %.4f, imgData[%u] = %u"), m_spLogger, i, pFloat[i], i, pImgData[i], FWGLOG_ENDVAL);
	}
	
	retData.swap(apFloat);
	
	FWGLOG_DEBUG_FORMAT(wxT("TerrainData min = %.4f, max = %.4f"), m_spLogger, minHeight, maxHeight, FWGLOG_ENDVAL);
	
	return FWG_NO_ERROR;
}

GameErrorCode GameTerrainManager::PrepareTerrainGlobalOptions()
{
	
	m_globalTerrainOptions = OGRE_NEW Ogre::TerrainGlobalOptions();
	
	
	Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
	lightdir.normalise();
    
    
	Ogre::Light* l = m_pRenderMgr->GetOgreSceneManager()->createLight("tstLight");
	l->setType(Ogre::Light::LT_DIRECTIONAL);
	l->setDirection(lightdir);
	l->setDiffuseColour(Ogre::ColourValue::White);
	l->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));
	
	
	m_globalTerrainOptions->setMaxPixelError(8);
	
	// lightmapped terrain distance
	m_globalTerrainOptions->setCompositeMapDistance(3000);
	
	// set ambient color
	m_globalTerrainOptions->setLightMapDirection(l->getDerivedDirection());
	m_globalTerrainOptions->setCompositeMapAmbient(m_pRenderMgr->GetOgreSceneManager()->getAmbientLight());
	m_globalTerrainOptions->setCompositeMapDiffuse(l->getDiffuseColour());
	
	return FWG_NO_ERROR;

}
