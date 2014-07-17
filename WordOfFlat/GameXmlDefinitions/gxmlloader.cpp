#include "gxmlloader.h"

GameErrorCode GameXmlResourceLoader::Initialize(GameLogger* pLogger)
{
	if (m_isInitialized) {
		return FWG_NO_ERROR;
	}

	m_spLogger = pLogger;

	m_isInitialized = true;
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::Load(GameDefinitionHolder& defHolder)
{
	GameErrorCode result = FWG_NO_ERROR;
	if(!m_isInitialized)
	{
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	if(FWG_FAILED(result = LoadMeshes(defHolder)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Load mesh definitions failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = LoadMaterials(defHolder)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Load materials definitions failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = LoadRenderEntities(defHolder)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Load render entity definitions failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = LoadCameras(defHolder)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Load cameras definitions failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = LoadRenderDef(defHolder)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Load render definitions failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = LoadAnimations(defHolder)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Load animation definitions failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = LoadAnimators(defHolder)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Load animator definitions failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = LoadInput(defHolder)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Load input definitions failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = LoadLogic(defHolder)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Load logic definitions failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = LoadEntities(defHolder)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Load entity definitions failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
		
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::LoadAnimations(GameDefinitionHolder& defHolder)
{
}

GameErrorCode GameXmlResourceLoader::LoadAnimators(GameDefinitionHolder& defHolder)
{
}

GameErrorCode GameXmlResourceLoader::LoadCameras(GameDefinitionHolder& defHolder)
{
}

GameErrorCode GameXmlResourceLoader::LoadEntities(GameDefinitionHolder& defHolder)
{
}

GameErrorCode GameXmlResourceLoader::LoadInput(GameDefinitionHolder& defHolder)
{
}

GameErrorCode GameXmlResourceLoader::LoadLogic(GameDefinitionHolder& defHolder)
{
}

GameErrorCode GameXmlResourceLoader::LoadMaterials(GameDefinitionHolder& defHolder)
{
}

GameErrorCode GameXmlResourceLoader::LoadMeshes(GameDefinitionHolder& defHolder)
{
}

GameErrorCode GameXmlResourceLoader::LoadRenderDef(GameDefinitionHolder& defHolder)
{
}

GameErrorCode GameXmlResourceLoader::LoadRenderEntities(GameDefinitionHolder& defHolder)
{
}
