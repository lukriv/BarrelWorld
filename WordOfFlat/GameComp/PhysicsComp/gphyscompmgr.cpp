#include "gphyscompmgr.h"

PhysicsCompManager::PhysicsCompManager(GamePhysicsSystem* pPhysicsSystem, GameEntityManager *pEntityMgr) : GameManagerBase(pPhysicsSystem->GetLogger(), pEntityMgr)
																, m_pPhysSystem(pPhysicsSystem)
{
}

PhysicsCompManager::~PhysicsCompManager()
{
}
