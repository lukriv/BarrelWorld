#ifndef __GAME_SERVER_OBJECT_IMPL_H__
#define __GAME_SERVER_OBJECT_IMPL_H__

#include "gamesrvobj.h"
#include "../GameSystem/glog.h"
#include "gameobjdef.h"

class GameObjectSrv : public IGameObjectSrv {
private:
	GameObjectType m_objType;
	GameObjectId m_objID;
	GameFlatWorldSrv *m_pParent;
	b2Body* m_pBody;
	IGameObject* m_pDrawableObj;
public:
	void SetObjID(GameObjectId objID) { m_objID = objID;}
	void SetObjType(GameObjectType objType) { m_objType = objType;}
	
	virtual GameObjectType GetObjType() { return m_objType; }
	virtual GameObjectId GetObjId() { return m_objID; }
	virtual void SetDrawableObj(IGameObject *pDrawableObj);
	
public:
	GameObjectSrv(GameFlatWorldSrv* pParent, b2Body* pBody): m_objType(GAME_OBJECT_TYPE_UNKNOWN),
						m_objID(GAME_OBJECT_ID_UNKNOWN),
						m_pParent(pParent),
						m_pBody(pBody),
						m_pDrawableObj(nullptr) {}
	
	bool IsInitialized() { return (m_objID != GAME_OBJECT_ID_UNKNOWN); }
	
	GameErrorCode AddPart( b2FixtureDef& pFixture);
};

#endif //__GAME_SERVER_OBJECT_IMPL_H__