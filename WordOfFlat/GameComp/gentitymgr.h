#ifndef __GAME_ENTITY_MANAGER_H__
#define __GAME_ENTITY_MANAGER_H__

#include <GameSystem/gmap.h>
#include <GameSystem/memalloccont.h>
#include <GameSystem/glog.h>
#include <GameSystem/circularQueue.h>

#include "TransformComp/gtranscmgr.h"
#include "LogicComp/glogiccontrollermgr.h"
#include "PhysicsComp/gphyscompmgr.h"
#include "PhysicsComp/gphysicscontrolmgr.h"
#include "RenderComp/grenderpositionmgr.h"
#include "RenderComp/grenderobjmgr.h"
#include "PropertiesComp/gpropertymgr.h"

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
	LogicControllerManager	m_logicControllerMgr;
	PhysicsCompManager m_physicsMgr;
	PhysicsControllerManager m_physicsControllerMgr;
	RenderPositionManager m_renderPosMgr;
	RenderObjectManager m_renderObjMgr;
	PropertyCompManager m_propertyMgr;
	
public:
	GameEntityManager(GameLogger *pLogger, GameCompManager& compMgr);
	~GameEntityManager();
	
	inline GameLogger * GetLogger() { return m_spLogger; }
	
	wxDword GetNewId();
	
	void DestroyEntity(wxDword entityId);
	void DestroyAllEntities();
	
	GameErrorCode SendTaskMessage(wxDword receiverID, TaskMessage &msg);
	
	inline TransformCompManager& GetTransformManager() 	{ return m_tranformMgr; }
	inline LogicControllerManager& GetLogicControllerManager() 	{ return m_logicControllerMgr; }
	inline PhysicsCompManager& GetPhysicsManager() 		{ return m_physicsMgr; }
	inline PhysicsControllerManager& GetPhysicsControllerManager() { return m_physicsControllerMgr; }
	inline RenderPositionManager& GetRenderPosManager() { return m_renderPosMgr; }
	inline RenderObjectManager& GetRenderObjManager() 	{ return m_renderObjMgr; }
	inline PropertyCompManager& GetPropertyManager() {return m_propertyMgr; }
	
	
};


#endif //__GAME_ENTITY_MANAGER_H__