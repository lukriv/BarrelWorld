#ifndef __GAME_COMPONENT_MANAGER_H__
#define __GAME_COMPONENT_MANAGER_H__

#include <OGRE/OgreSceneManager.h>
#include <MainWindow/gsettings.h>
#include "gentitymgr.h"
#include "RenderComp/grendersystem.h"
#include "InputComp/ginputsystem.h"
#include "MenuComp/gmenumgr.h"
#include "PhysicsComp/gphyssystem.h"
#include "LogicComp/glogicsystem.h"
#include "TerrainComp/gterrainmgr.h"




class GameCompManager : public RefObjectImpl<IRefObject> {
	GameLoggerPtr m_spLogger;
	// systems
	GameRenderSystem m_renderSystem;
	GameMenuSystem m_menuSystem;
	GameInputSystem m_inputSystem;
	GamePhysicsSystem m_physicsSystem;
	GameLogicSystem m_logicSystem;

	// specific managers
	GameTerrainManager m_terrainMgr; 
	GameEntityManager m_entityMgr; // should be the last manager (first in destroy)
	
public:
	GameCompManager(GameLogger *pLogger);
	~GameCompManager();
	
	GameErrorCode Initialize(GameEngineSettings& settings);
	void Uninitialize();
	
	inline GameRenderSystem& GetRenderSystem() { return m_renderSystem; }
	inline GameMenuSystem& GetMenuSystem() { return m_menuSystem; }
	inline GameInputSystem&	GetInputSystem() { return m_inputSystem; }
	inline GamePhysicsSystem& GetPhysicsSystem() { return m_physicsSystem; }
	inline GameLogicSystem& GetLogicSystem() { return m_logicSystem; }
	
	inline GameEntityManager& GetEntityManager() { return m_entityMgr; }
	inline GameTerrainManager& GetTerrainManager() { return m_terrainMgr; }
	
};

#endif // __GAME_COMPONENT_MANAGER_H__
