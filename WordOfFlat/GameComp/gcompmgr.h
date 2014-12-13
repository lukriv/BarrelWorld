#ifndef __GAME_COMPONENT_MANAGER_H__
#define __GAME_COMPONENT_MANAGER_H__

#include <OGRE/OgreSceneManager.h>
#include <MainWindow/gsettings.h>
#include "gentitymgr.h"
#include "RenderComp/grendercmgr.h"
#include "InputComp/ginputsystem.h"
#include "MenuComp/gmenumgr.h"
#include "LogicComp/glogiccmgr.h"


class GameCompManager : public RefObjectImpl<IRefObject> {
	GameLoggerPtr m_spLogger;
	RenderCompManager m_renderMgr;
	GameMenuSystem m_menuMgr;
	GameInputSystem m_inputSystem;
	LogicCompManager m_logicMgr;
	GameEntityManager m_entityMgr; // should be the last manager (first in destroy)
public:
	GameCompManager(GameLogger *pLogger);
	~GameCompManager();
	
	GameErrorCode Initialize(GameEngineSettings& settings);
	void Uninitialize();
	
	inline RenderCompManager& GetRenderManager() { return m_renderMgr; }
	inline GameMenuSystem& GetMenuSystem() { return m_menuMgr; }
	inline GameInputSystem&	GetInputSystem() { return m_inputSystem; }
	inline LogicCompManager& GetLogicManager() { return m_logicMgr; }
	inline GameEntityManager& GetEntityManager() { return m_entityMgr; }

};

#endif // __GAME_COMPONENT_MANAGER_H__
