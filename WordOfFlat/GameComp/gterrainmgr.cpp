#include "gterrainmgr.h"

#include "OGRE/Terrain/OgreTerrain.h"

#include "RenderComp/grendercmgr.h"
#include "PhysicsComp/gphyscmgr.h"



GameTerrainManager::GameTerrainManager(GameLogger* pLogger) : m_spLogger(pLogger)
															, m_globalTerrainOptions(nullptr)
{}



GameErrorCode GameTerrainManager::Initialize(RenderCompManager* pRenderMgr, PhysicsCompManager* pPhysMgr)
{
	m_pRenderMgr = pRenderMgr;
	m_pPhysicsMgr = pPhysMgr;
	
	
	if(m_pRenderMgr)
	{
		m_globalTerrainOptions = OGRE_NEW Ogre::TerrainGlobalOptions();
	}
	
	
	
	return FWG_NO_ERROR;
	
}


GameErrorCode GameTerrainManager::CreateTerrainGroup(TerrainDef& terrainDef)
{
	
}
