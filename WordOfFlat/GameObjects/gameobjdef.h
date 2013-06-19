#ifndef __GAME_OBJECT_DEFINITIONS_H__
#define __GAME_OBJECT_DEFINITIONS_H__



typedef wxDword GameTextureId; //!< Texture id type
typedef wxDword GameShapeId; //!< Shape id type
typedef wxDword GameObjectId; //!< Object id type



static const GameTextureId GAME_TEXTURE_ID_UNKNOWN = UINT_MAX;
static const GameShapeId GAME_SHAPE_ID_UNKNOWN = UINT_MAX;
static const GameObjectId GAME_OBJECT_ID_UNKNOWN = UINT_MAX;


enum GameObjectType {
	GAME_OBJECT_TYPE_UNKNOWN = 			0,
	GAME_OBJECT_TYPE_STATIC_ENTITY = 	1,
	GAME_OBJECT_TYPE_DYNAMIC_ENTITY = 	2,
	GAME_OBJECT_TYPE_KINEMATIC_ENTITY = 3,
	GAME_OBJECT_TYPE_SENZOR_ENTITY = 	4,
	GAME_OBJECT_TYPE_ENTITY_GROUP = 	5,
	GAME_OBJECT_TYPE_KD_TREE = 			6,
	GAME_OBJECT_TYPE_BVH_TREE = 		7
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