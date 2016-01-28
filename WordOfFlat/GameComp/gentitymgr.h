#ifndef __GAME_ENTITY_MANAGER_H__
#define __GAME_ENTITY_MANAGER_H__

#include <GameSystem/gmap.h>
#include <GameSystem/memalloccont.h>
#include <GameSystem/glog.h>
#include <GameSystem/circularQueue.h>

#include "TransformComp/gtranscmgr.h"
#include "InputComp/ginputcompmgr.h"
#include "LogicComp/gmoveablemgr.h"
#include "PhysicsComp/gphyscompmgr.h"
#include "RenderComp/grenderpositionmgr.h"
#include "RenderComp/grenderobjmgr.h"

// forward declaration
class GameCompManager;

class GameEntityManager {
	//typedef GameBasMap<wxDword, GameEntity> TEntityMap;
private:
	GameLoggerPtr m_spLogger;
	
	wxCriticalSection m_mgrLock;
	GameCircularQueue<wxDword> m_freeIds;
	wxDword m_nextFreeId;
	
	// component managers
	TransformCompManager m_tranformMgr;
	InputComponentManager m_inputCompMgr;
	MoveableManager	m_moveableCompMgr;
	PhysicsCompManager m_physicsCompMgr;
	RenderPositionManager m_renderPosMgr;
	RenderObjectManager m_renderObjMgr;
	
public:
	GameEntityManager(GameLogger *pLogger, GameCompManager& compMgr);
	~GameEntityManager();
	
	inline GameLogger * GetLogger() { return m_spLogger; }
	
	wxDword GetNewId();
	
	void DestroyEntity(wxDword entityId);
	void DestroyAllEntities();
	
	GameErrorCode SendTaskMessage(wxDword receiverID, TaskMessage &msg);
	
	inline TransformCompManager& GetTransformManager() 	{ return m_tranformMgr; }
	inline InputComponentManager& GetInputManager() 	{ return m_inputCompMgr; }
	inline MoveableManager& GetMoveableManager() 		{ return m_moveableCompMgr; }
	inline PhysicsCompManager& GetPhysicsManager() 		{ return m_physicsCompMgr; }
	inline RenderPositionManager& GetRenderPosManager() { return m_renderPosMgr; }
	inline RenderObjectManager& GetRenderObjManager() 	{ return m_renderObjMgr; }
	
	
};


#endif //__GAME_ENTITY_MANAGER_H__