#ifndef __GAME_PHYSICS_SYSTEM_H__
#define __GAME_PHYSICS_SYSTEM_H__

#include <bullet/btBulletDynamicsCommon.h>
#include <GameSystem/gdefs.h>
#include <GameSystem/gerror.h>
#include <GameSystem/glog.h>
#include <GameComp/gcompbase.h>

struct PhysCompDef;
struct PhysShapeDef;

class GamePhysicsSystem {
	btConstraintSolver *m_pSolver;
	btBroadphaseInterface	*m_pBroadphase;
	btCollisionConfiguration *m_pCollisionConfig;
	btCollisionDispatcher	*m_pDispatcher;	
	btDynamicsWorld *m_pDynamicsWorld;
	
	GameLoggerPtr m_spLogger;

public:
	GamePhysicsSystem(GameLogger* pLogger);
	~GamePhysicsSystem();
	
	inline btDynamicsWorld* GetDynamicsWorld() { return m_pDynamicsWorld; }
	inline GameLogger* GetLogger() { return m_spLogger; }
	
	GameErrorCode Initialize();
	void Uninitialize();
	
	GameErrorCode ProcessPhysics();
	
protected:
	btCollisionShape * CreateCollisionShape(PhysShapeDef &physShapeDef);
	
};

#endif //__GAME_PHYSICS_SYSTEM_H__