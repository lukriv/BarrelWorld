#ifndef __GAME_RESOURCE_LOADER_H__
#define __GAME_RESOURCE_LOADER_H__

#include "../GameSystem/glog.h"

struct GameTexResItem {
	wxString m_texFileName;
	wxInt32 m_refCount;
	sf::Texture *m_pTexture;
public:
	GameTexResItem() : m_refCount(0), m_pTexture(NULL) {}
};

struct GameGeomResItem {
	wxInt32 m_refCount;
	IGameGeometry *m_pGeometry;
public:
	GameTexResItem() : m_refCount(0), m_pGeometry(NULL) {}
};

typedef std::map<GameTextureId, GameTexResItem> TGameTextureMap;
typedef std::pair<GameTextureId, GameTexResItem> TGameTextureMapItem;
	
typedef std::map<GameShapeId, GameGeomResItem> TGameGeometryMap;
typedef std::pair<GameShapeId, GameGeomResItem> TGameGeometryMapItem;

class IGameResourceLoader {
public:
	virtual GameErrorCode LoadTexture(GameTextureId texID, sf::Texture *&pTex) = 0;
	virtual GameErrorCode LoadTextureFromFile(const wxChar* texFileName, sf::Texture *&pTex) = 0;
	
	virtual GameErrorCode LoadShape(GameShapeId geomID, IGameGeometry *&pShape) = 0;
	
	virtual GameErrorCode LoadTextureList(TGameTextureMap &texList) = 0;
	virtual GameErrorCode LoadGeometryList(TGameGeometryMap &geomList) = 0;
	
	virtual ~IGameResourceLoader() {};
};


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
	
	virtual GameErrorCode LoadShape(GameShapeId geomID, IGameGeometry *&pShape);
	
	virtual GameErrorCode LoadTextureList(TGameTextureMap &texList);
	virtual GameErrorCode LoadGeometryList(TGameGeometryMap &geomList);
	
};


#endif //__GAME_RESOURCE_LOADER_H__