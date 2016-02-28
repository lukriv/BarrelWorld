#include "glogiccontrollermgr.h"
#include "glogicsystem.h"

LogicControllerManager::LogicControllerManager(GameLogicSystem* pLogicSystem, GameEntityManager *pEntityMgr) : GameManagerBase(pLogicSystem->GetLogger(), pEntityMgr)
																				, m_pLogicSystem(pLogicSystem)
{
}

LogicControllerManager::~LogicControllerManager()
{
}


