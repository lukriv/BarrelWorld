#include "gphyscompmgr.h"

PhysicsCompManager::PhysicsCompManager(GamePhysicsSystem* pPhysicsSystem) : GameManagerBase(pPhysicsSystem->GetLogger())
																, m_pPhysSystem(pPhysicsSystem)
{
}

PhysicsCompManager::~PhysicsCompManager()
{
}
