#ifndef __GAME_OBJECT_DEFINITIONS_H__
#define __GAME_OBJECT_DEFINITIONS_H__

enum GameObjectType {
	GAME_OBJECT_TYPE_UNKNOWN = 0,
	GAME_OBJECT_TYPE_BOX = 1,
	GAME_OBJECT_TYPE_GROUND = 2
};

typedef wxDword GameObjectId;

static const GameObjectId GAME_OBJECT_ID_UNKNOWN = 0;


enum GameActionCode {
	GAME_ACTION_NOTHING = 0
};

enum GameObjectFlags {
	GAME_OBJECT_FLAG_NOTHING 	 =	0,
	GAME_OBJECT_FLAG_DESTROYABLE = (1<<0),
	GAME_OBJECT_FLAG_MOVEABLE    = (1<<1)
};

enum GameObjectState {
	GAME_OBJECT_STATE_UNKNOWN = 0
};


struct GameObjStateStruct {
	GameObjectState m_state;
};


#endif //__GAME_OBJECT_DEFINITIONS_H__