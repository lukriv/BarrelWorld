#ifndef __GAME_SERVER_OBJECT_H__
#define __GAME_SERVER_OBJECT_H__

#include "../GameSystem/refobject.h"
#include "gameobjdef.h"
#include "gamedrawobj.h"

class IGameObjectSrv : public IRefObject {
public:
	virtual GameObjectType GetType() = 0;
	virtual wxInt32 GetId() = 0;
	virtual void SetId(wxInt32 id) = 0;
	virtual void SetDrawableObj(IGameObject *pDrawableObj) = 0;
}

#endif //__GAME_SERVER_OBJECT_H__
