#include "glogicterrdecal.h"

GameErrorCode LogicTerrainDecal::Initialize(GameTerrainManager* pTerrainManager)
{
	if(!pTerrainManager)
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	m_pTerrainMgr = pTerrainManager;
	
	
	return FWG_NO_ERROR;
}

LogicTerrainDecal::LogicTerrainDecal(GameLogicSystem *pLogicSystem) : LogicBase(pLogicSystem), m_pTerrainMgr(nullptr)
{
}

GameErrorCode LogicTerrainDecal::UserLogic()
{
	return FWG_NO_ERROR;
}

LogicTerrainDecal::~LogicTerrainDecal()
{
}

GameErrorCode LogicTerrainDecal::Load(wxXmlNode* pNode)
{
	return FWG_NO_ERROR;
}

GameErrorCode LogicTerrainDecal::Store(wxXmlNode* pParentNode)
{
	return FWG_NO_ERROR;
}
