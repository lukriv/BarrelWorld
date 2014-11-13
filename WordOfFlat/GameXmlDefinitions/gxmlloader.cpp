#include "gxmlloader.h"
#include "gxmldefs.h"
#include "gxmlkeydefs.h"
#include <wx/filename.h>
#include <wx/tokenzr.h>
#include <wx/arrstr.h> 
#include <GameSystem/new.h>

//static const wxChar* staticXmlFile = wxT("example.xml");


#define SECOND_TAG_DEFINITION_CHECK(test,child) if((test) != nullptr) \
			{ \
				FWGLOG_ERROR_FORMAT(wxT("Tag ['%s'] on line %d was already defined"), \
								m_spLogger, \
								(child)->GetName().GetData().AsInternal(), \
								(child)->GetLineNumber(), \
								FWGLOG_ENDVAL); \
				return FWG_E_XML_INVALID_TAG_ERROR; \
			}

GameErrorCode GameXmlResourceLoader::Initialize(const wxChar* xmlFileName, const wxChar* xmlFilePath, GameLogger* pLogger)
{
	wxFileName normFileName;
	
	if (m_isInitialized) {
		return FWG_NO_ERROR;
	}
	
	if (xmlFileName == nullptr)
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	if (xmlFilePath == nullptr)
	{
		normFileName.Assign(xmlFileName);
	} else {
		normFileName.Assign(xmlFilePath, xmlFileName);
	}
	
	m_fileName = normFileName.GetFullPath();

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
	
	if(!xmlDoc.Load(m_fileName))
	{
		return FWG_E_OPEN_FILE_ERROR;
	}
	
	if(xmlDoc.GetRoot()->GetName() != GAME_TAG_ROOT_STR)
		return FWG_E_XML_ROOT_NOT_FOUND_ERROR;
	
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
								child->GetName().GetData().AsInternal(),
								child->GetLineNumber(),
								FWGLOG_ENDVAL);
			return FWG_E_XML_UNKNOWN_TAG_ERROR;
		}
		
		child = child->GetNext();
	}
	
	if(pDefinitions)
	{
		if(FWG_FAILED(result = ParseDefinitions(pDefinitions, defHolder)))
		{
			FWGLOG_ERROR_FORMAT(wxT("ParseDefinitions failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
	} else {
		FWGLOG_INFO(wxT("No definitions found"), m_spLogger);
	}
	
	if(pScene)
	{
		if(FWG_FAILED(result = ParseScene(pScene, defHolder)))
		{
			FWGLOG_ERROR_FORMAT(wxT("ParseScene failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
	} else {
		FWGLOG_INFO(wxT("No scene found"), m_spLogger);
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::LoadAnimations(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}

GameErrorCode GameXmlResourceLoader::LoadAnimators(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}

GameErrorCode GameXmlResourceLoader::LoadCameras(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxXmlNode* child = pNode->GetChildren();
	while(child)
	{
		if(child->GetName() == GAME_TAG_COMP_CAMERA) 
		{
			wxString cameraName;
			RefObjSmPtr<CameraDef> spCamera;
			
			if(FWG_FAILED(result = CreateCamera(child, cameraName, spCamera)))
			{
				// found unknown tag
				FWGLOG_ERROR_FORMAT(wxT("Create render entity failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}

			if(FWG_FAILED(result = defHolder.InsertDef<CameraDef>( cameraName, spCamera, defHolder.m_cameraDefs )))
			{
				FWGLOG_ERROR_FORMAT(wxT("Add camera '%s' to definition holder from line '%d' failed: 0x%08x"),
									m_spLogger, cameraName.GetData().AsInternal(), child->GetLineNumber(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else {
			// found unknown tag
			FWGLOG_ERROR_FORMAT(wxT("Unknown tag ['%s'] on line: %d"),
								m_spLogger,
								child->GetName().GetData().AsInternal(),
								child->GetLineNumber(),
								FWGLOG_ENDVAL);
			return FWG_E_XML_UNKNOWN_TAG_ERROR;
		}
		
		
		
		child = child->GetNext();
	}
	
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::LoadInput(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxXmlNode* child = pNode->GetChildren();
	while(child)
	{
		if(child->GetName() == GAME_TAG_COMP_INPUT) 
		{
			wxString inputName;
			RefObjSmPtr<InputDef> spInput;
			
			if(FWG_FAILED(result = CreateInput(child, inputName, spInput)))
			{
				// found unknown tag
				FWGLOG_ERROR_FORMAT(wxT("Create mesh failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}

			if(FWG_FAILED(result = defHolder.InsertDef<InputDef>( inputName, spInput, defHolder.m_inputDefs )))
			{
				FWGLOG_ERROR_FORMAT(wxT("Add input '%s' to definition holder from line '%d' failed: 0x%08x"),
									m_spLogger, inputName.GetData().AsInternal(), child->GetLineNumber(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else {
			// found unknown tag
			FWGLOG_ERROR_FORMAT(wxT("Unknown tag ['%s'] on line: %d"),
								m_spLogger,
								child->GetName().GetData().AsInternal(),
								child->GetLineNumber(),
								FWGLOG_ENDVAL);
			return FWG_E_XML_UNKNOWN_TAG_ERROR;
		}
		
		
		
		child = child->GetNext();
	}
	
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::LoadLogic(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxXmlNode* child = pNode->GetChildren();
	while(child)
	{
		if(child->GetName() == GAME_TAG_COMP_LOGIC) 
		{
			wxString logicName;
			RefObjSmPtr<LogicDef> spLogic;
			
			if(FWG_FAILED(result = CreateLogic(child, logicName, spLogic)))
			{
				// found unknown tag
				FWGLOG_ERROR_FORMAT(wxT("Create logic failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}

			if(FWG_FAILED(result = defHolder.InsertDef<LogicDef>( logicName, spLogic, defHolder.m_logicDefs )))
			{
				FWGLOG_ERROR_FORMAT(wxT("Add logic '%s' to definition holder from line '%d' failed: 0x%08x"),
									m_spLogger, logicName.GetData().AsInternal(), child->GetLineNumber(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else {
			// found unknown tag
			FWGLOG_ERROR_FORMAT(wxT("Unknown tag ['%s'] on line: %d"),
								m_spLogger,
								child->GetName().GetData().AsInternal(),
								child->GetLineNumber(),
								FWGLOG_ENDVAL);
			return FWG_E_XML_UNKNOWN_TAG_ERROR;
		}
		
		
		
		child = child->GetNext();
	}
	
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::LoadMaterials(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxXmlNode* child = pNode->GetChildren();
	while(child)
	{
		if(child->GetName() == GAME_TAG_COMP_MATERIAL) 
		{
			wxString materialName;
			RefObjSmPtr<NameDef> spMaterial;
			
			if(FWG_FAILED(result = CreateMaterial(child, materialName, spMaterial)))
			{
				// found unknown tag
				FWGLOG_ERROR_FORMAT(wxT("Create mesh failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}

			if(FWG_FAILED(result = defHolder.InsertDef<NameDef>( materialName, spMaterial, defHolder.m_materialDefs )))
			{
				FWGLOG_ERROR_FORMAT(wxT("Add material '%s' to definition holder from line '%d' failed: 0x%08x"),
									m_spLogger, materialName.GetData().AsInternal(), child->GetLineNumber(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else {
			// found unknown tag
			FWGLOG_ERROR_FORMAT(wxT("Unknown tag ['%s'] on line: %d"),
								m_spLogger,
								child->GetName().GetData().AsInternal(),
								child->GetLineNumber(),
								FWGLOG_ENDVAL);
			return FWG_E_XML_UNKNOWN_TAG_ERROR;
		}
		
		
		
		child = child->GetNext();
	}
	
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::LoadMeshes(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxXmlNode* child = pNode->GetChildren();
	while(child)
	{
		if(child->GetName() == GAME_TAG_COMP_MESH) 
		{
			wxString meshName;
			RefObjSmPtr<NameDef> spMesh;
			
			if(FWG_FAILED(result = CreateMesh(child, meshName, spMesh)))
			{
				// found unknown tag
				FWGLOG_ERROR_FORMAT(wxT("Create mesh failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}

			if(FWG_FAILED(result = defHolder.InsertDef<NameDef>( meshName, spMesh, defHolder.m_meshDefs )))
			{
				FWGLOG_ERROR_FORMAT(wxT("Add mesh '%s' to definition holder from line '%d' failed: 0x%08x"),
									m_spLogger, meshName.GetData().AsInternal(), child->GetLineNumber(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else {
			// found unknown tag
			FWGLOG_ERROR_FORMAT(wxT("Unknown tag ['%s'] on line: %d"),
								m_spLogger,
								child->GetName().GetData().AsInternal(),
								child->GetLineNumber(),
								FWGLOG_ENDVAL);
			return FWG_E_XML_UNKNOWN_TAG_ERROR;
		}
		
		
		
		child = child->GetNext();
	}
	
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::LoadRenderDef(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxXmlNode* child = pNode->GetChildren();
	while(child)
	{
		if(child->GetName() == GAME_TAG_COMP_RENDER) 
		{
			wxString renderName;
			RefObjSmPtr<RenderDef> spRenderDef;
			
			if(FWG_FAILED(result = CreateRender(child, defHolder, renderName, spRenderDef)))
			{
				// found unknown tag
				FWGLOG_ERROR_FORMAT(wxT("Create render component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}

			if(FWG_FAILED(result = defHolder.InsertDef<RenderDef>( renderName, spRenderDef, defHolder.m_renderDefs )))
			{
				FWGLOG_ERROR_FORMAT(wxT("Add render component '%s' to definition holder from line '%d' failed: 0x%08x"),
									m_spLogger, renderName.GetData().AsInternal(), child->GetLineNumber(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else {
			// found unknown tag
			FWGLOG_ERROR_FORMAT(wxT("Unknown tag ['%s'] on line: %d"),
								m_spLogger,
								child->GetName().GetData().AsInternal(),
								child->GetLineNumber(),
								FWGLOG_ENDVAL);
			return FWG_E_XML_UNKNOWN_TAG_ERROR;
		}
		
		
		
		child = child->GetNext();
	}
	
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::LoadRenderEntities(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxXmlNode* child = pNode->GetChildren();
	while(child)
	{
		if(child->GetName() == GAME_TAG_COMP_RENDER_ENT) 
		{
			wxString renderEntName;
			RefObjSmPtr<RenderEntityDef> spRenderEnt;
			
			if(FWG_FAILED(result = CreateRenderEntity(child, defHolder, renderEntName, spRenderEnt)))
			{
				// found unknown tag
				FWGLOG_ERROR_FORMAT(wxT("Create render entity failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}

			if(FWG_FAILED(result = defHolder.InsertDef<RenderEntityDef>( renderEntName, spRenderEnt, defHolder.m_renderEntityDefs )))
			{
				FWGLOG_ERROR_FORMAT(wxT("Add render entity '%s' to definition holder from line '%d' failed: 0x%08x"),
									m_spLogger, renderEntName.GetData().AsInternal(), child->GetLineNumber(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else {
			// found unknown tag
			FWGLOG_ERROR_FORMAT(wxT("Unknown tag ['%s'] on line: %d"),
								m_spLogger,
								child->GetName().GetData().AsInternal(),
								child->GetLineNumber(),
								FWGLOG_ENDVAL);
			return FWG_E_XML_UNKNOWN_TAG_ERROR;
		}
		
		
		
		child = child->GetNext();
	}
	
	
	return FWG_NO_ERROR;
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
								child->GetName().GetData().AsInternal(),
								child->GetLineNumber(),
								FWGLOG_ENDVAL);
			return FWG_E_XML_UNKNOWN_TAG_ERROR;
		}
		
		child = child->GetNext();
	}
	
	if(pMeshes)
	{
		if(FWG_FAILED(result = LoadMeshes(pMeshes, defHolder)))
		{
			FWGLOG_ERROR_FORMAT(wxT("Load meshes failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
	} else {
		FWGLOG_INFO(wxT("No mesh definitions found"), m_spLogger);
	}
	
	if(pMaterials)
	{
		if(FWG_FAILED(result = LoadMaterials(pMaterials, defHolder)))
		{
			FWGLOG_ERROR_FORMAT(wxT("Load materials failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
	} else {
		FWGLOG_INFO(wxT("No material definitions found"), m_spLogger);
	}
	
	if(pRenderEnts)
	{
		if(FWG_FAILED(result = LoadRenderEntities(pRenderEnts, defHolder)))
		{
			FWGLOG_ERROR_FORMAT(wxT("Load render entities failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
	} else {
		FWGLOG_INFO(wxT("No render entity definitions found"), m_spLogger);
	}
	
	if(pCameras)
	{
		if(FWG_FAILED(result = LoadCameras(pCameras, defHolder)))
		{
			FWGLOG_ERROR_FORMAT(wxT("ParseDefinitions failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
	} else {
		FWGLOG_INFO(wxT("No camera definitions found"), m_spLogger);
	}
	
	if(pRenderObjs)
	{
		if(FWG_FAILED(result = LoadRenderDef(pRenderObjs, defHolder)))
		{
			FWGLOG_ERROR_FORMAT(wxT("ParseDefinitions failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
	} else {
		FWGLOG_INFO(wxT("No render object definitions found"), m_spLogger);
	}
	
	if(pInputs)
	{
		if(FWG_FAILED(result = LoadInput(pInputs, defHolder)))
		{
			FWGLOG_ERROR_FORMAT(wxT("Load inputs failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
	} else {
		FWGLOG_INFO(wxT("No input definitions found"), m_spLogger);
	}
	
	if(pLogics)
	{
		if(FWG_FAILED(result = LoadLogic(pLogics, defHolder)))
		{
			FWGLOG_ERROR_FORMAT(wxT("Load logic failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
	} else {
		FWGLOG_INFO(wxT("No logic definitions found"), m_spLogger);
	}
	
	return FWG_NO_ERROR;	
	
}

GameErrorCode GameXmlResourceLoader::ParseScene(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxString entityName;
	RefObjSmPtr<EntityDef> spEntity;
	wxXmlNode *child = pNode->GetChildren();
	while(child)
	{
		if(child->GetName() == GAME_TAG_ENTITY_STR) 
		{
			entityName.Clear();
			if(FWG_FAILED(result = CreateEntity(child, defHolder, entityName, spEntity)))
			{
				// found unknown tag
				FWGLOG_ERROR_FORMAT(wxT("Create entity failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}

			if(FWG_FAILED(result = defHolder.InsertDef<EntityDef>( entityName, spEntity, defHolder.m_entityDefs )))
			{
				FWGLOG_ERROR_FORMAT(wxT("Add entity '%s' to definition holder from line '%d' failed: 0x%08x"),
									m_spLogger, entityName.GetData().AsInternal(), child->GetLineNumber(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else {
			// found unknown tag
			FWGLOG_ERROR_FORMAT(wxT("Unknown tag ['%s'] on line: %d"),
								m_spLogger,
								child->GetName().GetData().AsInternal(),
								child->GetLineNumber(),
								FWGLOG_ENDVAL);
			return FWG_E_XML_UNKNOWN_TAG_ERROR;
		}
		
		child = child->GetNext();
	}
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreateMesh(wxXmlNode* pNode, wxString& name, RefObjSmPtr<NameDef>& spDef)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxString meshName;
	bool meshNameFound = false;
	wxXmlNode *child = pNode->GetChildren();
	while (child)
	{
		if (child->GetName() == GAME_TAG_ITEM_MESHNAME) 
		{
			if (meshNameFound)
			{
				FWGLOG_ERROR_FORMAT(wxT("Tag '%s' is duplicate on line: %d"),
						m_spLogger, GAME_TAG_ITEM_MESHNAME, child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_TAG_ERROR;
			}
			
			// set meshName as found
			meshNameFound = true;				
			
			if(FWG_FAILED(result = GetAttrValue(child, meshName)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Get mesh name value failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}

		} else {
			// found unknown tag
			FWGLOG_ERROR_FORMAT(wxT("Unknown tag ['%s'] on line: %d"),
								m_spLogger,
								child->GetName().GetData().AsInternal(),
								child->GetLineNumber(),
								FWGLOG_ENDVAL);
			return FWG_E_XML_UNKNOWN_TAG_ERROR;
		}
		
		child = child->GetNext();
		
	}
	
	if (!meshNameFound)
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' is missing within tag '%s' on line: %d"),
						m_spLogger, GAME_TAG_ITEM_MESHNAME, pNode->GetName().GetData().AsInternal(), pNode->GetLineNumber(), FWGLOG_ENDVAL);
		return FWG_E_XML_TAG_NOT_FOUND_ERROR;
	}
	
	// create mesh
	FWG_RETURN_FAIL(GameNewChecked(spDef.OutRef()));
	spDef.In()->m_name = meshName;
	
	// get name attribute
	name = pNode->GetAttribute(GAME_ATTR_NAME_STR);
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreateMaterial(wxXmlNode* pNode, wxString& name, RefObjSmPtr<NameDef>& spDef)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxString materialName;
	bool materialNameFound = false;
	wxXmlNode *child = pNode->GetChildren();
	while (child)
	{
		if (child->GetName() == GAME_TAG_ITEM_MATERIALNAME) 
		{
			if (materialNameFound)
			{
				FWGLOG_ERROR_FORMAT(wxT("Tag '%s' is duplicate on line: %d"),
						m_spLogger, GAME_TAG_ITEM_MATERIALNAME, child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_TAG_ERROR;
			}
			
			// set meshName as found
			materialNameFound = true;				
			
			if(FWG_FAILED(result = GetAttrValue(child, materialName)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Get material name value failed: 0x%08x"),
						m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
			
		} else {
			// found unknown tag
			FWGLOG_ERROR_FORMAT(wxT("Unknown tag ['%s'] on line: %d"),
								m_spLogger,
								child->GetName().GetData().AsInternal(),
								child->GetLineNumber(),
								FWGLOG_ENDVAL);
			return FWG_E_XML_UNKNOWN_TAG_ERROR;
		}
		
		child = child->GetNext();
		
	}
	
	if (!materialNameFound)
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' is missing within tag '%s' on line: %d"),
						m_spLogger, GAME_TAG_ITEM_MATERIALNAME, pNode->GetName().GetData().AsInternal(), pNode->GetLineNumber(), FWGLOG_ENDVAL);
		return FWG_E_XML_TAG_NOT_FOUND_ERROR;
	}
	
	// create mesh
	FWG_RETURN_FAIL(GameNewChecked(spDef.OutRef()));
	spDef.In()->m_name = materialName;
	
	// get name attribute
	name = pNode->GetAttribute(GAME_ATTR_NAME_STR);
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreateRenderEntity(wxXmlNode* pNode, GameDefinitionHolder& defHolder, wxString& name, RefObjSmPtr<RenderEntityDef>& spDef)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<NameDef> spMesh;
	RefObjSmPtr<NameDef> spMaterial;
	Ogre::Vector3 position;
	
	bool positionFound = false;
	
	wxXmlNode *child = pNode->GetChildren();
	while (child)
	{
		if (child->GetName() == GAME_TAG_ITEM_MESH_REF) 
		{
			wxString meshRef;
			if (!spMesh.IsEmpty())
			{
				FWGLOG_ERROR_FORMAT(wxT("Tag '%s' is duplicate on line: %d"),
						m_spLogger, GAME_TAG_ITEM_MESH_REF, child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_TAG_ERROR;
			}
			
			
			if(FWG_FAILED(result = GetAttrValue(child, meshRef)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Get value failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}

			// find mesh in definition holder
			if(defHolder.m_meshDefs.Exists(meshRef))
			{
				spMesh = *defHolder.m_meshDefs.FindValue(meshRef);
			} else {
				FWGLOG_ERROR_FORMAT(wxT("Referenced mesh '%s' in tag '%s' does not exists on line: %d"),
						m_spLogger, meshRef.GetData().AsInternal(), GAME_TAG_ITEM_MESH_REF, child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_ATTR_ERROR;
			}
			
		} else if(child->GetName() == GAME_TAG_COMP_MESH) {
			wxString meshName;
			if (!spMesh.IsEmpty())
			{
				FWGLOG_ERROR_FORMAT(wxT("Tag '%s' is duplicate on line: %d"),
						m_spLogger, GAME_TAG_COMP_MESH, child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_TAG_ERROR;
			}
			
			if(FWG_FAILED(result = CreateMesh(child, meshName, spMesh)))
			{
				// found unknown tag
				FWGLOG_ERROR_FORMAT(wxT("Create mesh failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}

		} else if(child->GetName() == GAME_TAG_ITEM_MATERIAL_REF) {
			wxString materialRef;
			if (!spMaterial.IsEmpty())
			{
				FWGLOG_ERROR_FORMAT(wxT("Tag '%s' is duplicate on line: %d"),
						m_spLogger, GAME_TAG_ITEM_MATERIAL_REF, child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_TAG_ERROR;
			}
			
			if(FWG_FAILED(result = GetAttrValue(child, materialRef)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Get materialref value failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
			
			// find mesh in definition holder
			if(defHolder.m_materialDefs.Exists(materialRef))
			{
				spMaterial = *defHolder.m_materialDefs.FindValue(materialRef);
			} else {
				FWGLOG_ERROR_FORMAT(wxT("Referenced material '%s' in tag '%s' does not exists on line: %d"),
						m_spLogger, materialRef.GetData().AsInternal(), GAME_TAG_ITEM_MATERIAL_REF, child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_ATTR_ERROR;
			}
		} else if(child->GetName() == GAME_TAG_COMP_MATERIAL) {
			wxString materialName;
			if (!spMaterial.IsEmpty())
			{
				FWGLOG_ERROR_FORMAT(wxT("Tag '%s' is duplicate on line: %d"),
						m_spLogger, GAME_TAG_ITEM_MATERIAL_REF, child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_TAG_ERROR;
			}
			if(FWG_FAILED(result = CreateMaterial(child, materialName, spMaterial)))
			{
				// found unknown tag
				FWGLOG_ERROR_FORMAT(wxT("Create material failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
		} else if(child->GetName() == GAME_TAG_ITEM_POSITION) {
			if(positionFound)
			{
				FWGLOG_ERROR_FORMAT(wxT("Tag '%s' is duplicate on line: %d"),
						m_spLogger, GAME_TAG_ITEM_POSITION, child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_TAG_ERROR;
			}
			
			if(FWG_FAILED(result = GetAttrXYZ(child, position)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Read the position on line %d failed: 0x%08x"), m_spLogger, child->GetLineNumber(), result, FWGLOG_ENDVAL);
				return result;
			}
			positionFound = true;
		} else {
			// found unknown tag
			FWGLOG_ERROR_FORMAT(wxT("Unknown tag ['%s'] on line: %d"),
								m_spLogger,
								child->GetName().GetData().AsInternal(),
								child->GetLineNumber(),
								FWGLOG_ENDVAL);
			return FWG_E_XML_UNKNOWN_TAG_ERROR;
		}
		
		child = child->GetNext();
		
	}
	
	if (spMesh.IsEmpty()||spMaterial.IsEmpty()||(!positionFound))
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' has not all children (mesh[%d], material[%d], position[%d]) line: %d"),
						m_spLogger,
						pNode->GetName().GetData().AsInternal(),
						static_cast<int>(!spMesh.IsEmpty()),
						static_cast<int>(!spMaterial.IsEmpty()),
						static_cast<int>(positionFound),
						pNode->GetLineNumber(),
						FWGLOG_ENDVAL);
		return FWG_E_XML_TAG_NOT_FOUND_ERROR;
	}

	// get name attribute
	if(!pNode->GetAttribute(GAME_ATTR_NAME_STR, &name))
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' has no name (name is mandatory in this case) line: %d"),
						m_spLogger,
						pNode->GetName().GetData().AsInternal(),
						pNode->GetLineNumber(),
						FWGLOG_ENDVAL);
		return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
	}
	
	// create mesh
	FWG_RETURN_FAIL(GameNewChecked(spDef.OutRef()));
	spDef.In()->m_mesh = spMesh;
	spDef.In()->m_material = spMaterial;
	spDef.In()->m_position = position;
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreateCamera(wxXmlNode* pNode, wxString& name, RefObjSmPtr<CameraDef>& spCameraDef)
{	
	GameErrorCode result = FWG_NO_ERROR;
	Ogre::Vector3 position;
	Ogre::Vector3 direction;
	
	bool positionFound = false;
	bool directionFound = false;
	
	wxXmlNode *child = pNode->GetChildren();
	while (child)
	{
		if(child->GetName() == GAME_TAG_ITEM_POSITION) 
		{
			if(positionFound)
			{
				FWGLOG_ERROR_FORMAT(wxT("Tag '%s' is duplicate on line: %d"),
						m_spLogger, GAME_TAG_ITEM_POSITION, child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_TAG_ERROR;
			}
			
			if(FWG_FAILED(result = GetAttrXYZ(child, position)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Read the position on line %d failed: 0x%08x"), m_spLogger, child->GetLineNumber(), result, FWGLOG_ENDVAL);
				return result;
			}
			positionFound = true;
		} else if(child->GetName() == GAME_TAG_ITEM_DIRECTION) {
			if(directionFound)
			{
				FWGLOG_ERROR_FORMAT(wxT("Tag '%s' is duplicate on line: %d"),
						m_spLogger, GAME_TAG_ITEM_DIRECTION, child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_TAG_ERROR;
			}
			
			if(FWG_FAILED(result = GetAttrXYZ(child, direction)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Read the direction on line %d failed: 0x%08x"), m_spLogger, child->GetLineNumber(), result, FWGLOG_ENDVAL);
				return result;
			}
			directionFound = true;
		} else {
			// found unknown tag
			FWGLOG_ERROR_FORMAT(wxT("Unknown tag ['%s'] on line: %d"),
								m_spLogger,
								child->GetName().GetData().AsInternal(),
								child->GetLineNumber(),
								FWGLOG_ENDVAL);
			return FWG_E_XML_UNKNOWN_TAG_ERROR;
		}
		
		child = child->GetNext();
		
	}
	
	if (!directionFound||!positionFound)
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' has not all children (position[%d], direction[%d]) line: %d"),
						m_spLogger,
						pNode->GetName().GetData().AsInternal(),
						static_cast<int>(positionFound),
						static_cast<int>(directionFound),
						pNode->GetLineNumber(),
						FWGLOG_ENDVAL);
		return FWG_E_XML_TAG_NOT_FOUND_ERROR;
	}

	// get name attribute
	if(!pNode->GetAttribute(GAME_ATTR_NAME_STR, &name))
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' has no name (name is mandatory in this case) line: %d"),
						m_spLogger,
						pNode->GetName().GetData().AsInternal(),
						pNode->GetLineNumber(),
						FWGLOG_ENDVAL);
		return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
	}
	
	// create camera
	FWG_RETURN_FAIL(GameNewChecked(spCameraDef.OutRef()));
	spCameraDef.In()->m_position = position;
	spCameraDef.In()->m_direction = direction;
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreateRender(wxXmlNode* pNode, GameDefinitionHolder& defHolder, wxString& name, RefObjSmPtr<RenderDef>& spDef)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxString tempValue;
	
	RefObjSmPtr<RenderDef> spRenderDef;
	wxXmlNode *child = pNode->GetChildren();
	
	// create new render component
	FWG_RETURN_FAIL(GameNewChecked(spRenderDef.OutRef()));
	
	while (child)
	{
		if(child->GetName() == GAME_TAG_ITEM_RENDER_ENTITY_REF) 
		{
			tempValue.Clear(); // clear tempValue for sure
			RefObjSmPtr<RenderEntityDef> spRenderEntDef;
			
			if(FWG_FAILED(result = GetAttrValue(child, tempValue)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Read value: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
			
			if(defHolder.m_renderEntityDefs.Exists(tempValue))
			{
				spRenderEntDef = *defHolder.m_renderEntityDefs.FindValue(tempValue);
			}
			
			spRenderDef.In()->m_entities.push_back(spRenderEntDef);
		} else if(child->GetName() == GAME_TAG_COMP_RENDER_ENT) {
			wxString name;
			tempValue.Clear(); // clear tempValue for sure
			RefObjSmPtr<RenderEntityDef> spRenderEntDef;
			
			if(FWG_FAILED(result = CreateRenderEntity(child, defHolder, name, spRenderEntDef)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Create render entity failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
			
			if(FWG_FAILED(result = defHolder.InsertDef(name,spRenderEntDef,defHolder.m_renderEntityDefs)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Insert render entity on line '%d' to defHolder failed: 0x%08x"), m_spLogger, child->GetLineNumber(), result, FWGLOG_ENDVAL);
				return result;
			}
			
			spRenderDef.In()->m_entities.push_back(spRenderEntDef);
		} else if(child->GetName() == GAME_TAG_ITEM_CAMERA_REF) {
			
			RefObjSmPtr<CameraDef> spCameraDef;
			if(FWG_FAILED(result = GetAttrValue(child, tempValue)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Get value for cameraref failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
			
			if(defHolder.m_cameraDefs.Exists(tempValue))
			{
				spCameraDef = *defHolder.m_cameraDefs.FindValue(tempValue);
			}
			
			spRenderDef.In()->m_cameras.push_back(spCameraDef);
			
		} else {
			// found unknown tag
			FWGLOG_ERROR_FORMAT(wxT("Unknown tag ['%s'] on line: %d"),
								m_spLogger,
								child->GetName().GetData().AsInternal(),
								child->GetLineNumber(),
								FWGLOG_ENDVAL);
			return FWG_E_XML_UNKNOWN_TAG_ERROR;
		}
		
		child = child->GetNext();
		
	}
	
	// get name attribute
	if(!pNode->GetAttribute(GAME_ATTR_NAME_STR, &name))
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' has no name (name is mandatory in this case) line: %d"),
						m_spLogger,
						pNode->GetName().GetData().AsInternal(),
						pNode->GetLineNumber(),
						FWGLOG_ENDVAL);
		return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
	}
	
	// set return value
	spDef = spRenderDef;
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreateInput(wxXmlNode* pNode, wxString& name, RefObjSmPtr<InputDef> &spInputDef)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxInt32 keyCode = 0;
	wxString action;	
	RefObjSmPtr<InputDef> spTempInput;
	
	wxXmlNode *child = pNode->GetChildren();

	
	// create new input component
	FWG_RETURN_FAIL(GameNewChecked(spTempInput.OutRef()));
	
	while (child)
	{
		if(child->GetName() == GAME_TAG_INPUT_KEY) 
		{
			keyCode = 0;
			action.Clear(); // clear tempValue for sure
			
			if(FWG_FAILED(result = GetKeyValue(child, action, keyCode)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Read key attributes failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
			
			if(FWG_FAILED(result = spTempInput->m_inputMap.Insert(action, keyCode)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Insert key action failed (action='%s' on line='%d'): 0x%08x"), m_spLogger
										, action.GetData().AsInternal(), child->GetLineNumber(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else {
			// found unknown tag
			FWGLOG_ERROR_FORMAT(wxT("Unknown tag ['%s'] on line: %d"),
								m_spLogger,
								child->GetName().GetData().AsInternal(),
								child->GetLineNumber(),
								FWGLOG_ENDVAL);
			return FWG_E_XML_UNKNOWN_TAG_ERROR;
		}
		
		child = child->GetNext();
		
	}
	
	// get name attribute
	if(!pNode->GetAttribute(GAME_ATTR_NAME_STR, &name))
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' has no name (name is mandatory in this case) line: %d"),
						m_spLogger,
						pNode->GetName().GetData().AsInternal(),
						pNode->GetLineNumber(),
						FWGLOG_ENDVAL);
		return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
	}
	
	// set return value
	spInputDef = spTempInput;
	
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreateLogic(wxXmlNode* pNode, wxString& name, RefObjSmPtr<LogicDef>& spLogicDef)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxString logicName;
	bool logicNameFound = false;
	wxXmlNode *child = pNode->GetChildren();
	while (child)
	{
		if (child->GetName() == GAME_TAG_ITEM_LOGICTYPE) 
		{
			if (logicNameFound)
			{
				FWGLOG_ERROR_FORMAT(wxT("Tag '%s' is duplicate on line: %d"),
						m_spLogger, GAME_TAG_ITEM_LOGICTYPE, child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_TAG_ERROR;
			}
			
			// set meshName as found
			logicNameFound = true;				
			
			if(FWG_FAILED(result = GetAttrValue(child, logicName)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Get logic name value failed: 0x%08x"),
						m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
			
		} else {
			// found unknown tag
			FWGLOG_ERROR_FORMAT(wxT("Unknown tag ['%s'] on line: %d"),
								m_spLogger,
								child->GetName().GetData().AsInternal(),
								child->GetLineNumber(),
								FWGLOG_ENDVAL);
			return FWG_E_XML_UNKNOWN_TAG_ERROR;
		}
		
		child = child->GetNext();
		
	}
	
	if (!logicNameFound)
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' is missing within tag '%s' on line: %d"),
						m_spLogger, GAME_TAG_ITEM_LOGICTYPE, pNode->GetName().GetData().AsInternal(), pNode->GetLineNumber(), FWGLOG_ENDVAL);
		return FWG_E_XML_TAG_NOT_FOUND_ERROR;
	}
	
	// create mesh
	FWG_RETURN_FAIL(GameNewChecked(spLogicDef.OutRef()));
	spLogicDef->m_logicType = logicName;
	
	// get name attribute
	name = pNode->GetAttribute(GAME_ATTR_NAME_STR);
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreateTransform(wxXmlNode* pNode, RefObjSmPtr<TransformDef>& spTransform)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<TransformDef> spTransformTemp;
	
	bool positionFound = false;
	bool scaleFound = false;
	bool rotationFound = false;
	
	FWG_RETURN_FAIL( GameNewChecked(spTransformTemp.OutRef()));
	
	wxXmlNode *child = pNode->GetChildren();
	while (child)
	{
		if(child->GetName() == GAME_TAG_ITEM_POSITION) 
		{
			if(positionFound)
			{
				FWGLOG_ERROR_FORMAT(wxT("Tag '%s' is duplicate on line: %d"),
						m_spLogger, child->GetName().GetData().AsInternal(), child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_TAG_ERROR;
			}
			
			if(FWG_FAILED(result = GetAttrXYZ(child, spTransformTemp->m_position)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Read the position on line %d failed: 0x%08x"), m_spLogger, child->GetLineNumber(), result, FWGLOG_ENDVAL);
				return result;
			}
			positionFound = true;
		} else if(child->GetName() == GAME_TAG_ITEM_SCALE) {
			if(scaleFound)
			{
				FWGLOG_ERROR_FORMAT(wxT("Tag '%s' is duplicate on line: %d"),
						m_spLogger, child->GetName().GetData().AsInternal(), child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_TAG_ERROR;
			}
			
			if(FWG_FAILED(result = GetAttrXYZ(child, spTransformTemp->m_scale)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Read the direction on line %d failed: 0x%08x"), m_spLogger, child->GetLineNumber(), result, FWGLOG_ENDVAL);
				return result;
			}
			scaleFound = true;
		} else if(child->GetName() == GAME_TAG_ITEM_ROTATION) {
			if(rotationFound)
			{
				FWGLOG_ERROR_FORMAT(wxT("Tag '%s' is duplicate on line: %d"),
						m_spLogger, child->GetName().GetData().AsInternal(), child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_TAG_ERROR;
			}
			
			if(FWG_FAILED(result = GetAttrQuat(child, spTransformTemp->m_rotation)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Read the direction on line %d failed: 0x%08x"), m_spLogger, child->GetLineNumber(), result, FWGLOG_ENDVAL);
				return result;
			}
			rotationFound = true;
		} else {
			// found unknown tag
			FWGLOG_ERROR_FORMAT(wxT("Unknown tag ['%s'] on line: %d"),
								m_spLogger,
								child->GetName().GetData().AsInternal(),
								child->GetLineNumber(),
								FWGLOG_ENDVAL);
			return FWG_E_XML_UNKNOWN_TAG_ERROR;
		}
		
		child = child->GetNext();
		
	}
	
	// set return value
	spTransform = spTransformTemp;
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreateEntity(wxXmlNode* pNode, GameDefinitionHolder& defHolder, wxString& entityName, RefObjSmPtr<EntityDef>& spEnt)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxString tempValue;
	wxString name;
	
	RefObjSmPtr<EntityDef> spEntity;

	
	wxXmlNode *child = pNode->GetChildren();
	
	// create new render component
	FWG_RETURN_FAIL(GameNewChecked(spEntity.OutRef()));
	
	while (child)
	{
		if(child->GetName() == GAME_TAG_ITEM_RENDER_REF) 
		{
			tempValue.Clear(); // clear tempValue for sure
			if( !spEntity->m_renderDef.IsEmpty())
			{
				FWGLOG_ERROR_FORMAT(wxT("Render component is already defined for this entity on line: %d")
								, m_spLogger, child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_OBJECT_ALREADY_EXISTS_ERROR;
			}
			
			if(FWG_FAILED(result = GetAttrValue(child, tempValue)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Read value: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
			
			if(defHolder.m_renderDefs.Exists(tempValue))
			{
				spEntity->m_renderDef = *defHolder.m_renderDefs.FindValue(tempValue);
			} else {
				FWGLOG_ERROR_FORMAT(wxT("Render component with name '%s' not exists, line: %d"),
										m_spLogger, tempValue.GetData().AsInternal(), child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_OBJECT_NOT_FOUND_ERROR;
			}
			
		} else if(child->GetName() == GAME_TAG_COMP_RENDER) {
			name.Clear();
			if( !spEntity->m_renderDef.IsEmpty())
			{
				FWGLOG_ERROR_FORMAT(wxT("Render component is already defined for this entity on line: %d")
								, m_spLogger, child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_OBJECT_ALREADY_EXISTS_ERROR;
			}
			
			if(FWG_FAILED(result = CreateRender(child, defHolder, name, spEntity->m_renderDef)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Create render component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
			
			if(FWG_FAILED(result = defHolder.InsertDef(name, spEntity->m_renderDef,defHolder.m_renderDefs)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Insert render component on line '%d' to defHolder failed: 0x%08x"), m_spLogger, child->GetLineNumber(), result, FWGLOG_ENDVAL);
				return result;
			}
			
		} else if(child->GetName() == GAME_TAG_ITEM_INPUT_REF) {
			
			tempValue.Clear(); // clear tempValue for sure
			if( !spEntity->m_inputDef.IsEmpty())
			{
				FWGLOG_ERROR_FORMAT(wxT("Input component is already defined for this entity on line: %d")
								, m_spLogger, child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_OBJECT_ALREADY_EXISTS_ERROR;
			}
			
			if(FWG_FAILED(result = GetAttrValue(child, tempValue)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Read value: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
			
			if(defHolder.m_inputDefs.Exists(tempValue))
			{
				spEntity->m_inputDef = *defHolder.m_inputDefs.FindValue(tempValue);
			} else {
				FWGLOG_ERROR_FORMAT(wxT("Input component with name '%s' not exists, line: %d"),
										m_spLogger, tempValue.GetData().AsInternal(), child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_OBJECT_NOT_FOUND_ERROR;
			}
		} else if(child->GetName() == GAME_TAG_COMP_INPUT) {	
			name.Clear();
			if( !spEntity->m_inputDef.IsEmpty())
			{
				FWGLOG_ERROR_FORMAT(wxT("Input component is already defined for this entity on line: %d")
								, m_spLogger, child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_OBJECT_ALREADY_EXISTS_ERROR;
			}
			
			if(FWG_FAILED(result = CreateInput(child, name, spEntity->m_inputDef)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Create input component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
			
			if(FWG_FAILED(result = defHolder.InsertDef(name, spEntity->m_inputDef,defHolder.m_inputDefs)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Insert input component on line '%d' to defHolder failed: 0x%08x"), m_spLogger, child->GetLineNumber(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else if(child->GetName() == GAME_TAG_ITEM_LOGIC_REF) {
			
			tempValue.Clear(); // clear tempValue for sure
			if( !spEntity->m_logicDef.IsEmpty())
			{
				FWGLOG_ERROR_FORMAT(wxT("Logic component is already defined for this entity on line: %d")
								, m_spLogger, child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_OBJECT_ALREADY_EXISTS_ERROR;
			}
			
			if(FWG_FAILED(result = GetAttrValue(child, tempValue)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Read value: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
			
			if(defHolder.m_logicDefs.Exists(tempValue))
			{
				spEntity->m_logicDef = *defHolder.m_logicDefs.FindValue(tempValue);
			} else {
				FWGLOG_ERROR_FORMAT(wxT("Logic component with name '%s' not exists, line: %d"),
										m_spLogger, tempValue.GetData().AsInternal(), child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_OBJECT_NOT_FOUND_ERROR;
			}
		} else if(child->GetName() == GAME_TAG_COMP_LOGIC) {	
			name.Clear();
			if( !spEntity->m_logicDef.IsEmpty())
			{
				FWGLOG_ERROR_FORMAT(wxT("Logic component is already defined for this entity on line: %d")
								, m_spLogger, child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_OBJECT_ALREADY_EXISTS_ERROR;
			}
			
			if(FWG_FAILED(result = CreateLogic(child, name, spEntity->m_logicDef)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Create logic component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
			
			if(FWG_FAILED(result = defHolder.InsertDef(name, spEntity->m_logicDef,defHolder.m_logicDefs)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Insert logic component on line '%d' to defHolder failed: 0x%08x"), m_spLogger, child->GetLineNumber(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else if(child->GetName() == GAME_TAG_COMP_TRANSFORM) {	
			if( !spEntity->m_transformation.IsEmpty())
			{
				FWGLOG_ERROR_FORMAT(wxT("Transform component is already defined for this entity on line: %d")
								, m_spLogger, child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_OBJECT_ALREADY_EXISTS_ERROR;
			}
			
			if(FWG_FAILED(result = CreateTransform(child, spEntity->m_transformation)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Create transform component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
		} else {
			// found unknown tag
			FWGLOG_ERROR_FORMAT(wxT("Unknown tag ['%s'] on line: %d"),
								m_spLogger,
								child->GetName().GetData().AsInternal(),
								child->GetLineNumber(),
								FWGLOG_ENDVAL);
			return FWG_E_XML_UNKNOWN_TAG_ERROR;
		}
		
		child = child->GetNext();
		
	}

	// get name attribute
	if(!pNode->GetAttribute(GAME_ATTR_NAME_STR, &entityName))
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' has no name (name is mandatory in this case) line: %d"),
						m_spLogger,
						pNode->GetName().GetData().AsInternal(),
						pNode->GetLineNumber(),
						FWGLOG_ENDVAL);
		return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
	}
	
	// set return value
	spEnt = spEntity;
	
	return FWG_NO_ERROR;
}


GameErrorCode GameXmlResourceLoader::GetAttrXYZ(wxXmlNode* pNode, Ogre::Vector3 &vector)
{
	wxString xyzStr;
	wxArrayString arrStr;
	double dTemp;
	Ogre::Vector3 tempVec;
	if(!pNode->GetAttribute(GAME_ATTR_XYZ_STR, &xyzStr))
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' has not attribute '%s' - line: %d"),
						m_spLogger, pNode->GetName().GetData().AsInternal(), GAME_ATTR_XYZ_STR, pNode->GetLineNumber(), FWGLOG_ENDVAL);
		return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
	}	
	
	arrStr = wxStringTokenize (xyzStr, wxString(wxT(",;")), wxTOKEN_RET_EMPTY_ALL);
	
	// check count
	if(arrStr.GetCount() != 3)
	{
		FWGLOG_ERROR_FORMAT(wxT("Attribute '%s' from tag '%s' on line %d, contains wrong number of items (%d)"),
						m_spLogger, GAME_ATTR_XYZ_STR, pNode->GetName().GetData().AsInternal(),
						pNode->GetLineNumber(),
						arrStr.GetCount(),
						FWGLOG_ENDVAL);
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	for ( wxInt32 i = 0; i < 3; i++)
	{
		if(!arrStr.Item(i).ToDouble(&dTemp))
		{
			FWGLOG_ERROR_FORMAT(wxT("Cannot convert item '%s' ( idx=[ %d ] ) to number: attribute '%s' from tag '%s' on line %d"),
						m_spLogger,
						arrStr.Item(i).GetData().AsInternal(),
						i,
						GAME_ATTR_XYZ_STR,
						pNode->GetName().GetData().AsInternal(),
						pNode->GetLineNumber(),
						FWGLOG_ENDVAL);
			return FWG_E_INVALID_PARAMETER_ERROR;
		}
		
		tempVec[i] = static_cast<Ogre::Real>(dTemp);
		
	}
	
	vector = tempVec;
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::GetAttrValue(wxXmlNode *pNode, wxString &value)
{
	wxString tempValue;
	// get value
	if(!pNode->GetAttribute(GAME_ATTR_VALUE_STR, &tempValue))
	{
		FWGLOG_ERROR_FORMAT(wxT("'%s' attribut from tag '%s' is missing on line: %d"),
				m_spLogger, GAME_ATTR_VALUE_STR, pNode->GetName().GetData().AsInternal(), pNode->GetLineNumber(), FWGLOG_ENDVAL);
		return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
	}
	
	// swap string containers
	value.swap(tempValue);
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::GetKeyValue(wxXmlNode *pNode, wxString &action, wxInt32 &keyCode)
{
	wxString tempAction;
	wxString tempValue;
	long 	tempKeyCode = -1;

	// get value
	if(!pNode->GetAttribute(GAME_ATTR_ACTION_STR, &tempAction))
	{
		FWGLOG_ERROR_FORMAT(wxT("'%s' attribut from tag '%s' is missing on line: %d"),
				m_spLogger, GAME_ATTR_ACTION_STR, pNode->GetName().GetData().AsInternal(), pNode->GetLineNumber(), FWGLOG_ENDVAL);
		return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
	}

	// get value
	if(!pNode->GetAttribute(GAME_ATTR_VALUE_STR, &tempValue))
	{
		FWGLOG_ERROR_FORMAT(wxT("'%s' attribut from tag '%s' is missing on line: %d"),
				m_spLogger, GAME_ATTR_VALUE_STR, pNode->GetName().GetData().AsInternal(), pNode->GetLineNumber(), FWGLOG_ENDVAL);
		return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
	}
	
	// found keycode for stringValue
	// check if value is not direct keycode
	if(!tempValue.ToLong(&tempKeyCode))
	{
		tempKeyCode = -1; // reset keycode
		for(wxDword i = 0; i < WXSIZEOF(KeyTable); i++)
		{
			if(tempValue.Cmp(KeyTable[i].m_keyName) == 0)
			{
				tempKeyCode = KeyTable[i].m_keyCode;
				break;
			}
		}
		
		// check if search was successful
		if(tempKeyCode == -1)
		{
			FWGLOG_ERROR_FORMAT(wxT("'%s' attribut from tag '%s' has wrong format on line: %d"),
				m_spLogger, GAME_ATTR_VALUE_STR, pNode->GetName().GetData().AsInternal(), pNode->GetLineNumber(), FWGLOG_ENDVAL);
			return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
		}
	}
	
	
	// swap string containers
	action.swap(tempAction);
	keyCode = tempKeyCode;
	
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::GetAttrQuat(wxXmlNode* pNode, Ogre::Quaternion& quat)
{
	wxString quatStr;
	wxArrayString arrStr;
	double dTemp;
	Ogre::Quaternion tempQuat;
	if(!pNode->GetAttribute(GAME_ATTR_QUAT_STR, &quatStr))
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' has not attribute '%s' - line: %d"),
						m_spLogger, pNode->GetName().GetData().AsInternal(), GAME_ATTR_QUAT_STR, pNode->GetLineNumber(), FWGLOG_ENDVAL);
		return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
	}	
	
	arrStr = wxStringTokenize (quatStr, wxString(wxT(",;")), wxTOKEN_RET_EMPTY_ALL);
	
	// check count
	if(arrStr.GetCount() != 4)
	{
		FWGLOG_ERROR_FORMAT(wxT("Attribute '%s' from tag '%s' on line %d, contains wrong number of items (%d)"),
						m_spLogger, GAME_ATTR_QUAT_STR, pNode->GetName().GetData().AsInternal(),
						pNode->GetLineNumber(),
						arrStr.GetCount(),
						FWGLOG_ENDVAL);
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	for ( wxInt32 i = 0; i < 4; i++)
	{
		if(!arrStr.Item(i).ToDouble(&dTemp))
		{
			FWGLOG_ERROR_FORMAT(wxT("Cannot convert item '%s' ( idx=[ %d ] ) to number: attribute '%s' from tag '%s' on line %d"),
						m_spLogger,
						arrStr.Item(i).GetData().AsInternal(),
						i,
						GAME_ATTR_QUAT_STR,
						pNode->GetName().GetData().AsInternal(),
						pNode->GetLineNumber(),
						FWGLOG_ENDVAL);
			return FWG_E_INVALID_PARAMETER_ERROR;
		}
		
		tempQuat[i] = static_cast<Ogre::Real>(dTemp);
		
	}
	
	quat.swap(tempQuat);
	
	return FWG_NO_ERROR;
}
