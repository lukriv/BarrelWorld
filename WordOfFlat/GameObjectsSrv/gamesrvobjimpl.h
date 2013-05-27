#ifndef __GAME_SERVER_OBJECT_IMPL_H__
#define __GAME_SERVER_OBJECT_IMPL_H__

#include "gamesrvobj.h"
#include "../GameSystem/glog.h"

class GameObjectSrvImpl : public IGameObjectSrv {
private:
	wxVector<b2Fixture*> m_pFixtureList;
	wxVector<b2Shape*> m_pShapeList;
	b2Body* m_pBody;
	IGameObject* m_pDrawableObj;
public:
	virtual GameObjectType GetType();
	virtual wxInt32 GetId();
	virtual void SetDrawableObj(IGameObject *pDrawableObj);
	
public:
	GameObjectSrvImpl(): m_pBody(nullptr), m_pDrawableObj(nullptr) {}
	
	GameErrorCode Initialize( b2BodyDef* pBodyDef,  GameLogger* pLogger = NULL);
	bool IsInitialized() { return (m_pBody != nullptr); }
	
	GameErrorCode AddPart(b2Shape* pShape, b2FixtureDef* pFixture = NULL);
};

#endif //__GAME_SERVER_OBJECT_IMPL_H__