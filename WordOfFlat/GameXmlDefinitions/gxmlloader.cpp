#include "gxmlloader.h"
#include "gxmldefs.h"

static const wxChar* staticXmlFile = wxT("example.xml");

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
	wxXmlDocument xmlDoc;
	
	if(!m_isInitialized)
	{
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	if(!xmlDoc.Load(wxString(staticXmlFile)))
	{
		return FWG_E_OPEN_FILE_ERROR;
	}
	
	if(xmlDoc.GetRoot()->GetName() != GAME_TAG_ROOT_STR)
		return FWG_E_ROOT_NOT_FOUND_ERROR;
	
	//get root
	wxXmlNode* child = xmlDoc.GetRoot()->GetChildren();
	while(child)
	{
		if(child->GetName() == GAME_TAG_ROOT_STR) 
		{
			if(!ParseChapter(child, resMgr, sceneMgr))
			{
				return false;
			}
		} else {
			return false;
		}
		
		child = child->GetNext();
	}
	
	return true;
	
	
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

GameErrorCode GameXmlResourceLoader::LoadAnimations(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
}

GameErrorCode GameXmlResourceLoader::LoadAnimators(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
}

GameErrorCode GameXmlResourceLoader::LoadCameras(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
}

GameErrorCode GameXmlResourceLoader::LoadEntities(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
}

GameErrorCode GameXmlResourceLoader::LoadInput(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
}

GameErrorCode GameXmlResourceLoader::LoadLogic(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
}

GameErrorCode GameXmlResourceLoader::LoadMaterials(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
}

GameErrorCode GameXmlResourceLoader::LoadMeshes(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
}

GameErrorCode GameXmlResourceLoader::LoadRenderDef(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
}

GameErrorCode GameXmlResourceLoader::LoadRenderEntities(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
}
