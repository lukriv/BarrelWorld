#ifndef __GAME_SERVER_OBJECT_IMPL_H__
#define __GAME_SERVER_OBJECT_IMPL_H__

#include "gamesrvobj.h"

class GameObjectSrvImpl : public IGameObjectSrv {
private:
	b2BodyDef m_bodyDef;
public:
	virtual GameObjectType GetType() = 0;
	virtual wxInt32 GetId() = 0;
}

#endif //__GAME_SERVER_OBJECT_IMPL_H__