#include "ginputcompmgr.h"

InputComponentManager::InputComponentManager(GameInputSystem* pInputSystem) : GameManagerBase(pInputSystem->GetLogger()),
																		m_pInputSystem(pInputSystem)
{
}

InputComponentManager::~InputComponentManager()
{
}



