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

typedef std::map<wxString, GameTexResItem> TGameTextureMap;
typedef std::pair<wxString, GameTexResItem> TGameTextureMapItem;
	
typedef std::map<wxString, GameGeomResItem> TGameGeometryMap;
typedef std::pair<wxString, GameGeomResItem> TGameGeometryMapItem;

class IGameResourceLoader {
public:
	virtual GameErrorCode LoadTexture(const wxChar* texName, sf::Texture *&pTex) = 0;
	virtual GameErrorCode LoadTextureFromFile(const wxChar* texFileName, sf::Texture *&pTex) = 0;
	
	virtual GameErrorCode LoadShape(const wxChar* geomName, IGameGeometry *&pShape) = 0;
	
	virtual GameErrorCode LoadTextureList(wxVector<TGameTextureMapItem> &texList) = 0;
	virtual GameErrorCode LoadGeometryList(wxVector<TGameGeometryMapItem> &geomList) = 0;
	
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
	virtual GameErrorCode LoadTexture(const wxChar* texName, sf::Texture *&pTex);
	virtual GameErrorCode LoadTextureFromFile(const wxChar* texFileName, sf::Texture *&pTex);
	
	virtual GameErrorCode LoadShape(const wxChar* geomName, IGameGeometry *&pShape);
	
	virtual GameErrorCode LoadTextureList(wxVector<TGameTextureMapItem> &texList);
	virtual GameErrorCode LoadGeometryList(wxVector<TGameGeometryMapItem> &geomList);
	
};


#endif //__GAME_RESOURCE_LOADER_H__