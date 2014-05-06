#ifndef __GAME_CLIENT_LOGIC_H__
#define __GAME_CLIENT_LOGIC_H__

#include "gmainlogic.h" // Base class: GameMainLogicBase

class ClientGameLogic : public RefObjectImpl<GameMainLogicBase>
{
	RefObjSmPtr<GameCompManager> m_spCompManager;
public:
	ClientGameLogic();
	virtual ~ClientGameLogic();
	
	GameErrorCode Initialize(GameLogger *pLogger, Ogre::RenderWindow *pWindow, Ogre::SceneManager *pSceneManager);
	
	GameErrorCode LoadGame(GameDefinitionHolder &defHolder);
	
	GameErrorCode StoreGame (GameDefinitionHolder &defHolder);
	
};

#endif // __GAME_CLIENT_LOGIC_H__
