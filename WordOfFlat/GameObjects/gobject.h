#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "gameobjdef.h"

class IGameObject {
public:	
	virtual GameObjectId GetObjID() = 0;
	virtual GameObjectType GetType() = 0;
		
	virtual ~IGameObject() {}
		
}


#endif //__GAME_DRAWABLE_OBJECT_H__