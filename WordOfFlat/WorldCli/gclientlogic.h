#ifndef __GAME_CLIENT_LOGIC_H__
#define __GAME_CLIENT_LOGIC_H__

#include "gmainlogic.h" // Base class: GameMainLogicBase

#include <GameSystem/gthread.h>

class ClientGameLogic : public RefObjectImpl<GameMainLogicBase>, protected GameThread {
	RefObjSmPtr<GameEntityFactory> m_spEntityFactory;
	RefObjSmPtr<GameCompManager> m_spCompManager;
	RefObjSmPtr<GameMenu> m_spGameMenus;
	RefObjSmPtr<GameInputSystem> m_spInputSystem;
	GameLoggerPtr m_pLogger;
	bool m_stopped;
	bool m_stopRequest;
	bool m_isInitialized;
public:
	ClientGameLogic();
	virtual ~ClientGameLogic();
	
	GameErrorCode Initialize(GameLogger *pLogger, Ogre::RenderWindow *pWindow, Ogre::SceneManager *pSceneManager, GameInputSystem *pInputSystem, GameMenu *pGameMenu);
	
	void Uninitialize();
	
	GameErrorCode LoadGame(GameDefinitionHolder &defHolder);
	
	GameErrorCode StoreGame (GameDefinitionHolder &defHolder);
	
	GameErrorCode StartGame ();
	
	GameErrorCode StopGame ();
	
	bool IsStopped() { return m_stopped; }

// global menu functions
public:
	void SetExit(MyGUI::Widget* _sender) { m_stopRequest = true; }

protected:
	virtual void *Entry();
	
	GameErrorCode StopRequest();
	
protected:
	GameErrorCode PrepareCameras();
	

	
};

#endif // __GAME_CLIENT_LOGIC_H__
