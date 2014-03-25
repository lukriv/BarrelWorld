#include "gtestloader.h"
#include "../GameSystem/new.h"


GameErrorCode GameTestResourceLoader::Initialize(GameLogger* pLogger)
{
	if (m_isInitialized) {
		return FWG_NO_ERROR;
	}

	m_spLogger = pLogger;

	m_isInitialized = true;
	return FWG_NO_ERROR;
}


GameErrorCode GameTestResourceLoader::Load(GameDefinitionHolder& defHolder)
{
	if(!m_isInitialized)
	{
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	
	return FWG_NO_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadAnimations(GameDefinitionHolder& defHolder)
{
	return FWG_NO_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadAnimators(GameDefinitionHolder& defHolder)
{
	return FWG_NO_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadEntities(GameDefinitionHolder& defHolder)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<EntityDef> spEntityDef;
	
	FWG_RETURN_FAIL(GameNewChecked(spEntityDef.OutRef()));
	
	if(defHolder.m_meshDefs.KeyExists(wxString(wxT("TestingCube"))))
	{
		spEntityDef->m_mesh = *defHolder.m_meshDefs.FindValue(wxString(wxT("TestingCube")));
	} else {
		return FWG_E_OBJECT_NOT_FOUND_ERROR;
	}
	
	if(defHolder.m_materialDefs.KeyExists(wxString(wxT("testMaterial"))))
	{
		spEntityDef->m_material = *defHolder.m_materialDefs.FindValue(wxString(wxT("testMaterial")));
	} else {
		return FWG_E_OBJECT_NOT_FOUND_ERROR;
	}
	
	FWG_RETURN_FAIL(GameNewChecked(spEntityDef->m_transformation.OutRef()));
	
	spEntityDef->m_transformation->m_position[0] = 0.0f;
	spEntityDef->m_transformation->m_position[1] = 0.0f;
	spEntityDef->m_transformation->m_position[2] = 0.0f;

	if(FWG_FAILED(result = defHolder.m_entityDefs.Insert( wxString(wxT("entity1")), spEntityDef )))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameTestResourceLoader::LoadEntities() : add entity failed: 0x%08x"),
																m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	
	//spEntityDef.Attach(new (std::nothrow) EntityDef);
	//if(spEntityDef.IsEmpty())
	//{
	//	return FWG_E_MEMORY_ALLOCATION_ERROR;
	//}
	
	return FWG_NO_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadMaterials(GameDefinitionHolder& defHolder)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<NameDef> spMaterialDef;
	
	FWG_RETURN_FAIL(GameNewChecked(spMaterialDef.OutRef()));
	
	spMaterialDef->m_name = wxT("Test/ColourTest");
	
	if(FWG_FAILED(result = defHolder.m_materialDefs.Insert( wxString(wxT("testMaterial")), spMaterialDef )))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameTestResourceLoader::LoadMaterials() : add item failed: 0x%08x"),
																m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadMeshes(GameDefinitionHolder& defHolder)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<NameDef> spMeshDef;
	
	FWG_RETURN_FAIL(GameNewChecked(spMeshDef.OutRef()));
	
	spMeshDef->m_name = wxT("TestingCube");
	
	if(FWG_FAILED(result = defHolder.m_meshDefs.Insert( wxString(wxT("testCubeMesh")), spMeshDef )))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameTestResourceLoader::LoadMeshes() : add item failed: 0x%08x"),
																m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	return FWG_NO_ERROR;
}
