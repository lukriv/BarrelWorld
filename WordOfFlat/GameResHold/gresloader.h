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

struct GamePhysJointItem {
	b2DistanceJointDef
};

struct GamePhysBodyItem {
	b2BodyDef m_bodyDef;
	wxVector<wxDword> m_fixtureRefList; // reference indet to Fixture map
};

struct GamePhysFixItem {
	b2FixtureDef m_fixtureDef;
	wxDword m_shapeRef; //reference index to Geometry map
};

typedef std::map<GameTextureId, GameTexResItem> TGameTextureMap;
typedef std::pair<GameTextureId, GameTexResItem> TGameTextureMapItem;
	
typedef std::map<GameShapeId, GameGeomResItem> TGameGeometryMap;
typedef std::pair<GameShapeId, GameGeomResItem> TGameGeometryMapItem;

typedef std::map<wxDword, GamePhysJointItem> TGamePhysJointMap;
typedef std::pair<wxDword, GamePhysJointItem> TGamePhysJointMapItem;

typedef std::map<wxDword, GamePhysBodyItem> TGamePhysBodyMap;
typedef std::pair<wxDword, GamePhysBodyItem> TGamePhysBodyMapItem;

typedef std::map<wxDword, GamePhysFixItem> TGamePhysFixMap;
typedef std::pair<wxDword, GamePhysFixItem> TGamePhysFixMapItem;

class IGameResourceLoader {
public:
	virtual GameErrorCode LoadTexture(GameTextureId texID, sf::Texture *&pTex) = 0;
	virtual GameErrorCode LoadTextureFromFile(const wxChar* texFileName, sf::Texture *&pTex) = 0;
	
	virtual GameErrorCode LoadGeometry(GameShapeId geomID, IGameGeometry *&pShape) = 0;

	
	virtual GameErrorCode LoadTextureList(TGameTextureMap &texList) = 0;
	virtual GameErrorCode LoadGeometryList(TGameGeometryMap &geomList) = 0;
	virtual GameErrorCode LoadPhysJointList(TGamePhysJointMap &physJointList) = 0;
	virtual GameErrorCode LoadPhysBodyList(TGamePhysBodyMap &physBodyList) = 0;
	virtual GameErrorCode LoadPhysFixList(TGamePhysFixMap &physFixList) = 0;
	
	virtual ~IGameResourceLoader() {};
};


#endif //__GAME_RESOURCE_LOADER_H__