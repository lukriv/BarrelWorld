#include "gmoveablemgr.h"
#include "glogicsystem.h"

MoveableManager::MoveableManager(GameLogicSystem* pLogicSystem) : GameManagerBase(pLogicSystem->GetLogger())
																				, m_pLogicSystem(pLogicSystem)
{
}

MoveableManager::~MoveableManager()
{
}


