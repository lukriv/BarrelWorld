#ifndef __GAME_SERVER_OBJECT_H__
#define __GAME_SERVER_OBJECT_H__

#include "../GameSystem/refobject.h"
#include "gameobjdef.h"

class IGameObjectSrv : public IRefObject {
public:
	virtual GameObjectType GetType() = 0;
	virtual wxInt32 GetId() = 0;
}

#endif //__GAME_SERVER_OBJECT_H__
