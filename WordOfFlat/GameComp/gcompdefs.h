#ifndef __GAME_COMPONENT_DEFINITIONS_H__
#define __GAME_COMPONENT_DEFINITIONS_H__


enum GameComponentType {
	GAME_COMP_UNDEFINED					= 0,
	GAME_COMP_TRANSFORM					= 1,
	GAME_COMP_RENDER_POSITION			= 2,
	GAME_COMP_RENDER_OBJECT				= 3,
	GAME_COMP_PHYSICS					= 4,
	GAME_COMP_INPUT						= 5,
	GAME_COMP_MOVEABLE					= 6,
	GAME_COMP_PHYSICS_CONTROLER			= 7
};



enum GameTaskMessageType {
	GAME_TASK_UNKNOWN				= 0,
	GAME_TASK_TRANSFORM_UPDATE		= 1
};




#endif // __GAME_COMPONENT_DEFINITIONS_H__