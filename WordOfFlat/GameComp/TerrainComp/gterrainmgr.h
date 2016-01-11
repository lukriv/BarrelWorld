#ifndef __GAME_TERRAIN_MANAGER_H__
#define __GAME_TERRAIN_MANAGER_H__

#include <wx/vector.h>
#include <wx/scopedarray.h>
#include <GameSystem/gmap.h>
#include <GameSystem/glog.h>
#include <GameSystem/refobjectsmptr.h>

class GameRenderSystem;
class GamePhysicsSystem;
struct TerrainDef;
struct TerrainPage;

class btRigidBody;
class wxXmlNode;
namespace Ogre {
	class TerrainGlobalOptions;
	class TerrainGroup;
};


struct PhysicsTerrainData {
	wxInt32 m_objectCount;
	btRigidBody* m_pRigidBody;
	float* m_pTerrainData;
	
	PhysicsTerrainData() : m_objectCount(0), m_pRigidBody(nullptr), m_pTerrainData(nullptr) {}
	
	PhysicsTerrainData& operator=(const PhysicsTerrainData& terData)
	{
		if(this == &terData)
		{
			return *this;
		}
		
		m_objectCount = terData.m_objectCount;
		m_pRigidBody = terData.m_pRigidBody;
		m_pTerrainData = terData.m_pTerrainData;
		
		return *this;
	}
};


class GameTerrainManager {
	typedef wxVector< PhysicsTerrainData > TPhysTerrainGrid;
private:
	GameLoggerPtr m_spLogger;
	
	GameRenderSystem *m_pRenderMgr;
	GamePhysicsSystem *m_pPhysicsMgr;
	
	
	Ogre::TerrainGlobalOptions* m_globalTerrainOptions;
	Ogre::TerrainGroup* m_pTerrainGroup;
	
	wxInt32 m_gridOrigin[2];
	TPhysTerrainGrid m_physTerrainGrid;
	
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
	GameErrorCode Initialize(GameRenderSystem *pRenderMgr, GamePhysicsSystem *pPhysMgr);
	
	void Uninitialize();
	
	inline GameLogger * GetLogger() { return m_spLogger; }
	
	
	
	
	/**
	 * \brief Create new terrain group from terrain definition
	 * 
	 * This creates whole terrain group with all terrain pages. No return terrain entities or group is necessary.
	 * 
	 * @param terrainDef
	 */
	GameErrorCode CreateTerrainGroup (TerrainDef &terrainDef);
	
	GameErrorCode Load(wxXmlNode *pNode);
	GameErrorCode Store(wxXmlNode *pParentNode);
	
protected:
	GameErrorCode PrepareTerrainGlobalOptions();
	
	GameErrorCode CreateTerrainDef(wxXmlNode* pNode, RefObjSmPtr<TerrainDef>& spTerrain);
	GameErrorCode CreateTerrainPageDef(wxXmlNode* pNode, RefObjSmPtr<TerrainPage>& spTerrainPage);

	GameErrorCode LoadTerrainData(const wxString &resource, wxDword terrDataSize, wxScopedArray<float> &retData, float &minHeight, float &maxHeight);
};


#endif //__GAME_TERRAIN_MANAGER_H__