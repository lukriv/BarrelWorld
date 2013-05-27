#include "flatworldsrvimpl.h"



GameErrorCode FlatLocalWorldSrvImpl::Initialize(GameLogger* logger)
{
	GameErrorCode result = FWG_NO_ERROR;
	
	if(m_isInitialized)
	{
		return FWG_NO_ERROR;
	}
	
	//initialize gravity vector
	m_gravity.Set(0.0f, -10.0f);
	
	b2World* pWorld = new (std::nothrow) b2World(m_gravity);
	
	if (pWorld == NULL)
	{
		FWGLOG_ERROR(wxT("FlatLocalWorldSrv::Initialize() : New world creation failed"), logger );
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	m_apWorld.reset(pWorld);
	pWorld = NULL; //for sure
	
	// initialization was successful
	m_isInitialized = true;
		
	return result;	
}




GameFlatWorldID FlatLocalWorldSrvImpl::GetFWId()
{
	return m_wrldId;
}


wxInt32 FlatLocalWorldSrvImpl::release()
{
	wxInt32 refCount = wxAtomicDec(m_refCount);
	if (refCount == 0)
	{
		delete this;
	}
	
	return refCount;
}


void FlatLocalWorldSrvImpl::addRef()
{
	wxAtomicInc(m_refCount);
}

GameErrorCode FlatLocalWorldSrvImpl::AddNewObject(IGameObjectSrv* object)
{
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}

GameErrorCode FlatLocalWorldSrvImpl::LoadWorld(const wxChar* worldName)
{
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}

GameErrorCode FlatLocalWorldSrvImpl::SetWorldSize(const b2Vec2& LLpoint, const b2Vec2& RUpoint)
{
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}

GameErrorCode FlatLocalWorldSrvImpl::StepWorld()
{
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}


GameErrorCode FlatLocalWorldSrvImpl::GenerateTestStaticWorld()
{
	GameErrorCode result = FWG_NO_ERROR;
	if(!m_isInitialized)
	{
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	m_wrldId = GAME_FLAT_WRLD_TEST;
	
	b2BodyDef bodyDef;
	bodyDef.position.Set(0.0f, -10.0f);
	
	
	
	return result;
}
