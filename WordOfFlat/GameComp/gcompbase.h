#ifndef __GAME_COMPONENT_BASE_H__
#define __GAME_COMPONENT_BASE_H__


#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>

#include "gcompdefs.h"

class TaskMessage {
	GameTaskMessageType m_taskType;
public:
	TaskMessage(GameTaskMessageType taskType) : m_taskType(taskType) {}
	inline GameTaskMessageType GetTaskType() { return m_taskType; }
	
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