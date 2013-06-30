#ifndef __GAME_TEST_LOADER_H__
#define __GAME_TEST_LOADER_H__

#include "../GameResHold/gresloader.h"

class GameTestResourceLoader : public IGameResourceLoader {
 private:
	bool m_isInitialized;
	GameLoggerPtr m_spLogger;
	 
public:
	GameTestResourceLoader() : m_isInitialized(false) {}
	~GameTestResourceLoader() {}
	
	GameErrorCode Initialize(GameLogger *pLogger = NULL);
	
public:
	virtual GameErrorCode LoadTexture(GameTextureId texID, sf::Texture *&pTex);
	virtual GameErrorCode LoadTextureFromFile(const wxChar* texFileName, sf::Texture *&pTex);
	
	virtual GameErrorCode LoadGeometry(GameShapeId geomID, IGameGeometry *&pShape);
	
	virtual GameErrorCode LoadTextureList(TGameTextureMap &texList);
	virtual GameErrorCode LoadGeometryList(TGameGeometryMap &geomList);
	
};


#endif //__GAME_TEST_LOADER_H__