#ifndef __GAME_RESOURCE_LOADER_H__
#define __GAME_RESOURCE_LOADER_H__

#include <SFML/Graphics/Texture.hpp>
#include <Box2D/box2D.h>

#include "../GameSystem/glog.h"
#include "../GameObjects/ggeometry.h"
#include "../GameObjects/gameobjdef.h"



struct GameTexResItem {
	wxInt32 m_refCount;
	wxString m_texFileName;
	sf::Texture *m_pTexture;
public:
	GameTexResItem() : m_refCount(0), m_pTexture(NULL) {}
};

struct GameGeomResItem {
	wxInt32 m_refCount;
	GameGeometryContainer *m_pGeometry;
public:
	GameGeomResItem() : m_refCount(0), m_pGeometry(NULL) {}
};

struct GamePhysJointItem {
	wxInt32 m_refCount;
	b2JointDef *m_pJointDef;
	wxVector<GameObjectID> m_bodyRefList;
public:
	GamePhysJointItem() : m_refCount(0), m_pJointDef(NULL) {}
};

struct GamePhysBodyItem {
	wxInt32 m_refCount;
	b2BodyDef *m_pBodyDef;
	wxVector<GameObjectID> m_fixtureRefList; // reference indet to Fixture map
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

typedef std::map<GameObjectID, GameTexResItem> TGameTextureMap;
typedef std::pair<GameObjectID, GameTexResItem> TGameTextureMapItem;
	
typedef std::map<GameObjectID, GameGeomResItem> TGameGeometryMap;
typedef std::pair<GameObjectID, GameGeomResItem> TGameGeometryMapItem;

typedef std::map<GameObjectID, GamePhysJointItem> TGamePhysJointMap;
typedef std::pair<GameObjectID, GamePhysJointItem> TGamePhysJointMapItem;

typedef std::map<GameObjectID, GamePhysBodyItem> TGamePhysBodyMap;
typedef std::pair<GameObjectID, GamePhysBodyItem> TGamePhysBodyMapItem;

typedef std::map<GameObjectID, GamePhysFixItem> TGamePhysFixMap;
typedef std::pair<GameObjectID, GamePhysFixItem> TGamePhysFixMapItem;

class IGameResourceLoader {
public:
	virtual GameErrorCode LoadTextureFromFile(const wxChar* texFileName, sf::Texture *&pTexImage) = 0;
	
	virtual GameErrorCode LoadTexture(GameObjectID texID, sf::Texture *&pTexImage) = 0;
	virtual GameErrorCode LoadGeometry(GameObjectID geomID, GameGeometryContainer *&pShape) = 0;
	virtual GameErrorCode LoadPhysJoint(GameObjectID jointID, b2JointDef *&pJointDef) = 0;
	virtual GameErrorCode LoadPhysBody(GameObjectID bodyID, b2BodyDef *&pBodyDef) = 0;
	virtual GameErrorCode LoadPhysFixture(GameObjectID fixID, b2FixtureDef *&pFixDef) = 0;
	
	virtual GameErrorCode LoadTextureList(TGameTextureMap &texList) = 0;
	virtual GameErrorCode LoadGeometryList(TGameGeometryMap &geomList) = 0;
	virtual GameErrorCode LoadPhysJointList(TGamePhysJointMap &physJointList) = 0;
	virtual GameErrorCode LoadPhysBodyList(TGamePhysBodyMap &physBodyList) = 0;
	virtual GameErrorCode LoadPhysFixList(TGamePhysFixMap &physFixList) = 0;
	
	virtual ~IGameResourceLoader() {};
};


#endif //__GAME_RESOURCE_LOADER_H__