#include "ginputcompmgr.h"

InputComponentManager::InputComponentManager(GameInputSystem* pInputSystem, GameEntityManager *pEntityMgr) : GameManagerBase(pInputSystem->GetLogger(), pEntityMgr),
																		m_pInputSystem(pInputSystem)
{
}

InputComponentManager::~InputComponentManager()
{
}



