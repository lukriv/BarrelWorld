#ifndef __GAME_PHYSICS_COMPONENT_MANAGER_H__
#define __GAME_PHYSICS_COMPONENT_MANAGER_H__

#include <bullet/btBulletDynamicsCommon.h>
#include <GameSystem/gdefs.h>
#include <GameSystem/gerror.h>
#include <GameComp/gcompbase.h>


class PhysicsCompManager {
	btConstraintSolver *m_pSolver;
	btBroadphaseInterface	*m_pBroadphase;
	btCollisionConfiguration *m_pCollisionConfig;
	btCollisionDispatcher	*m_pDispatcher;	
	btDynamicsWorld *m_pDynamicsWorld;

public:
	PhysicsCompManager();
	~PhysicsCompManager();
	
	GameErrorCode Initialize();
	void Uninitialize();
	

	
};

#endif //__GAME_PHYSICS_COMPONENT_MANAGER_H__