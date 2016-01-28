#include "gmoveablemgr.h"
#include "glogicsystem.h"

MoveableManager::MoveableManager(GameLogicSystem* pLogicSystem, GameEntityManager *pEntityMgr) : GameManagerBase(pLogicSystem->GetLogger(), pEntityMgr)
																				, m_pLogicSystem(pLogicSystem)
{
}

MoveableManager::~MoveableManager()
{
}


