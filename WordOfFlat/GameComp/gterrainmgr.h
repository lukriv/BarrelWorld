#ifndef __GAME_TERRAIN_MANAGER_H__
#define __GAME_TERRAIN_MANAGER_H__

#include <wx/vector.h>
#include <GameSystem/gmap.h>
#include <GameSystem/glog.h>
#include <GameResHold/gdeftables.h>


class RenderCompManager;
class PhysicsCompManager;

class GameTerrainEntity;

namespace Ogre {
	class TerrainGlobalOptions;
	class TerrainGroup;
};


class GameTerrainManager {
	typedef wxVector< wxVector<GameTerrainEntity*> > TTerrainGrid;
private:
	GameLoggerPtr m_spLogger;
	
	RenderCompManager *m_pRenderMgr;
	PhysicsCompManager *m_pPhysicsMgr;
	
	
	Ogre::TerrainGlobalOptions* m_globalTerrainOptions;
	Ogre::TerrainGroup* m_pTerrainGroup;
	
	TTerrainGrid m_terrainGrid;
	
public:
	GameTerrainManager(GameLogger *pLogger);
	~GameTerrainManager();
	
	/**
	 * \brief Initialize terrain manager
	 * 
	 * Terrain manager could be initialized with render manager only or with physics manager only or with both managers.
	 * 
	 * @param pRenderMgr
	 * @param pPhysMgr
	 * @return 
	 */
	GameErrorCode Initialize(RenderCompManager *pRenderMgr, PhysicsCompManager *pPhysMgr);
	
	inline GameLogger * GetLogger() { return m_spLogger; }
	
	/**
	 * \brief Create new terrain group from terrain definition
	 * 
	 * This creates whole terrain group with all terrain pages. No return terrain entities or group is necessary.
	 * 
	 * @param terrainDef
	 */
	GameErrorCode CreateTerrainGroup (TerrainDef &terrainDef);
	
};


#endif //__GAME_TERRAIN_MANAGER_H__