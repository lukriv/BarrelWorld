#ifndef __GAME_OBJ_H__
#define __GAME_OBJ_H__

#include "refobject.h"

enum GameObjectType {
	GAME_TYPE_TREE,
	GAME_TYPE_GROUND
};

class IGameObject : public IRefObject {
public:
	virtual GameObjectType GetType() = 0;
	virtual wxInt32 GetId() = 0;
	virtual bool IsStatic() = 0;
	virtual bool IsMoveable() = 0;
	virtual bool IsDestroyable() = 0;
	virtual bool IsDrawable() = 0;
	
	/*! \brief Draw an object
	 * 
	 * If object is not drawable, this method do nothing.
	 * 
	 * \retval true Drawing object was successful.
	 * \retval false Drawing object failed.
	 */
	virtual bool Draw() = 0;
}


#endif //__GAME_OBJ_H__