#include "gphysicscontrolmgr.h"

#include <GameComp/transformComp/gtranscomp.h>
#include <GameComp/inputComp/ginputcomp.h>

PhysicsControllerManager::PhysicsControllerManager(GamePhysicsSystem* pPhysicsSystem, GameEntityManager* pEntityMgr) :
	GameManagerBase(pPhysicsSystem->GetLogger(), pEntityMgr),
	m_pPhysicsSystem(pPhysicsSystem)
{
}

PhysicsControllerManager::~PhysicsControllerManager()
{
}
