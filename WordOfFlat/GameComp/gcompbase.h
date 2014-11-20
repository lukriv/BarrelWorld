#ifndef __GAME_COMPONENT_BASE_H__
#define __GAME_COMPONENT_BASE_H__


enum GameEntityComponent {
	ENTITY_COMP_TRANSFORM				= 1<<0,
	ENTITY_COMP_RENDER		 			= 1<<1,
	ENTITY_COMP_LOGIC 					= 1<<2,
	ENTITY_COMP_INPUT 					= 1<<3,
	ENTITY_COMP_PHYSICS					= 1<<4,
	ENTITY_COMP_ALL						= wxUINT32_MAX;
};

class TaskMessage {
	wxDword m_taskType;
public:
	
	
};

class ComponentBase : public RefObjectImpl<IRefObject> {
	GameEntityComponent m_compType;
public:
	ComponentBase(GameEntityComponent compType) : m_compType(compType) {}
	
	virtual GameErrorCode Update() = 0;
	virtual GameErrorCode ReceiveMessage(TaskMessage &msg) = 0;
	
};


#endif // __GAME_COMPONENT_BASE_H__