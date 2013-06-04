#ifndef __GAME_OBJECT_DEFINITIONS_H__
#define __GAME_OBJECT_DEFINITIONS_H__

enum GameObjectType {
	GAME_OBJECT_TYPE_UNKNOWN = 0,
	GAME_OBJECT_TYPE_BOX = 1,
	GAME_OBJECT_TYPE_GROUND = 2
};

typedef wxDword GameObjectId;

static const GameObjectId GAME_OBJECT_ID_UNKNOWN = 0;

#endif //__GAME_OBJECT_DEFINITIONS_H__