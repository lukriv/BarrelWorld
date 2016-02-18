#include "gphysicscontrolmgr.h"


PhysicsControllerManager::PhysicsControllerManager(m_pPhysicsSystem* pPhysicsSystem, GameEntityManager* pEntityMgr) :
	GameManagerBase(pPhysicsSystem->GetLogger(), pEntityMgr),
	m_pPhysicsSystem(pPhysicsSystem)
{
}

PhysicsControllerManager::~PhysicsControllerManager()
{
}
