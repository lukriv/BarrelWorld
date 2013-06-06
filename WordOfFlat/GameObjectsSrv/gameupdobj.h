#ifndef __GAME_UPDATEABLE_OBJECT_H__
#define __GAME_UPDATEABLE_OBJECT_H__

#include <b2Math.h>
#include "refobject.h"
#include "../GameObjectsSrv/gameobjdef.h"

class IGameObjectSrv;

class IGameObject : public IRefObject {
public:	
	virtual GameObjectId GetObjID() = 0;
	virtual GameObjectType GetType() = 0;
	virtual IGameObjectSrv* GetSrvObj() = 0;
	
	virtual void UpdateObject(const b2Transform &mat) = 0;
	virtual void UpdateObject(const b2Transform &mat, const GameObjStateStruct &state) = 0;
	
	virtual b2Vec2 GetMiddlePoint() = 0;
	virtual b2AABB GetAABB() = 0;
	
	
	virtual ~IGameObject() {}
		
}


#endif //__GAME_DRAWABLE_OBJECT_H__