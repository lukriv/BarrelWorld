#ifndef __GAME_OBJECT_DEFINITIONS_H__
#define __GAME_OBJECT_DEFINITIONS_H__

#include "../GameSystem/gdefs.h"

typedef wxDword GameTextureId; //!< Texture id type
typedef wxDword GameShapeId; //!< Shape id type
typedef wxDword GamePhysObjId; //!< Physics object id type
typedef wxDword GameObjectId; //!< Object id type


static const float Pixelize = 50.0f; //!< Conversion from meters to pixels (one meter is 50 pixels)


static const GameTextureId GAME_TEXTURE_ID_INVALID = UINT_MAX;
static const GameShapeId GAME_SHAPE_ID_INVALID = UINT_MAX;
static const GamePhysObjId GAME_PHYSICS_ID_INVALID = UINT_MAX;
static const GameObjectId GAME_OBJECT_ID_INVALID = UINT_MAX;


enum GameObjectType {
	GAME_OBJECT_TYPE_UNKNOWN = 					0,
	GAME_OBJECT_TYPE_BASIC_ENTITY = 			1,
	GAME_OBJECT_TYPE_EFECT_ENTITY = 			5,
	GAME_OBJECT_TYPE_ENTITY_GROUP = 			16,
	GAME_OBJECT_TYPE_KD_TREE = 					17,
	GAME_OBJECT_TYPE_BVH_TREE = 				18
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