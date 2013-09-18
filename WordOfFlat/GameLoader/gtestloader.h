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
	virtual GameErrorCode LoadTextureFromFile(const wxChar* texFileName, sf::Texture *&pTexImage);

	virtual GameErrorCode LoadTexture(GameObjectID texID, sf::Texture *&pTexImage);
	virtual GameErrorCode LoadGeometry(GameObjectID geomID, GameGeometryContainer *&pShape);
	virtual GameErrorCode LoadPhysJoint(GameObjectID jointID, b2JointDef *&pJointDef);
	virtual GameErrorCode LoadPhysBody(GameObjectID bodyID, b2BodyDef *&pBodyDef);
	virtual GameErrorCode LoadPhysFixture(GameObjectID fixID, b2FixtureDef *&pFixDef);
	
	virtual GameErrorCode LoadTextureList(TGameTextureMap &texList);
	virtual GameErrorCode LoadGeometryList(TGameGeometryMap &geomList);
	virtual GameErrorCode LoadPhysJointList(TGamePhysJointMap &physJointList);
	virtual GameErrorCode LoadPhysBodyList(TGamePhysBodyMap &physBodyList);
	virtual GameErrorCode LoadPhysFixList(TGamePhysFixMap &physFixList);
	
};


#endif //__GAME_TEST_LOADER_H__