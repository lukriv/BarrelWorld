#include "gflatworldsrvimpl.h"



GameErrorCode GameFlatWorldSrv::Initialize(GameLogger* logger)
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




GameFlatWorldID GameFlatWorldSrv::GetFWId()
{
	return m_wrldId;
}


wxInt32 GameFlatWorldSrv::release()
{
	wxInt32 refCount = wxAtomicDec(m_refCount);
	if (refCount == 0)
	{
		delete this;
	}
	
	return refCount;
}


void GameFlatWorldSrv::addRef()
{
	wxAtomicInc(m_refCount);
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
	
	m_wrldId = GAME_FLAT_WRLD_TEST;
	
	b2BodyDef bodyDef;
	
	
	
	
	return result;
}
GameErrorCode GameFlatWorldSrv::CreateNewObject(b2BodyDef &bodyDef, IGameObjectSrv *&pObject)
{
	GameErrorCode result = FWG_NO_ERROR;
	b2Body *pBody = nullptr;
	pBody = m_apWorld->CreateBody(&bodyDef);
	
	wxScopedPtr<GameObjectSrv> apObject;
	apObject.reset(new (std::nothrow) GameObjectSrv(this, pBody));
	if ((apObject.get() == nullptr)||(pBody == nullptr))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameFlatWorldSrv::CreateNewObject() : Create new object failed: 0x%08x"),
			m_spLogger, FWG_E_MEMORY_ALLOCATION_ERROR, FWGLOG_ENDVAL);
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	pObject = apObject.release();
	
	return FWG_NO_ERROR;
}

GameErrorCode GameFlatWorldSrv::AddNewObject(GameObjectId objID, IGameObjectSrv* pObject)
{
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}
