#ifndef __GAME_SERVER_OBJECT_H__
#define __GAME_SERVER_OBJECT_H__

#include "../GameSystem/refobject.h"
#include "gameobjdef.h"
#include "gamedrawobj.h"

class IGameObjectSrv : public IRefObject {
public:
	virtual GameObjectType GetObjType() = 0;
	virtual GameObjectId GetObjId() = 0;
	virtual void SetDrawableObj(IGameObject *pDrawableObj) = 0;
}

#endif //__GAME_SERVER_OBJECT_H__
