#ifndef __GAME_OBJ_H__
#define __GAME_OBJ_H__

enum GameObjectType {
	GAME_TYPE_TREE
};

class IGameObject {
public:
	virtual GameObjectType GetType() = 0;
}

#endif //__GAME_OBJ_H__