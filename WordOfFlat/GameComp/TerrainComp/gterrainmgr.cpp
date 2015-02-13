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
		m_globalTerrainOptions = OGRE_NEW Ogre::TerrainGlobalOptions();
		
		
		//Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
		//lightdir.normalise();
        //
		//Ogre::Light* light = mSceneMgr->createLight("tstLight");
		//light->setType(Ogre::Light::LT_DIRECTIONAL);
		//light->setDirection(lightdir);
		//light->setDiffuseColour(Ogre::ColourValue::White);
		//light->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));
		
		
		m_globalTerrainOptions->setMaxPixelError(8);
		
		// lightmapped terrain distance
		m_globalTerrainOptions->setCompositeMapDistance(3000);
		
		// set ambient color
		m_globalTerrainOptions->setCompositeMapAmbient(m_pRenderMgr->GetOgreSceneManager()->getAmbientLight());
		
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
			if(*iter != nullptr)
			{
				m_pPhysicsMgr->GetDynamicsWorld()->removeRigidBody(*iter);
				delete *iter;
				*iter = nullptr;
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
													
		m_pTerrainGroup->setOrigin(Ogre::Vector3::ZERO);
		
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
		////defaultimp.layerList[0].textureNames.push_back("grass_0_normalheight.dds");
		
	}
	
	wxVector< RefObjSmPtr<TerrainPage> >::iterator iter;
	for (iter = terrainDef.m_terrainPages.begin(); iter != terrainDef.m_terrainPages.end(); ++iter)
	{
		TerrainPage *pTerrPage = (*iter).In();
		Ogre::Image img;
		img.load(pTerrPage->m_filename.ToStdString(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		
		if(m_pRenderMgr)
		{
			m_pTerrainGroup->defineTerrain(pTerrPage->m_pageX, pTerrPage->m_pageY, &img);
		}
		
		if(m_pPhysicsMgr)		
		{
			
			btHeightfieldTerrainShape *pTerrainShape = new btHeightfieldTerrainShape(terrainDef.m_mapSize,
																				terrainDef.m_mapSize,
																				img.getData(),
																				1,
																				0,
																				255,
																				1,
																				PHY_UCHAR,
																				true);
			
			btDefaultMotionState *state = new btDefaultMotionState();
			
			btScalar mass = 0;
			btVector3 localInertia(0, 0, 0);
			
			pTerrainShape->calculateLocalInertia(mass, localInertia);
			
			btRigidBody::btRigidBodyConstructionInfo info(mass, state, pTerrainShape, localInertia);
			
			btRigidBody *body = new btRigidBody(info);
			
			m_physTerrainGrid.push_back(body);
			m_pPhysicsMgr->GetDynamicsWorld()->addRigidBody(body);
		}
		
	}
	
	if(m_pTerrainGroup)
	{
		m_pTerrainGroup->loadAllTerrains(true);
		m_pTerrainGroup->freeTemporaryResources();
	}
	
	return FWG_NO_ERROR;
}

