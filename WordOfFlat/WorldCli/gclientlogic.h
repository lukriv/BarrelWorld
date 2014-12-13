#ifndef __GAME_CLIENT_LOGIC_H__
#define __GAME_CLIENT_LOGIC_H__

#include <MyGUI/MyGUI_Widget.h>
#include <GameSystem/glog.h>
#include <GameSystem/gthread.h>
#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>
#include <GameSystem/refobjectsmptr.h>

#include <GameComp/gcompmgr.h>
#include <GameResHold/gdefholder.h>
#include <GameResHold/gentityfactory.h>
#include <MainWindow/ginputsystem.h>
#include "gmenumgr.h"

class GameMenu;

class ClientGameLogic : public RefObjectImpl<IRefObject>, protected GameThread {
	RefObjSmPtr<GameEntityFactory> m_spEntityFactory;
	
	GameLoggerPtr m_pLogger;
	wxCriticalSection m_renderLocker;
	bool m_stopRequest;
	bool m_isInitialized;
public:
	ClientGameLogic();
	virtual ~ClientGameLogic();
	
	GameErrorCode Initialize(GameLogger *pLogger, Ogre::RenderWindow *pWindow, Ogre::SceneManager *pSceneManager, GameInputSystem *pInputSystem, GameMenuSystem *pGameMenu);
	
	void Uninitialize();
	
	GameErrorCode LoadGame(GameDefinitionHolder &defHolder);
	
	GameErrorCode StoreGame (GameDefinitionHolder &defHolder);
	
	GameErrorCode StartGame ();
	
	GameErrorCode StopGame ();
	
	bool IsStopped();
	
	wxCriticalSection& GetRenderLocker() { return m_renderLocker; }

// global menu functions
public:
	void SetExitMenu(MyGUI::Widget* _sender);
	
	void SetShowExit(MyGUI::Widget* _sender);
	
// keyboard functions
	void SetExit(bool keyDown);

protected:
	virtual void *Entry();
	
	GameErrorCode StopRequest();
	
protected:
	GameErrorCode PrepareCameras();
	GameErrorCode PrepareLights();
	GameErrorCode PrepareGlobalInput();
	

	
};

#endif // __GAME_CLIENT_LOGIC_H__
