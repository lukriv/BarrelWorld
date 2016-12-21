#include "subsystemmanager.h"


BW::SubsystemManager* BW::SubsystemManager::g_pResMgr = nullptr;

BW::SubsystemManager::SubsystemManager()
{
}

BW::SubsystemManager::~SubsystemManager()
{
}


BW::SubsystemManager& BW::SubsystemManager::GetSubsystemMgr()
{
	return *g_pResMgr;
}

void BW::SubsystemManager::Release()
{
	delete g_pResMgr;
	g_pResMgr = nullptr;
}
