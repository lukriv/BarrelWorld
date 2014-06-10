#ifndef __GAME_TEST_LOADER_H__
#define __GAME_TEST_LOADER_H__

#include "../GameResHold/gdefloader.h"
#include "../GameResHold/gdefholder.h"
#include "../GameSystem/glog.h"

class GameTestResourceLoader : public IGameDefLoader
{
private:
	bool m_isInitialized;
	GameLoggerPtr m_spLogger;

private:
	GameErrorCode LoadMeshes(GameDefinitionHolder& defHolder);
	GameErrorCode LoadMaterials(GameDefinitionHolder& defHolder);
	GameErrorCode LoadCameras(GameDefinitionHolder& defHolder);
	GameErrorCode LoadRenderObj(GameDefinitionHolder& defHolder);
	GameErrorCode LoadAnimations(GameDefinitionHolder& defHolder);
	GameErrorCode LoadAnimators(GameDefinitionHolder& defHolder);
	GameErrorCode LoadInput(GameDefinitionHolder& defHolder);
	GameErrorCode LoadLogic(GameDefinitionHolder& defHolder);
	GameErrorCode LoadEntities(GameDefinitionHolder& defHolder);
public:
	GameTestResourceLoader() : m_isInitialized(false) {}
	~GameTestResourceLoader() {}

	GameErrorCode Initialize(GameLogger *pLogger = NULL);
public:
	virtual GameErrorCode Load(GameDefinitionHolder& defHolder);

};


#endif //__GAME_TEST_LOADER_H__
