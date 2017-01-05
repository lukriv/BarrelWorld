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
	g_pResMgr = nullptr;
}
