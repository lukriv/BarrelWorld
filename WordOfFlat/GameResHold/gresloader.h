#ifndef __GAME_RESOURCE_LOADER_H__
#define __GAME_RESOURCE_LOADER_H__

#include <SFML/Graphics/Texture.hpp>
#include <Box2D/box2D.h>

#include "../GameSystem/glog.h"
#include "../GameObjects/ggeometry.h"
#include "../GameObjects/gameobjdef.h"



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
	GameGeomResItem() : m_refCount(0), m_pGeometry(NULL) {}
};

struct GamePhysJointItem {
	wxInt32 m_refCount;
	b2JointDef *m_pJointDef;
	wxVector<GamePhysObjId> m_bodyRefList;
public:
	GamePhysJointItem() : m_refCount(0), m_pJointDef(NULL) {}
};

struct GamePhysBodyItem {
	wxInt32 m_refCount;
	b2BodyDef *m_pBodyDef;
	wxVector<GamePhysObjId> m_fixtureRefList; // reference indet to Fixture map
public:
	GamePhysBodyItem() : m_refCount(0), m_pBodyDef(NULL) {}
};

struct GamePhysFixItem {
	wxInt32 m_refCount;
	b2FixtureDef *m_pFixtureDef;
	wxDword m_shapeRef; //reference index to Geometry map
public:
	GamePhysFixItem() : m_refCount(0), m_pFixtureDef(NULL) {}
};

typedef std::map<GameTextureId, GameTexResItem> TGameTextureMap;
typedef std::pair<GameTextureId, GameTexResItem> TGameTextureMapItem;
	
typedef std::map<GameShapeId, GameGeomResItem> TGameGeometryMap;
typedef std::pair<GameShapeId, GameGeomResItem> TGameGeometryMapItem;

typedef std::map<GamePhysObjId, GamePhysJointItem> TGamePhysJointMap;
typedef std::pair<GamePhysObjId, GamePhysJointItem> TGamePhysJointMapItem;

typedef std::map<GamePhysObjId, GamePhysBodyItem> TGamePhysBodyMap;
typedef std::pair<GamePhysObjId, GamePhysBodyItem> TGamePhysBodyMapItem;

typedef std::map<GamePhysObjId, GamePhysFixItem> TGamePhysFixMap;
typedef std::pair<GamePhysObjId, GamePhysFixItem> TGamePhysFixMapItem;

class IGameResourceLoader {
public:
	virtual GameErrorCode LoadTextureFromFile(const wxChar* texFileName, sf::Texture *&pTex) = 0;
	
	virtual GameErrorCode LoadTexture(GameTextureId texID, sf::Texture *&pTex) = 0;
	virtual GameErrorCode LoadGeometry(GameShapeId geomID, IGameGeometry *&pShape) = 0;
	virtual GameErrorCode LoadPhysJoint(GamePhysObjId jointID, b2JointDef *&pJointDef) = 0;
	virtual GameErrorCode LoadPhysBody(GamePhysObjId bodyID, b2BodyDef *&pBodyDef) = 0;
	virtual GameErrorCode LoadPhysFixture(GamePhysObjId fixID, b2FixtureDef *&pFixDef) = 0;
	
	virtual GameErrorCode LoadTextureList(TGameTextureMap &texList) = 0;
	virtual GameErrorCode LoadGeometryList(TGameGeometryMap &geomList) = 0;
	virtual GameErrorCode LoadPhysJointList(TGamePhysJointMap &physJointList) = 0;
	virtual GameErrorCode LoadPhysBodyList(TGamePhysBodyMap &physBodyList) = 0;
	virtual GameErrorCode LoadPhysFixList(TGamePhysFixMap &physFixList) = 0;
	
	virtual ~IGameResourceLoader() {};
};


#endif //__GAME_RESOURCE_LOADER_H__