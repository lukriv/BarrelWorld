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
	virtual GameErrorCode LoadTextureFromFile(const wxChar* texFileName, sf::Texture *&pTex);

	virtual GameErrorCode LoadTexture(GameTextureId texID, sf::Texture *&pTex);
	virtual GameErrorCode LoadGeometry(GameShapeId geomID, IGameGeometry *&pShape);
	virtual GameErrorCode LoadPhysJoint(GamePhysObjId jointID, b2JointDef *&pJointDef);
	virtual GameErrorCode LoadPhysBody(GamePhysObjId bodyID, b2BodyDef *&pBodyDef);
	virtual GameErrorCode LoadPhysFixture(GamePhysObjId fixID, b2FixtureDef *&pFixDef);
	
	virtual GameErrorCode LoadTextureList(TGameTextureMap &texList);
	virtual GameErrorCode LoadGeometryList(TGameGeometryMap &geomList);
	virtual GameErrorCode LoadPhysJointList(TGamePhysJointMap &physJointList);
	virtual GameErrorCode LoadPhysBodyList(TGamePhysBodyMap &physBodyList);
	virtual GameErrorCode LoadPhysFixList(TGamePhysFixMap &physFixList);
	
};


#endif //__GAME_TEST_LOADER_H__