#ifndef __GAME_OBJECT_BASE_H__
#define __GAME_OBJECT_BASE_H__

#include "gameobjdef.h"
#include "gobject.h"

class GameObjectBase : public IGameObject {
private:
	GameObjectId m_objId;

public:
	GameObjectBase() : m_objId(GAME_OBJECT_ID_UNKNOWN) {}
	inline void SetObjID(GameObjectId objID) { m_objId = objID;}
	
public:
//from IGameObject
	virtual GameObjectId GetObjID() { return m_objId;}
	
};


#endif //__GAME_OBJECT_BASE_H__