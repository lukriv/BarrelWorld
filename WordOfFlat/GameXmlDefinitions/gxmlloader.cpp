#include "gxmlloader.h"
#include "gxmldefs.h"

static const wxChar* staticXmlFile = wxT("example.xml");


#define SECOND_TAG_DEFINITION(test,child) if((test) != nullptr) \
			{ \
				FWGLOG_ERROR_FORMAT(wxT("Tag ['%s'] on line %d was already defined"), \
								m_spLogger, \
								(child)->GetName().GetData(), \
								(child)->GetLineNumber(), \
								FWGLOG_ENDVAL); \
				return FWG_E_XML_INVALID_TAG_ERROR; \
			}

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
	
	wxXmlNode *pDefinitions = nullptr;
	wxXmlNode *pScene = nullptr;
	
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
		if(child->GetName() == GAME_TAG_DEFINITIONS_STR) 
		{
			pDefinitions = child;
		} else if(child->GetName() == GAME_TAG_SCENE_STR) {
			pScene = child;
		} else {
			// found unknown tag
			FWGLOG_ERROR_FORMAT(wxT("Unknown tag ['%s'] on line: %d"),
								m_spLogger,
								child->GetName().GetData(),
								child->GetLineNumber(),
								FWGLOG_ENDVAL);
			return FWG_E_XML_UNKNOWN_TAG_ERROR;
		}
		
		child = child->GetNext();
	}
	
	if(FWG_FAILED(result = ParseDefinitions(pDefinitions, defHolder)))
	{
		FWGLOG_ERROR_FORMAT(wxT("ParseDefinitions failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = ParseScene(pScene, defHolder)))
	{
		FWGLOG_ERROR_FORMAT(wxT("ParseScene failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
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

GameErrorCode GameXmlResourceLoader::ParseDefinitions(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
	GameErrorCode result = FWG_NO_ERROR;
	
	wxXmlNode* pMeshes = nullptr;
	wxXmlNode* pMaterials = nullptr;
	wxXmlNode* pInputs = nullptr;
	wxXmlNode* pLogics = nullptr;
	wxXmlNode* pCameras = nullptr;
	wxXmlNode* pRenderEnts = nullptr;
	wxXmlNode* pRenderObjs = nullptr;
	
	wxXmlNode* child = pNode->GetChildren();
	while(child)
	{
		if(child->GetName() == GAME_TAG_DEFS_MESHES_STR) 
		{
			SECOND_TAG_DEFINITION_CHECK(pMeshes,child);
			pMeshes = child;
		} else if(child->GetName() == GAME_TAG_DEFS_MATERIALS_STR) {
			SECOND_TAG_DEFINITION_CHECK(pMaterials,child);
			pMaterials = child;
		} else if(child->GetName() == GAME_TAG_DEFS_INPUTS_STR) {
			SECOND_TAG_DEFINITION_CHECK(pInputs,child);
			pInputs = child;
		} else if(child->GetName() == GAME_TAG_DEFS_LOGICS_STR) {
			SECOND_TAG_DEFINITION_CHECK(pLogics,child);
			pLogics = child;
		} else if(child->GetName() == GAME_TAG_DEFS_CAMERAS_STR) {
			SECOND_TAG_DEFINITION_CHECK(pCameras,child);
			pCameras = child;
		} else if(child->GetName() == GAME_TAG_DEFS_RENDER_ENTS_STR) {
			SECOND_TAG_DEFINITION_CHECK(pRenderEnts,child);
			pRenderEnts = child;
		} else if(child->GetName() == GAME_TAG_DEFS_RENDER_STR) {
			SECOND_TAG_DEFINITION_CHECK(pRenderObjs,child);
			pRenderObjs = child;
		} else {
			// found unknown tag
			FWGLOG_ERROR_FORMAT(wxT("Unknown tag ['%s'] on line: %d"),
								m_spLogger,
								child->GetName().GetData(),
								child->GetLineNumber(),
								FWGLOG_ENDVAL);
			return FWG_E_XML_UNKNOWN_TAG_ERROR;
		}
		
		child = child->GetNext();
	}
	
	
}

GameErrorCode GameXmlResourceLoader::ParseScene(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
}

GameErrorCode GameXmlResourceLoader::CreateMesh(wxXmlNode* pNode, wxString& name, RefObjSmPtr<NameDef>& spDef)
{
}
