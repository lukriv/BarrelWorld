#ifndef __GAME_PHYSICS_COMPONENT_MANAGER_H__
#define __GAME_PHYSICS_COMPONENT_MANAGER_H__


#include "../GameSystem/gdefs.h"
#include "gcompmgrimpl.h"
#include "gobjdef.h"


struct PhysCompDef {
	GameObjectID m_geometryRef; //!< List of geometry objects - can be empty (dep. on type)
	GameObjectID m_physRef; //!< List of Joints or Bodys - depend on type
	b2Transform m_tranformation;
	
	RenderCompDef() :  m_geometryRef(GAME_OBJECT_ID_INVALID)
			, m_physRef(GAME_OBJECT_ID_INVALID){}
};


class GamePhysicsComponentMgr : public GameComponentMgrBase {
	
// IGameComponent interface
public:
	
	GameErrorCode CreatePhysicsComponent(const PhysCompDef &entityDef, GameObjectId &compId);
	
	GameErrorCode DeletePhysicsComponent(GameObjectID compId);
	
	GameErrorCode GetPhysicsComponent(GameObjectID compID);
	
	GameErrorCode StepPhysics();
	

	
};

#endif //__GAME_PHYSICS_COMPONENT_MANAGER_H__