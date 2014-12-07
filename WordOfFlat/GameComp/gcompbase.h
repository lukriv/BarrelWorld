#ifndef __GAME_COMPONENT_BASE_H__
#define __GAME_COMPONENT_BASE_H__


#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>

#define GAME_COMP_COUNT 5

enum GameComponentType {
	GAME_COMP_TRANSFORM					= 0,
	GAME_COMP_RENDER		 			= 1,
	GAME_COMP_LOGIC 					= 2,
	GAME_COMP_INPUT 					= 3,
	GAME_COMP_PHYSICS					= 4,
};



enum GameComponentMaskType {
	GAME_COMP_MASK_TRANSFORM			= 1<<GAME_COMP_TRANSFORM,
	GAME_COMP_MASK_RENDER		 		= 1<<GAME_COMP_RENDER,
	GAME_COMP_MASK_LOGIC 				= 1<<GAME_COMP_LOGIC,
	GAME_COMP_MASK_INPUT 				= 1<<GAME_COMP_INPUT,
	GAME_COMP_MASK_PHYSICS				= 1<<GAME_COMP_PHYSICS,
	GAME_COMP_MASK_ALL					= wxUINT32_MAX,
};

class TaskMessage {
	wxDword m_taskType;
public:
	TaskMessage(wxDword taskType) : m_taskType(taskType) {}
	inline wxDword GetTaskType() { return m_taskType; }
	
};


class GameEntity; // forward declaration of GameEntity

class ComponentBase : public RefObjectImpl<IRefObject> {
	GameComponentType m_compType;
	bool m_enabled;
public:
	ComponentBase(GameComponentType compType) : m_compType(compType), m_enabled(true) {}
	
	inline GameComponentType GetComponentType() { return m_compType; }
	
	inline void Enable(bool enable) { m_enabled = enable; }
	inline bool IsEnabled() { return m_enabled; }
	
	
	virtual GameErrorCode ReinitComponent(GameEntity *pNewParentEntity) = 0;
	virtual GameErrorCode Update() = 0;
	virtual GameErrorCode ReceiveMessage(TaskMessage &msg) = 0;
	
};


#endif // __GAME_COMPONENT_BASE_H__