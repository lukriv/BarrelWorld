#include "gxmlloader.h"
#include "gxmldefs.h"
#include "gxmlkeydefs.h"
#include "gxmlparams.h"
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
	
	wxXmlNode* pInputs = nullptr;
	
	wxXmlNode* child = pNode->GetChildren();
	while(child)
	{

		if(child->GetName() == GAME_TAG_DEFS_INPUTS_STR) {
			SECOND_TAG_DEFINITION_CHECK(pInputs,child);
			pInputs = child;
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
		} else if(child->GetName() == GAME_TAG_COMP_TERRAIN) {
			if(!defHolder.m_spTerrain.IsEmpty())
			{
				FWGLOG_ERROR_FORMAT(wxT("Tag '%s' already defined"), m_spLogger
										, child->GetName().GetData().AsInternal()
										, FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_TAG_ERROR;
			}

			if(FWG_FAILED(result = CreateTerrain(child, defHolder.m_spTerrain)))
			{
				// found unknown tag
				FWGLOG_ERROR_FORMAT(wxT("Create terrain failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
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
	GameBasSet<wxString> foundedParams;
	
	// create mesh
	FWG_RETURN_FAIL(GameNewChecked(spDef.OutRef()));
	
	wxXmlNode *child = pNode->GetChildren();
	while (child)
	{

		{
			// set meshName as found
			if(FWG_FAILED(result = GetParameter(child, MeshXmlParamTable, WXSIZEOF(MeshXmlParamTable), (void*)spDef.In())))
			{
				FWGLOG_ERROR_FORMAT(wxT("Unknown tag within tag ['%s'] on line: %d"),
								m_spLogger,
								pNode->GetName().GetData().AsInternal(),
								pNode->GetLineNumber(),
								FWGLOG_ENDVAL);
				return result;
			}
			
			if(foundedParams.Exists(child->GetName()))
			{
				FWGLOG_ERROR_FORMAT(wxT("Parameter '%s' already exists ( tag '%s' on line %d )"), m_spLogger
							, child->GetName().GetData().AsInternal()
							, pNode->GetName().GetData().AsInternal()
							, pNode->GetLineNumber()
							, FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_TAG_ERROR;
			}
			
			if(child->GetType() == wxXML_ELEMENT_NODE)
			{
				FWG_RETURN_FAIL(foundedParams.Insert(child->GetName()));
			}
		}
		
		child = child->GetNext();
		
	}
	
	if (FWG_FAILED(result = ParametersValidate(foundedParams, MeshXmlParamTable, WXSIZEOF(MeshXmlParamTable))))
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' have not all mandatory subtags line %d: 0x%08x"),
						m_spLogger
						, pNode->GetName().GetData().AsInternal()
						, pNode->GetLineNumber()
						, result
						, FWGLOG_ENDVAL);
		return result;
	}
	
	// get name attribute
	name = pNode->GetAttribute(GAME_ATTR_NAME_STR);
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreateMaterial(wxXmlNode* pNode, wxString& name, RefObjSmPtr<NameDef>& spDef)
{
	GameErrorCode result = FWG_NO_ERROR;
	GameBasSet<wxString> foundedParams;
	
	wxXmlNode *child = pNode->GetChildren();
	
	// create mesh
	FWG_RETURN_FAIL(GameNewChecked(spDef.OutRef()));
	
	while (child)
	{
		{

			// set meshName as found
			if(FWG_FAILED(result = GetParameter(child, MaterialXmlParamTable, WXSIZEOF(MaterialXmlParamTable), (void*)spDef.In())))
			{
				FWGLOG_ERROR_FORMAT(wxT("Unknown tag within tag ['%s'] on line: %d"),
								m_spLogger,
								pNode->GetName().GetData().AsInternal(),
								pNode->GetLineNumber(),
								FWGLOG_ENDVAL);
				return result;
			}
			
			if(foundedParams.Exists(child->GetName()))
			{
				FWGLOG_ERROR_FORMAT(wxT("Parameter '%s' already exists ( tag '%s' on line %d )"), m_spLogger
							, child->GetName().GetData().AsInternal()
							, pNode->GetName().GetData().AsInternal()
							, pNode->GetLineNumber()
							, FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_ATTR_ERROR;
			}
			
			if(child->GetType() == wxXML_ELEMENT_NODE)
			{
				FWG_RETURN_FAIL(foundedParams.Insert(child->GetName()));
			}

		}
		
		child = child->GetNext();
		
	}
	
	if (FWG_FAILED(result = ParametersValidate(foundedParams, MaterialXmlParamTable, WXSIZEOF(MaterialXmlParamTable))))
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' have not all mandatory subtags line %d: 0x%08x"),
						m_spLogger
						, pNode->GetName().GetData().AsInternal()
						, pNode->GetLineNumber()
						, result
						, FWGLOG_ENDVAL);
		return result;
	}
	
	
	// get name attribute
	name = pNode->GetAttribute(GAME_ATTR_NAME_STR);
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreateRenderEntity(wxXmlNode* pNode, GameDefinitionHolder& defHolder, wxString& name, RefObjSmPtr<RenderEntityDef>& spDef)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<NameDef> spMesh;
	RefObjSmPtr<NameDef> spMaterial;
	GameBasSet<wxString> foundedParams;
	
	// create mesh
	FWG_RETURN_FAIL(GameNewChecked(spDef.OutRef()));
	
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
		} else {

			// set meshName as found
			if(FWG_FAILED(result = GetParameter(child, RenderEntityXmlParamTable, WXSIZEOF(RenderEntityXmlParamTable), (void*)spDef.In())))
			{
				FWGLOG_ERROR_FORMAT(wxT("Unknown tag within tag ['%s'] on line: %d"),
								m_spLogger,
								pNode->GetName().GetData().AsInternal(),
								pNode->GetLineNumber(),
								FWGLOG_ENDVAL);
				return result;
			}
			
			if(foundedParams.Exists(child->GetName()))
			{
				FWGLOG_ERROR_FORMAT(wxT("Parameter '%s' already exists ( tag '%s' on line %d )"), m_spLogger
							, child->GetName().GetData().AsInternal()
							, pNode->GetName().GetData().AsInternal()
							, pNode->GetLineNumber()
							, FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_ATTR_ERROR;
			}
			
			if(child->GetType() == wxXML_ELEMENT_NODE)
			{
				FWG_RETURN_FAIL(foundedParams.Insert(child->GetName()));
			}

		}
		
		child = child->GetNext();
		
	}
	
	if (spMesh.IsEmpty()||spMaterial.IsEmpty())
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' has not all children (mesh[%d], material[%d]) line: %d"),
						m_spLogger,
						pNode->GetName().GetData().AsInternal(),
						static_cast<int>(!spMesh.IsEmpty()),
						static_cast<int>(!spMaterial.IsEmpty()),
						pNode->GetLineNumber(),
						FWGLOG_ENDVAL);
		return FWG_E_XML_TAG_NOT_FOUND_ERROR;
	}
	
	if (FWG_FAILED(result = ParametersValidate(foundedParams, RenderEntityXmlParamTable, WXSIZEOF(RenderEntityXmlParamTable))))
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' have not all mandatory subtags line %d: 0x%08x"),
						m_spLogger
						, pNode->GetName().GetData().AsInternal()
						, pNode->GetLineNumber()
						, result
						, FWGLOG_ENDVAL);
		return result;
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
	

	spDef.In()->m_mesh = spMesh;
	spDef.In()->m_material = spMaterial;
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreateCamera(wxXmlNode* pNode, wxString& name, RefObjSmPtr<CameraDef>& spCameraDef)
{	
	GameErrorCode result = FWG_NO_ERROR;
	GameBasSet<wxString> foundedParams;
	
	// create camera
	FWG_RETURN_FAIL(GameNewChecked(spCameraDef.OutRef()));
	
	wxXmlNode *child = pNode->GetChildren();
	while (child)
	{
		{

			// set meshName as found
			if(FWG_FAILED(result = GetParameter(child, CameraXmlParamTable, WXSIZEOF(CameraXmlParamTable), (void*)spCameraDef.In())))
			{
				FWGLOG_ERROR_FORMAT(wxT("Unknown tag within tag ['%s'] on line: %d"),
								m_spLogger,
								pNode->GetName().GetData().AsInternal(),
								pNode->GetLineNumber(),
								FWGLOG_ENDVAL);
				return result;
			}
			
			if(foundedParams.Exists(child->GetName()))
			{
				FWGLOG_ERROR_FORMAT(wxT("Parameter '%s' already exists ( tag '%s' on line %d )"), m_spLogger
							, child->GetName().GetData().AsInternal()
							, child->GetName().GetData().AsInternal()
							, child->GetLineNumber()
							, FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_ATTR_ERROR;
			}
			
			if(child->GetType() == wxXML_ELEMENT_NODE)
			{
				FWG_RETURN_FAIL(foundedParams.Insert(child->GetName()));
			}

		}
		
		child = child->GetNext();
		
	}
	
	if (FWG_FAILED(result = ParametersValidate(foundedParams, CameraXmlParamTable, WXSIZEOF(CameraXmlParamTable))))
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' have not all mandatory subtags line %d: 0x%08x"),
						m_spLogger
						, pNode->GetName().GetData().AsInternal()
						, pNode->GetLineNumber()
						, result
						, FWGLOG_ENDVAL);
		return result;
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
		if(child->GetName() == GAME_TAG_ITEM_CAMERA_REF) {
			
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
	wxString typeStr;
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
	
	// get type attribute
	if(pNode->GetAttribute(GAME_ATTR_TYPE_STR, &typeStr))
	{
		if(FWG_FAILED(result = ConvertToInputType(typeStr, spTempInput->m_inputType)))
		{
			FWGLOG_ERROR_FORMAT(wxT("Tag '%s' has not valid type attribute on line: %d"),
						m_spLogger,
						pNode->GetName().GetData().AsInternal(),
						pNode->GetLineNumber(),
						FWGLOG_ENDVAL);
			return FWG_E_XML_INVALID_ATTR_ERROR;
		}
		
	}
	
	// set return value
	spInputDef = spTempInput;
	
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreateLogic(wxXmlNode* pNode, wxString& name, RefObjSmPtr<LogicDef>& spLogicDef)
{
	GameErrorCode result = FWG_NO_ERROR;
	GameBasSet<wxString> foundedParams;
	
	// create logic def
	FWG_RETURN_FAIL(GameNewChecked(spLogicDef.OutRef()));
	
	wxXmlNode *child = pNode->GetChildren();
	while (child)
	{
		{

			// set meshName as found
			if(FWG_FAILED(result = GetParameter(child, LogicXmlParamTable, WXSIZEOF(LogicXmlParamTable), (void*)spLogicDef.In())))
			{
				FWGLOG_ERROR_FORMAT(wxT("Unknown tag within tag ['%s'] on line: %d"),
								m_spLogger,
								pNode->GetName().GetData().AsInternal(),
								pNode->GetLineNumber(),
								FWGLOG_ENDVAL);
				return result;
			}
			
			if(foundedParams.Exists(child->GetName()))
			{
				FWGLOG_ERROR_FORMAT(wxT("Parameter '%s' already exists ( tag '%s' on line %d )"), m_spLogger
							, child->GetName().GetData().AsInternal()
							, pNode->GetName().GetData().AsInternal()
							, pNode->GetLineNumber()
							, FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_ATTR_ERROR;
			}
			
			if(child->GetType() == wxXML_ELEMENT_NODE)
			{
				FWG_RETURN_FAIL(foundedParams.Insert(child->GetName()));
			}

		}
		
		child = child->GetNext();
		
	}
	
	if (FWG_FAILED(result = ParametersValidate(foundedParams, LogicXmlParamTable, WXSIZEOF(LogicXmlParamTable))))
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' have not all mandatory subtags line %d: 0x%08x"),
						m_spLogger
						, pNode->GetName().GetData().AsInternal()
						, pNode->GetLineNumber()
						, result
						, FWGLOG_ENDVAL);
		return result;
	}
	
	
	// get name attribute
	name = pNode->GetAttribute(GAME_ATTR_NAME_STR);
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreateTransform(wxXmlNode* pNode, RefObjSmPtr<TransformDef>& spTransform)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<TransformDef> spTransformTemp;
	
	GameBasSet<wxString> foundedParams;
	
	FWG_RETURN_FAIL( GameNewChecked(spTransformTemp.OutRef()));
	
	wxXmlNode *child = pNode->GetChildren();
	while (child)
	{
		{

			// set meshName as found
			if(FWG_FAILED(result = GetParameter(child, TransformXmlParamTable, WXSIZEOF(TransformXmlParamTable), (void*)spTransformTemp.In())))
			{
				FWGLOG_ERROR_FORMAT(wxT("Unknown tag within tag ['%s'] on line: %d"),
								m_spLogger,
								pNode->GetName().GetData().AsInternal(),
								pNode->GetLineNumber(),
								FWGLOG_ENDVAL);
				return result;
			}
			
			if(foundedParams.Exists(child->GetName()))
			{
				FWGLOG_ERROR_FORMAT(wxT("Parameter '%s' already exists ( tag '%s' on line %d )"), m_spLogger
							, child->GetName().GetData().AsInternal()
							, pNode->GetName().GetData().AsInternal()
							, pNode->GetLineNumber()
							, FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_ATTR_ERROR;
			}
			
			if(child->GetType() == wxXML_ELEMENT_NODE)
			{
				FWG_RETURN_FAIL(foundedParams.Insert(child->GetName()));
			}

		}
		
		child = child->GetNext();
		
	}
	
	if (FWG_FAILED(result = ParametersValidate(foundedParams, TransformXmlParamTable, WXSIZEOF(TransformXmlParamTable))))
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' have not all mandatory subtags line %d: 0x%08x"),
						m_spLogger
						, pNode->GetName().GetData().AsInternal()
						, pNode->GetLineNumber()
						, result
						, FWGLOG_ENDVAL);
		return result;
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
		} else if(child->GetName() == GAME_TAG_COMP_PHYSICS) {	
			name.Clear();
			if( !spEntity->m_physDef.IsEmpty())
			{
				FWGLOG_ERROR_FORMAT(wxT("Physics component is already defined for this entity on line: %d")
								, m_spLogger, child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_OBJECT_ALREADY_EXISTS_ERROR;
			}
			
			if(FWG_FAILED(result = CreatePhysics(child, spEntity->m_physDef)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Create physics component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
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


GameErrorCode GameXmlResourceLoader::GetParameter(wxXmlNode* pNode, const ParamDefinition* pDefTable, wxInt32 tableSize, void* pObject)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxString paramName;
	wxString paramValue;
	
	
	if(pNode->GetType() == wxXML_COMMENT_NODE)
	{
		return FWG_NO_ERROR;
	}
	
	paramName.assign(pNode->GetName());
	
	const ParamDefinition* pParDef = nullptr;
		
	for(pParDef = pDefTable; pParDef < &pDefTable[tableSize]; ++pParDef)
	{
		if(paramName.CmpNoCase(pParDef->m_paramName) == 0)
		{
			break;
		}
	}
	
	if(pParDef == &pDefTable[tableSize])
	{
		FWGLOG_ERROR_FORMAT(wxT("Unknown tag ['%s'] on line: %d"),
								m_spLogger,
								pNode->GetName().GetData().AsInternal(),
								pNode->GetLineNumber(),
								FWGLOG_ENDVAL);
		return FWG_E_XML_UNKNOWN_TAG_ERROR;
	}
	
	// get content
	wxXmlNode* pChild = pNode->GetChildren();
	if ( (pChild == nullptr) || (pChild->GetType() != wxXML_TEXT_NODE) )
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag with name '%s' has no text content on line: %d"),
						m_spLogger, paramName.GetData().AsInternal(), pNode->GetLineNumber(), FWGLOG_ENDVAL);
		return FWG_E_XML_INVALID_TAG_ERROR;
	} else {
		paramValue = pChild->GetContent();
	}
	
	// prepare base address
	char *pObjectMember = static_cast<char*>(pObject) + pParDef->m_structOffset;
	
	// get data from text
	switch (pParDef->m_type)
	{
		case GAMEDEF_TYPE_TEXT:
		{
			wxString *pString = reinterpret_cast<wxString*>(pObjectMember);
			pString->assign(paramValue);
			break;
		}
		case GAMEDEF_TYPE_INT:
		{
			wxInt32 *pInt32 = reinterpret_cast<wxInt32*>(pObjectMember);
			
			if(FWG_FAILED(result = GameXmlUtils::ConvertToInt32(paramValue, *pInt32)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Cannot convert '%s' to int in tag '%s' on line: %d"),
						m_spLogger,
						paramValue.GetData().AsInternal(),
						pNode->GetName().GetData().AsInternal(),
						pNode->GetLineNumber(),						
						FWGLOG_ENDVAL);
				return result;
			}
			
			break;
		}
		case GAMEDEF_TYPE_DWORD:
		{
			wxDword *pDword = reinterpret_cast<wxDword*>(pObjectMember);
			
			if(FWG_FAILED(result = GameXmlUtils::ConvertToDword(paramValue, *pDword)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Cannot convert '%s' to dword in tag '%s' on line: %d"),
						m_spLogger,
						paramValue.GetData().AsInternal(),
						pNode->GetName().GetData().AsInternal(),
						pNode->GetLineNumber(),						
						FWGLOG_ENDVAL);
				return result;
			}
			
			break;
		}
		case GAMEDEF_TYPE_FLOAT:
		{
			float *pFloat = reinterpret_cast<float*>(pObjectMember);
			
			if(FWG_FAILED(result = GameXmlUtils::ConvertToFloat(paramValue, *pFloat)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Cannot convert '%s' to float in tag '%s' on line: %d"),
						m_spLogger,
						paramValue.GetData().AsInternal(),
						pNode->GetName().GetData().AsInternal(),
						pNode->GetLineNumber(),						
						FWGLOG_ENDVAL);
				return result;
			}
			
			break;
		}
		case GAMEDEF_TYPE_VEC3:
		{
			Ogre::Vector3 *pVec3 = reinterpret_cast<Ogre::Vector3*>(pObjectMember);
			if(FWG_FAILED(result = GameXmlUtils::ConvertToVec3(paramValue, pVec3)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Cannot convert '%s' to vector in tag '%s' on line: %d"),
						m_spLogger,
						paramValue.GetData().AsInternal(),
						pNode->GetName().GetData().AsInternal(),
						pNode->GetLineNumber(),						
						FWGLOG_ENDVAL);
				return result;
			}
			break;
		}
		case GAMEDEF_TYPE_QUAT:
		{
			Ogre::Quaternion *pQuat = reinterpret_cast<Ogre::Quaternion*>(pObjectMember);
			if(FWG_FAILED(result = GameXmlUtils::ConvertToQuat(paramValue, pQuat)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Cannot convert '%s' to quaternion in tag '%s' on line: %d"),
						m_spLogger,
						paramValue.GetData().AsInternal(),
						pNode->GetName().GetData().AsInternal(),
						pNode->GetLineNumber(),						
						FWGLOG_ENDVAL);
				return result;
			}
			break;
		}
		default:
			FWGLOG_ERROR_FORMAT(wxT("Parameter with name '%s' in tag has unknown type - line: %d"),
						m_spLogger, paramName.GetData().AsInternal(), pNode->GetLineNumber(), FWGLOG_ENDVAL);
			return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	return result;
}

GameErrorCode GameXmlResourceLoader::ParametersValidate(GameBasSet<wxString>& foundedParams, const ParamDefinition* pDefTable, wxInt32 tableSize)
{
	
	const ParamDefinition *pParDef = nullptr;
	
	for(pParDef = pDefTable; pParDef < &pDefTable[tableSize]; ++pParDef)
	{
		if(pParDef->m_mandatory)
		{
			if(!foundedParams.Exists(wxString(pParDef->m_paramName)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Parameter '%s' does not exists!"),
						m_spLogger, pParDef->m_paramName, FWGLOG_ENDVAL);
				return FWG_E_XML_TAG_NOT_FOUND_ERROR;				
			}
		}
	}
	
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

GameErrorCode GameXmlResourceLoader::CreatePhysics(wxXmlNode* pNode, RefObjSmPtr<PhysCompDef>& spPhysics)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<PhysCompDef> spPhysicsTemp;
	wxString typeStr;
	
	GameBasSet<wxString> foundedParams;
	
	FWG_RETURN_FAIL( GameNewChecked(spPhysicsTemp.OutRef()));
	
	if(!pNode->GetAttribute(wxString(GAME_ATTR_TYPE_STR), &typeStr))
	{
		FWGLOG_ERROR_FORMAT(wxT("Attribut parameter '%s' not found ( tag '%s' on line %d )"), m_spLogger
							, GAME_ATTR_TYPE_STR
							, pNode->GetName().GetData().AsInternal()
							, pNode->GetLineNumber()
							, FWGLOG_ENDVAL);
		return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
	}
	
	if(FWG_FAILED(result = ConvertToPhysicsType(typeStr, reinterpret_cast<wxInt32&>(spPhysicsTemp->m_physType))))
	{
		FWGLOG_ERROR_FORMAT(wxT("Attribut parameter '%s' contain unknown type ( tag '%s' on line %d )"), m_spLogger
							, GAME_ATTR_TYPE_STR
							, pNode->GetName().GetData().AsInternal()
							, pNode->GetLineNumber()
							, FWGLOG_ENDVAL);
		return result;
	}
	
	wxXmlNode *child = pNode->GetChildren();
	while (child)
	{
		if (child->GetName() == GAME_TAG_COMP_SHAPE) {

			wxString paramName;
			if( !spPhysicsTemp->m_shape.IsEmpty())
			{
				FWGLOG_ERROR_FORMAT(wxT("Shape is already defined for this physics component on line: %d")
								, m_spLogger, child->GetLineNumber(), FWGLOG_ENDVAL);
				return FWG_E_OBJECT_ALREADY_EXISTS_ERROR;
			}
			
			if(FWG_FAILED(result = CreatePhysicsShape(child, spPhysicsTemp->m_shape)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Create physics shape failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
			
		} else {

			// set meshName as found
			if(FWG_FAILED(result = GetParameter(child, PhysicsXmlParamTable, WXSIZEOF(PhysicsXmlParamTable), (void*)spPhysicsTemp.In())))
			{
				FWGLOG_ERROR_FORMAT(wxT("Unknown tag within tag ['%s'] on line: %d"),
								m_spLogger,
								pNode->GetName().GetData().AsInternal(),
								pNode->GetLineNumber(),
								FWGLOG_ENDVAL);
				return result;
			}
			
			if(foundedParams.Exists(child->GetName()))
			{
				FWGLOG_ERROR_FORMAT(wxT("Parameter '%s' already exists ( tag '%s' on line %d )"), m_spLogger
							, child->GetName().GetData().AsInternal()
							, pNode->GetName().GetData().AsInternal()
							, pNode->GetLineNumber()
							, FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_ATTR_ERROR;
			}
			
			if(child->GetType() == wxXML_ELEMENT_NODE)
			{
				FWG_RETURN_FAIL(foundedParams.Insert(child->GetName()));
			}

		}
		
		child = child->GetNext();
		
	}
	
	if (FWG_FAILED(result = ParametersValidate(foundedParams, PhysicsXmlParamTable, WXSIZEOF(PhysicsXmlParamTable))))
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' have not all mandatory subtags line %d: 0x%08x"),
						m_spLogger
						, pNode->GetName().GetData().AsInternal()
						, pNode->GetLineNumber()
						, result
						, FWGLOG_ENDVAL);
		return result;
	}
	
	// set return value
	spPhysics = spPhysicsTemp;
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreatePhysicsShape(wxXmlNode* pNode, RefObjSmPtr<PhysShapeDef>& spPhysicsShape)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<PhysShapeDef> spShapeTemp;
	wxString typeStr;
	
	GameBasSet<wxString> foundedParams;
	
	FWG_RETURN_FAIL( GameNewChecked(spShapeTemp.OutRef()));
	
	if(!pNode->GetAttribute(wxString(GAME_ATTR_TYPE_STR), &typeStr))
	{
		FWGLOG_ERROR_FORMAT(wxT("Attribut parameter '%s' not found ( tag '%s' on line %d )"), m_spLogger
							, GAME_ATTR_TYPE_STR
							, pNode->GetName().GetData().AsInternal()
							, pNode->GetLineNumber()
							, FWGLOG_ENDVAL);
		return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
	}
	
	if(FWG_FAILED(result = ConvertToShapeType(typeStr, reinterpret_cast<wxInt32&>(spShapeTemp->m_shapeType))))
	{
		FWGLOG_ERROR_FORMAT(wxT("Attribut parameter '%s' contain unknown type ( tag '%s' on line %d )"), m_spLogger
							, GAME_ATTR_TYPE_STR
							, pNode->GetName().GetData().AsInternal()
							, pNode->GetLineNumber()
							, FWGLOG_ENDVAL);
		return result;
	}
	
	
	
	wxXmlNode *child = pNode->GetChildren();
	while (child)
	{
		if (child->GetName() == GAME_TAG_COMP_SHAPE) 
		{

			// not implemented yet - posibility for 
			return FWG_E_NOT_IMPLEMENTED_ERROR;
			//wxString paramName;
			//if( !spPhysicsTemp->m_shape.IsEmpty())
			//{
			//	FWGLOG_ERROR_FORMAT(wxT("Shape is already defined for this physics component on line: %d")
			//					, m_spLogger, child->GetLineNumber(), FWGLOG_ENDVAL);
			//	return FWG_E_OBJECT_ALREADY_EXISTS_ERROR;
			//}
			//
			//if(FWG_FAILED(result = CreatePhysicsShape(child, spPhysicsTemp->m_shape)))
			//{
			//	FWGLOG_ERROR_FORMAT(wxT("Create physics shape failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
			//	return result;
			//}
			
		} else {

			// set meshName as found
			if(FWG_FAILED(result = GetParameter(child, PhysicsShapeXmlParamTable, WXSIZEOF(PhysicsShapeXmlParamTable), (void*)spShapeTemp.In())))
			{
				FWGLOG_ERROR_FORMAT(wxT("Unknown tag within tag ['%s'] on line: %d"),
								m_spLogger,
								pNode->GetName().GetData().AsInternal(),
								pNode->GetLineNumber(),
								FWGLOG_ENDVAL);
				return result;
			}
			
			if(foundedParams.Exists(child->GetName()))
			{
				FWGLOG_ERROR_FORMAT(wxT("Parameter '%s' already exists ( tag '%s' on line %d )"), m_spLogger
							, child->GetName().GetData().AsInternal()
							, pNode->GetName().GetData().AsInternal()
							, pNode->GetLineNumber()
							, FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_ATTR_ERROR;
			}
			
			if(child->GetType() == wxXML_ELEMENT_NODE)
			{
				FWG_RETURN_FAIL(foundedParams.Insert(child->GetName()));
			}

		}
		
		child = child->GetNext();
		
	}
	
	if (FWG_FAILED(result = ParametersValidate(foundedParams, PhysicsShapeXmlParamTable, WXSIZEOF(PhysicsShapeXmlParamTable))))
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' have not all mandatory subtags line %d: 0x%08x"),
						m_spLogger
						, pNode->GetName().GetData().AsInternal()
						, pNode->GetLineNumber()
						, result
						, FWGLOG_ENDVAL);
		return result;
	}
	
	// set return value
	spPhysicsShape = spShapeTemp;
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::ConvertToShapeType(const wxString& input, wxInt32& retType)
{
	const PhysicsShapeTypeConvertRow *pConv = nullptr;
	const PhysicsShapeTypeConvertRow *endPoint = &PhysicsShapeTypeConvertTable[WXSIZEOF(PhysicsShapeTypeConvertTable)];
	for (pConv = PhysicsShapeTypeConvertTable; pConv != endPoint; ++pConv)
	{
		if(input.Cmp(pConv->m_typeName) == 0)
		{
			retType = pConv->m_typeEnum;
			return FWG_NO_ERROR;
		}
	}
	
	return FWG_E_XML_INVALID_ATTR_ERROR;
}

GameErrorCode GameXmlResourceLoader::ConvertToPhysicsType(const wxString& input, wxInt32& retType)
{
	const PhysicsTypeConvertRow *pConv = nullptr;
	const PhysicsTypeConvertRow *endPoint = &PhysicsTypeConvertTable[WXSIZEOF(PhysicsTypeConvertTable)];
	for (pConv = PhysicsTypeConvertTable; pConv != endPoint; ++pConv)
	{
		if(input.Cmp(pConv->m_typeName) == 0)
		{
			retType = pConv->m_typeEnum;
			return FWG_NO_ERROR;
		}
	}
	
	return FWG_E_XML_INVALID_ATTR_ERROR;
}


GameErrorCode GameXmlResourceLoader::ConvertToInputType(const wxString& input, InputDef::InputType& retType)
{
	const InputTypeConvertRow *pConv = nullptr;
	const InputTypeConvertRow *endPoint = &InputTypeConvertTable[WXSIZEOF(InputTypeConvertTable)];
	for (pConv = InputTypeConvertTable; pConv != endPoint; ++pConv)
	{
		if(input.Cmp(pConv->m_typeName) == 0)
		{
			retType = pConv->m_typeEnum;
			return FWG_NO_ERROR;
		}
	}
	
	return FWG_E_XML_INVALID_ATTR_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreateTerrain(wxXmlNode* pNode, RefObjSmPtr<TerrainDef>& spTerrain)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<TerrainDef> spTerrainTemp;
	
	GameBasSet<wxString> foundedParams;
	
	FWG_RETURN_FAIL( GameNewChecked(spTerrainTemp.OutRef()));
	
	wxXmlNode *child = pNode->GetChildren();
	while (child)
	{
		if (child->GetName() == GAME_TAG_COMP_TERRAIN_PAGES) 
		{
			// create terrain pages
			wxXmlNode *pTerrainPages = child->GetChildren();
			
			while(pTerrainPages)
			{
				if(pTerrainPages->GetName() == GAME_TAG_COMP_TERRAIN_PAGE)
				{
					RefObjSmPtr<TerrainPage> spTerrainPage;
					FWG_RETURN_FAIL(GameNewChecked(spTerrainPage.OutRef()));
					if(FWG_FAILED(result = CreateTerrainPage(pTerrainPages, spTerrainPage)))
					{
						FWGLOG_ERROR_FORMAT(wxT("Create terrain page failed on line: %d with error: 0x%08x"),
									m_spLogger,
									pNode->GetLineNumber(),
									result,
									FWGLOG_ENDVAL);
						return result;
					}
					// add terrain page
					spTerrainTemp->m_terrainPages.push_back(spTerrainPage);
				} else if(pTerrainPages->GetType() != wxXML_COMMENT_NODE) {
					FWGLOG_ERROR_FORMAT(wxT("Unknown tag within tag ['%s'] on line: %d"),
									m_spLogger,
									pNode->GetName().GetData().AsInternal(),
									pNode->GetLineNumber(),
									FWGLOG_ENDVAL);
					return FWG_E_XML_INVALID_TAG_ERROR;
				}
				pTerrainPages = pTerrainPages->GetNext();
			}
		} else {

			// set meshName as found
			if(FWG_FAILED(result = GetParameter(child, TerrainXmlParamTable, WXSIZEOF(TerrainXmlParamTable), (void*)spTerrainTemp.In())))
			{
				FWGLOG_ERROR_FORMAT(wxT("Unknown tag within tag ['%s'] on line: %d"),
								m_spLogger,
								pNode->GetName().GetData().AsInternal(),
								pNode->GetLineNumber(),
								FWGLOG_ENDVAL);
				return result;
			}
			
			if(foundedParams.Exists(child->GetName()))
			{
				FWGLOG_ERROR_FORMAT(wxT("Parameter '%s' already exists ( tag '%s' on line %d )"), m_spLogger
							, child->GetName().GetData().AsInternal()
							, pNode->GetName().GetData().AsInternal()
							, pNode->GetLineNumber()
							, FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_ATTR_ERROR;
			}
			
			if(child->GetType() == wxXML_ELEMENT_NODE)
			{
				FWG_RETURN_FAIL(foundedParams.Insert(child->GetName()));
			}
		}
		
		child = child->GetNext();
		
	}
	
	if (FWG_FAILED(result = ParametersValidate(foundedParams, TerrainXmlParamTable, WXSIZEOF(TerrainXmlParamTable))))
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' have not all mandatory subtags line %d: 0x%08x"),
						m_spLogger
						, pNode->GetName().GetData().AsInternal()
						, pNode->GetLineNumber()
						, result
						, FWGLOG_ENDVAL);
		return result;
	}
	
	// set return value
	spTerrain = spTerrainTemp;
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::CreateTerrainPage(wxXmlNode* pNode, RefObjSmPtr<TerrainPage>& spTerrainPage)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<TerrainPage> spTerrainPageTemp;
	
	GameBasSet<wxString> foundedParams;
	
	FWG_RETURN_FAIL( GameNewChecked(spTerrainPageTemp.OutRef()));
	
	wxXmlNode *child = pNode->GetChildren();
	while (child)
	{
		{

			// set meshName as found
			if(FWG_FAILED(result = GetParameter(child, TerrainPageXmlParamTable, WXSIZEOF(TerrainPageXmlParamTable), (void*)spTerrainPageTemp.In())))
			{
				FWGLOG_ERROR_FORMAT(wxT("Unknown tag within tag ['%s'] on line: %d"),
								m_spLogger,
								pNode->GetName().GetData().AsInternal(),
								pNode->GetLineNumber(),
								FWGLOG_ENDVAL);
				return result;
			}
			
			if(foundedParams.Exists(child->GetName()))
			{
				FWGLOG_ERROR_FORMAT(wxT("Parameter '%s' already exists ( tag '%s' on line %d )"), m_spLogger
							, child->GetName().GetData().AsInternal()
							, pNode->GetName().GetData().AsInternal()
							, pNode->GetLineNumber()
							, FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_ATTR_ERROR;
			}
			
			if(child->GetType() == wxXML_ELEMENT_NODE)
			{
				FWG_RETURN_FAIL(foundedParams.Insert(child->GetName()));
			}

		}
		
		child = child->GetNext();
		
	}
	
	if (FWG_FAILED(result = ParametersValidate(foundedParams, TerrainPageXmlParamTable, WXSIZEOF(TerrainPageXmlParamTable))))
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' have not all mandatory subtags line %d: 0x%08x"),
						m_spLogger
						, pNode->GetName().GetData().AsInternal()
						, pNode->GetLineNumber()
						, result
						, FWGLOG_ENDVAL);
		return result;
	}
	
	// set return value
	spTerrainPage = spTerrainPageTemp;
	
	return FWG_NO_ERROR;
}

