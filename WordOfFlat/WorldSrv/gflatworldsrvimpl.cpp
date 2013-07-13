#include "gflatworldsrvimpl.h"



GameErrorCode GameFlatWorldSrv::Initialize(GameLogger* logger)
{
	FWG_UNREFERENCED_PARAMETER(logger);
	return FWG_E_NOT_IMPLEMENTED_ERROR;	
}




GameFlatWorldID GameFlatWorldSrv::GetFWId()
{
	return m_wrldId;
}

GameErrorCode GameFlatWorldSrv::LoadWorld(const wxChar* worldName)
{
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}

GameErrorCode GameFlatWorldSrv::SetWorldSize(const b2Vec2& LLpoint, const b2Vec2& RUpoint)
{
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}

GameErrorCode GameFlatWorldSrv::StepWorld()
{
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}


GameErrorCode GameFlatWorldSrv::GenerateTestStaticWorld()
{
	GameErrorCode result = FWG_NO_ERROR;
	if(!m_isInitialized)
	{
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	m_wrldId = 0;
	
	b2BodyDef bodyDef;
	
	
	
	
	return result;
}
GameErrorCode GameFlatWorldSrv::CreateNewObject(b2BodyDef &bodyDef, IGameObject *&pObject)
{
	FWG_UNREFERENCED_PARAMETER(bodyDef);
	FWG_UNREFERENCED_PARAMETER(pObject);
	return FWG_NO_ERROR;
}

