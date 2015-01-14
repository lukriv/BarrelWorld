#include "gxmlloader.h"


GameErrorCode GameXmlResourceLoader::AddAttrValue(wxXmlNode *pNode, const wxString &value)
{
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::AddKeyValue(wxXmlNode *pNode, const wxString &action, const wxInt32 &keyCode)
{
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::AddParameter(wxXmlNode* pNode, const ParamDefinition* pDefTable, wxInt32 tableSize, wxString& paramName, void* pObject)
{
	return FWG_NO_ERROR;
}



GameErrorCode GameXmlResourceLoader::ConstructScene(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::ConstructDefinitions(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
//	GameErrorCode result = FWG_NO_ERROR;
//	
//	wxXmlNode* pChild = nullptr;
//	
//	if(!defHolder.m_meshDefs.IsEmpty())
//	{
//		
//	}
//	if(child->GetName() == GAME_TAG_DEFS_MESHES_STR) 
//		{
//			SECOND_TAG_DEFINITION_CHECK(pMeshes,child);
//			pMeshes = child;
//		} else if(child->GetName() == GAME_TAG_DEFS_MATERIALS_STR) {
//			SECOND_TAG_DEFINITION_CHECK(pMaterials,child);
//			pMaterials = child;
//		} else if(child->GetName() == GAME_TAG_DEFS_INPUTS_STR) {
//			SECOND_TAG_DEFINITION_CHECK(pInputs,child);
//			pInputs = child;
//		} else if(child->GetName() == GAME_TAG_DEFS_LOGICS_STR) {
//			SECOND_TAG_DEFINITION_CHECK(pLogics,child);
//			pLogics = child;
//		} else if(child->GetName() == GAME_TAG_DEFS_CAMERAS_STR) {
//			SECOND_TAG_DEFINITION_CHECK(pCameras,child);
//			pCameras = child;
//		} else if(child->GetName() == GAME_TAG_DEFS_RENDER_ENTS_STR) {
//			SECOND_TAG_DEFINITION_CHECK(pRenderEnts,child);
//			pRenderEnts = child;
//		} else if(child->GetName() == GAME_TAG_DEFS_RENDER_STR) {
//			SECOND_TAG_DEFINITION_CHECK(pRenderObjs,child);
//			pRenderObjs = child;
//		} else {
//			// found unknown tag
//			FWGLOG_ERROR_FORMAT(wxT("Unknown tag ['%s'] on line: %d"),
//								m_spLogger,
//								child->GetName().GetData().AsInternal(),
//								child->GetLineNumber(),
//								FWGLOG_ENDVAL);
//			return FWG_E_XML_UNKNOWN_TAG_ERROR;
//		}
//		
//		child = child->GetNext();
//	}
//	
//	if(pMeshes)
//	{
//		if(FWG_FAILED(result = LoadMeshes(pMeshes, defHolder)))
//		{
//			FWGLOG_ERROR_FORMAT(wxT("Load meshes failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
//			return result;
//		}
//	} else {
//		FWGLOG_INFO(wxT("No mesh definitions found"), m_spLogger);
//	}
//	
//	if(pMaterials)
//	{
//		if(FWG_FAILED(result = LoadMaterials(pMaterials, defHolder)))
//		{
//			FWGLOG_ERROR_FORMAT(wxT("Load materials failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
//			return result;
//		}
//	} else {
//		FWGLOG_INFO(wxT("No material definitions found"), m_spLogger);
//	}
//	
//	if(pRenderEnts)
//	{
//		if(FWG_FAILED(result = LoadRenderEntities(pRenderEnts, defHolder)))
//		{
//			FWGLOG_ERROR_FORMAT(wxT("Load render entities failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
//			return result;
//		}
//	} else {
//		FWGLOG_INFO(wxT("No render entity definitions found"), m_spLogger);
//	}
//	
//	if(pCameras)
//	{
//		if(FWG_FAILED(result = LoadCameras(pCameras, defHolder)))
//		{
//			FWGLOG_ERROR_FORMAT(wxT("ParseDefinitions failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
//			return result;
//		}
//	} else {
//		FWGLOG_INFO(wxT("No camera definitions found"), m_spLogger);
//	}
//	
//	if(pRenderObjs)
//	{
//		if(FWG_FAILED(result = LoadRenderDef(pRenderObjs, defHolder)))
//		{
//			FWGLOG_ERROR_FORMAT(wxT("ParseDefinitions failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
//			return result;
//		}
//	} else {
//		FWGLOG_INFO(wxT("No render object definitions found"), m_spLogger);
//	}
//	
//	if(pInputs)
//	{
//		if(FWG_FAILED(result = LoadInput(pInputs, defHolder)))
//		{
//			FWGLOG_ERROR_FORMAT(wxT("Load inputs failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
//			return result;
//		}
//	} else {
//		FWGLOG_INFO(wxT("No input definitions found"), m_spLogger);
//	}
//	
//	if(pLogics)
//	{
//		if(FWG_FAILED(result = LoadLogic(pLogics, defHolder)))
//		{
//			FWGLOG_ERROR_FORMAT(wxT("Load logic failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
//			return result;
//		}
//	} else {
//		FWGLOG_INFO(wxT("No logic definitions found"), m_spLogger);
//	}
	return FWG_NO_ERROR;
}



GameErrorCode GameXmlResourceLoader::CreateNodeCamera(wxXmlNode* pNode, const wxString& name, const CameraDef* pCameraDef)
{
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreateNodeEntity(wxXmlNode* pNode, GameDefinitionHolder& defHolder, const wxString& name, const EntityDef* pEntity)
{
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreateNodeInput(wxXmlNode* pNode, const wxString& name, const InputDef* pInputDef)
{
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreateNodeLogic(wxXmlNode* pNode, const wxString& name, const LogicDef* pLogicDef)
{
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreateNodeMaterial(wxXmlNode* pNode, const wxString& name, const NameDef* spDef)
{
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreateNodeMesh(wxXmlNode* pNode, const wxString& name, const NameDef* pDef)
{
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreateNodePhysics(wxXmlNode* pNode, const PhysCompDef* pPhysics)
{
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreateNodePhysicsShape(wxXmlNode* pNode, const PhysShapeDef* pPhysicsShape)
{
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreateNodeRender(wxXmlNode* pNode, GameDefinitionHolder& defHolder, const wxString& name, const RenderDef* pDef)
{
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreateNodeRenderEntity(wxXmlNode* pNode, GameDefinitionHolder& defHolder, const wxString& name, const RenderEntityDef* pDef)
{
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreateNodeTransform(wxXmlNode* pNode, const TransformDef* pTransform)
{
	return FWG_NO_ERROR;
}

///////////////////////////////////
// Store definitions and entities
///////////////////////////////////

GameErrorCode GameXmlResourceLoader::StoreCameras(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
	return FWG_NO_ERROR;
}
GameErrorCode GameXmlResourceLoader::StoreInput(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::StoreLogic(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::StoreMaterials(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::StoreMeshes(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::StoreRenderDef(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::StoreRenderEntities(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
	return FWG_NO_ERROR;
}


///////////////////////////////
// Conversions
///////////////////////////////

GameErrorCode GameXmlResourceLoader::ConvertFromQuat(const Ogre::Quaternion& resultQuat, wxString& output)
{
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::ConvertFromShapeType(wxInt32 retType, wxString& output)
{
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::ConvertFromVec3(const Ogre::Vector3 &resultVec, wxString& output)
{
	return FWG_NO_ERROR;
}
