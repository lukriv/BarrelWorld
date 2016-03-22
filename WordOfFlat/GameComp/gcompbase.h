#ifndef __GAME_COMPONENT_BASE_H__
#define __GAME_COMPONENT_BASE_H__


#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>

#include "gcompdefs.h"

class GameEntityManager;

class TaskMessage {
	GameTaskMessageType m_taskType;
	void *m_context;
public:
	TaskMessage() : m_taskType(GAME_TASK_UNKNOWN), m_context( nullptr ) {}
	TaskMessage(GameTaskMessageType taskType) : m_taskType(taskType), m_context( nullptr ) {}
	TaskMessage(GameTaskMessageType taskType, void *context) : m_taskType(taskType), m_context( context ) {}
	TaskMessage( const TaskMessage& that)
	{
		m_taskType = that.m_taskType;
		m_context = that.m_context;
	}
	
	inline void SetContext(void* context) { m_context = context; }
	
	inline GameTaskMessageType GetTaskType() { return m_taskType; }
	inline void* GetContext() { return m_context; }
	
	TaskMessage &operator=(const TaskMessage& taskmsg)
	{
		if(&taskmsg != this)
		{
			m_taskType = taskmsg.m_taskType;
			m_context = taskmsg.m_context;
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
	
	virtual void Enable(bool enable) { m_enabled = enable; }
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