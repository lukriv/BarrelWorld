#include "glogicterrdecal.h"

GameErrorCode LogicTerrainDecal::Initialize(GameTerrainManager* pTerrainManager, GameEntity* pEntity)
{
	if((!pTerrainManager)||(!pEntity))
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	m_spInput = pEntity->GetComponent(GAME_COMP_INPUT);
	
	if(m_spInput.IsEmpty())
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	m_pTerrainMgr = pTerrainManager;
	m_pParent = pEntity;
	
	
	return FWG_NO_ERROR;
}

LogicTerrainDecal::LogicTerrainDecal() : m_pTerrainMgr(nullptr), m_pEntity(nullptr)
{
}

GameErrorCode LogicTerrainDecal::ReceiveMessage(TaskMessage& msg)
{
	return FWG_NO_ERROR;
}

GameErrorCode LogicTerrainDecal::ReinitComponent(GameEntity* pNewParentEntity)
{
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}

GameErrorCode LogicTerrainDecal::Update()
{
	return FWG_NO_ERROR;
}

GameErrorCode LogicTerrainDecal::UserLogic()
{
}

LogicTerrainDecal::~LogicTerrainDecal()
{
}

