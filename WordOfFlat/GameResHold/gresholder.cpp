#include "gresholder.h"

GameResourceHolder* GameResourceHolder::g_pResourceHolder = NULL;


IGameGeometry* GameResourceHolder::GetGeometry(GameShapeId geomID)
{
	TGameGeometryMap::iterator iter;
	GameErrorCode result = FWG_NO_ERROR;
	
	wxCriticalSectionLocker locker(m_resouceLocker);
	
	//find geometry
	iter = m_geomMap.find(geomID);
	if (iter == m_geomMap.end()) return NULL;
	if (iter->second.m_pGeometry == NULL)
	{
		if(FWG_FAILED(result = m_pResLoader->LoadGeometry(geomID, iter->second.m_pGeometry))){
			FWGLOG_ERROR_FORMAT(wxT("GameResourceHolder::GetGeometry() : Load geometry failed: 0x%08x"),
				m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		// increment reference counter
		iter->second.m_refCount++;
	}
	
	return iter->second.m_pGeometry;
}



sf::Texture* GameResourceHolder::GetTexture(GameTextureId texID)
{
}

GameErrorCode GameResourceHolder::Initialize(GameLogger* pLogger, IGameResourceLoader* pResLoader)
{
	GameErrorCode result = FWG_NO_ERROR;
	if (pLoader == NULL) {
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	if (FWG_FAILED(result = pResLoader->LoadTextureList(m_texMap))) {
		FWGLOG_ERROR_FORMAT(wxT("GameResourceHolder::Initialize(): Load texture map failed: 0x%08x"),
				pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if (FWG_FAILED(result = pResLoader->LoadGeometryList(m_geomMap))) {
		FWGLOG_ERROR_FORMAT(wxT("GameResourceHolder::Initialize(): Load geometry map failed: 0x%08x"),
				pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	m_spLogger = pLogger;
	
	m_isInitialized = true;
	
	return result;
}

void GameResourceHolder::ReleaseGeometry(GameShapeId geomID)
{
}

void GameResourceHolder::ReleaseTexture(GameTextureId texID)
{
}

GameErrorCode GameResourceHolder::SetLoader(IGameResourceLoader* pLoader)
{
}

void GameResourceHolder::addRef()
{
}

wxInt32 GameResourceHolder::release()
{
}

GameResourceHolder::~GameResourceHolder()
{
}


//----------- Static methods --------------//

GameResourceHolder* GameResourceHolder::GetResourceHolder()
{
	if (g_pResourceHolder != NULL){
		g_pResourceHolder->addRef();	
	}
	return g_pResourceHolder;
}

GameErrorCode GameResourceHolder::InitializeResourceHolder(GameLogger* pLogger)
{
	GameErrorCode result = FWG_NO_ERROR;
	if (g_pResourceHolder != NULL) {
		return FWG_NO_ERROR;
	}
	
	wxScopedPtr<GameResourceHolder> apResHolder;
	apResHolder.reset(new (std::nothrow) GameResourceHolder());
	
	if (FWG_FAILED(result = apResHolder->Initialize(pLogger)))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameResourceHolder::InitializeResourceHolder() : Resource holder initialization failed: 0x%08x"),
						pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	g_pResourceHolder = apResHolder.release();
	
	return result;
}


void GameResourceHolder::ReleaseResourceHolder()
{
	if(g_pResourceHolder != NULL) {
		g_pResourceHolder->release();
		g_pResourceHolder = NULL;
	}
}