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

#include "TransformComp/gtranscmgr.h"
#include "InputComp/ginputcompmgr.h"
#include "LogicComp/gmoveablemgr.h"
#include "PhysicsComp/gphyscompmgr.h"
#include "RenderComp/grenderpositionmgr.h"
#include "RenderComp/grenderobjmgr.h"


class GameCompManager : public RefObjectImpl<IRefObject> {
	GameLoggerPtr m_spLogger;
	// systems
	GameRenderSystem m_renderSystem;
	GameMenuSystem m_menuSystem;
	GameInputSystem m_inputSystem;
	GamePhysicsSystem m_physicsSystem;
	GameLogicSystem m_logicSystem;

	// specific managers
	GameEntityManager m_entityMgr; // should be the last manager (first in destroy)
	GameTerrainManager m_terrainMgr; 
	
	// component managers
	TransformCompManager m_tranformMgr;
	InputComponentManager m_inputCompMgr;
	MoveableManager	m_moveableCompMgr;
	PhysicsCompManager m_physicsCompMgr;
	RenderPositionManager m_renderPosMgr;
	RenderObjectManager m_renderObjMgr;
	
	
	
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
	
	inline TransformCompManager& GetTransformManager() { return m_tranformMgr; }
	inline InputComponentManager& GetInputManager() { return m_inputCompMgr; }
	inline MoveableManager& GetMoveableManager() { return m_moveableCompMgr; }
	inline PhysicsCompManager& GetPhysicsManager() { return m_physicsCompMgr; }
	inline RenderPositionManager& GetRenderPosManager() { return m_renderPosMgr; }
	inline RenderObjectManager& GetRenderObjManager() { return m_renderObjMgr; }

};

#endif // __GAME_COMPONENT_MANAGER_H__
