#ifndef __GAME_COMPONENT_BASE_H__
#define __GAME_COMPONENT_BASE_H__


#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>

#include "gcompdefs.h"

class GameEntityManager;

class TaskMessage {
	GameTaskMessageType m_taskType;
public:
	TaskMessage() : m_taskType(GAME_TASK_UNKNOWN) {}
	TaskMessage(GameTaskMessageType taskType) : m_taskType(taskType) {}
	TaskMessage( const TaskMessage& that)
	{
		m_taskType = that.m_taskType;
	}
	
	inline GameTaskMessageType GetTaskType() { return m_taskType; }
	
	TaskMessage &operator=(const TaskMessage& taskmsg)
	{
		if(&taskmsg != this)
		{
			m_taskType = taskmsg.m_taskType;
		}
		
		return *this;
	}
};

class wxXmlNode; // forward declaration

class ComponentBase : public RefObjectImpl<IRefObject> {
	GameComponentType m_compType;
	GameEntityManager *m_pEntytyMgr;
	wxDword m_entityID;
	bool m_enabled;
public:
	ComponentBase(GameComponentType compType) : m_compType(compType)
											, m_pEntytyMgr(nullptr)
											, m_entityID(0)
											, m_enabled(true) {}
	
	inline GameComponentType GetComponentType() { return m_compType; }
	
	inline void Enable(bool enable) { m_enabled = enable; }
	inline bool IsEnabled() { return m_enabled; }
	
	inline void SetParentEntity(wxDword id) 
	{
		m_entityID = id;
	}
	
	inline wxDword GetComponentId() { return m_entityID; }
	
	inline void SetEntityManager(GameEntityManager *pEntityMgr)
	{
		m_pEntytyMgr = pEntityMgr;
	}
	
	inline GameEntityManager* GetEntityManager() { return m_pEntytyMgr; }
	
	virtual GameErrorCode ReceiveMessage(TaskMessage &msg) = 0;
	
	virtual GameErrorCode Load(wxXmlNode *pNode) = 0;
	virtual GameErrorCode Store(wxXmlNode *pParentNode) = 0;

	
};


#endif // __GAME_COMPONENT_BASE_H__