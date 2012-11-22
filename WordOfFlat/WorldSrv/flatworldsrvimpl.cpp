#include "flatworldsrvimpl.h"

#include <wx/log.h>

GameErrorCode FlatLocalWorldSrvImpl::Initialize()
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
		wxLogError(wxT("FlatLocalWorldSrv::Initialize() : New world creation failed"));
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	m_apWorld.reset(pWorld);
	pWorld = NULL; //for sure
	
	// initialization was successful
	m_isInitialized = true;
		
	return result;	
}

GameErrorCode FlatLocalWorldSrvImpl::GenerateTestStaticWorld()
{
	GameErrorCode result = FWG_NO_ERROR;
	if(!m_isInitialized)
	{
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	
	
	return result;
}
