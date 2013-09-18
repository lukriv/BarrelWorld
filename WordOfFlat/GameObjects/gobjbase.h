#ifndef __GAME_OBJECT_BASE_H__
#define __GAME_OBJECT_BASE_H__

#include "gameobjdef.h"
#include "gobject.h"

class GameObjectBase : public IGameObject {
private:
	GameObjectID m_objId;

public:
	GameObjectBase() : m_objId(GAME_OBJECT_ID_INVALID) {}
	inline void SetObjID(GameObjectID objID) { m_objId = objID;}
	
public:
//from IGameObject
	virtual GameObjectID GetObjID() { return m_objId;}
	
};


#endif //__GAME_OBJECT_BASE_H__