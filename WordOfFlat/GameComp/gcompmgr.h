#ifndef __GAME_COMPONENT_MANAGER_H__
#define __GAME_COMPONENT_MANAGER_H__

#include <OGRE/OgreSceneManager.h>
#include <MainWindow/gsettings.h>
#include "gentitymgr.h"
#include "TransformComp/gtranscmgr.h"
#include "RenderComp/grendercmgr.h"
#include "InputComp/ginputsystem.h"
#include "MenuComp/gmenumgr.h"
#include "PhysicsComp/gphyssystem.h"
#include "LogicComp/glogicsystem.h"
#include "TerrainComp/gterrainmgr.h"


class GameCompManager : public RefObjectImpl<IRefObject> {
	GameLoggerPtr m_spLogger;
	TransformCompManager m_tranformMgr;
	RenderSystem m_renderMgr;
	GameMenuSystem m_menuMgr;
	GameInputSystem m_inputSystem;
	PhysicsSystem m_physicsManager;
	LogicSystem m_logicMgr;
	GameEntityManager m_entityMgr; // should be the last manager (first in destroy)
	GameTerrainManager m_terrainMgr; 
public:
	GameCompManager(GameLogger *pLogger);
	~GameCompManager();
	
	GameErrorCode Initialize(GameEngineSettings& settings);
	void Uninitialize();
	
	inline TransformCompManager& GetTransformManager() { return m_tranformMgr; }
	inline RenderSystem& GetRenderManager() { return m_renderMgr; }
	inline GameMenuSystem& GetMenuSystem() { return m_menuMgr; }
	inline GameInputSystem&	GetInputSystem() { return m_inputSystem; }
	inline PhysicsSystem& GetPhysicsManager() { return m_physicsManager; }
	inline LogicSystem& GetLogicManager() { return m_logicMgr; }
	inline GameEntityManager& GetEntityManager() { return m_entityMgr; }
	inline GameTerrainManager& GetTerrainManager() { return m_terrainMgr; }

};

#endif // __GAME_COMPONENT_MANAGER_H__
