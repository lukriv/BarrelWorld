#include "gxmlloader.h"
#include "gxmldefs.h"
#include <wx/filename.h>
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
			
			if(FWG_FAILED(result = CreateRenderEntity(child, renderEntName, spRenderEnt)))
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
	
	if(FWG_FAILED(result = LoadMeshes(pMeshes, defHolder)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Load meshes failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = LoadMaterials(pMaterials, defHolder)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Load materials failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}

	if(FWG_FAILED(result = LoadRenderEntities(pRenderEnts, defHolder)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Load render entities failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = LoadCameras(pCameras, defHolder)))
	{
		FWGLOG_ERROR_FORMAT(wxT("ParseDefinitions failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = LoadRenderDef(pRenderObjs, defHolder)))
	{
		FWGLOG_ERROR_FORMAT(wxT("ParseDefinitions failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
		
	if(FWG_FAILED(result = LoadInput(pInputs, defHolder)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Load inputs failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = LoadLogic(pLogics, defHolder)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Load logic failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	return FWG_NO_ERROR;	
	
}

GameErrorCode GameXmlResourceLoader::ParseScene(wxXmlNode* pNode, GameDefinitionHolder& defHolder)
{
}

GameErrorCode GameXmlResourceLoader::CreateMesh(wxXmlNode* pNode, wxString& name, RefObjSmPtr<NameDef>& spDef)
{
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
				
			if(!child->GetAttribute(GAME_ATTR_VALUE_STR, &meshName))
			{
				FWGLOG_ERROR_FORMAT(wxT("'%s' attribut for '%s' is missing on line: %d"),
						m_spLogger, GAME_ATTR_VALUE_STR, GAME_TAG_ITEM_MESHNAME, child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
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
						m_spLogger, GAME_TAG_ITEM_MESHNAME, pNode->GetName().GetData().AsInternal(), child->GetLineNumber(), FWGLOG_ENDVAL);
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
				
			if(!child->GetAttribute(GAME_ATTR_VALUE_STR, &materialName))
			{
				FWGLOG_ERROR_FORMAT(wxT("'%s' attribut for '%s' is missing on line: %d"),
						m_spLogger, GAME_ATTR_VALUE_STR, GAME_TAG_ITEM_MATERIALNAME, child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
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
						m_spLogger, GAME_TAG_ITEM_MATERIALNAME, pNode->GetName().GetData().AsInternal(), child->GetLineNumber(), FWGLOG_ENDVAL);
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
			
			// get value
			if(!child->GetAttribute(GAME_ATTR_VALUE_STR, &meshRef))
			{
				FWGLOG_ERROR_FORMAT(wxT("'%s' attribut for '%s' is missing on line: %d"),
						m_spLogger, GAME_ATTR_VALUE_STR, GAME_TAG_ITEM_MESH_REF, child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
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
			
		} else if(GAME_TAG_COMP_MESH) {
		} else if(GAME_TAG_ITEM_MATERIAL_REF) {
		} else if(GAME_TAG_COMP_MATERIAL) {
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
						m_spLogger, GAME_TAG_ITEM_MATERIALNAME, pNode->GetName().GetData().AsInternal(), child->GetLineNumber(), FWGLOG_ENDVAL);
		return FWG_E_XML_TAG_NOT_FOUND_ERROR;
	}
	
	// create mesh
	FWG_RETURN_FAIL(GameNewChecked(spDef.OutRef()));
	spDef.In()->m_name = materialName;
	
	// get name attribute
	name = pNode->GetAttribute(GAME_ATTR_NAME_STR);
	
	return FWG_NO_ERROR;
}
