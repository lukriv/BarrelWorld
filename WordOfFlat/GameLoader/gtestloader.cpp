#include "gtestloader.h"
#include <GameSystem/new.h>
#include <OIS/OISKeyboard.h>


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
	
	if(FWG_FAILED(result = LoadCameras(defHolder)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Load cameras definitions failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = LoadRenderObj(defHolder)))
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
	
	if(defHolder.m_renderDefs.Exists(wxString(wxT("render1"))))
	{
		spEntityDef.In()->m_renderDef = *defHolder.m_renderDefs.FindValue(wxString(wxT("render1")));
	} else {
		return FWG_E_OBJECT_NOT_FOUND_ERROR;
	}
	
	if(defHolder.m_logicDefs.Exists(wxString(wxT("manualLogic"))))
	{
		spEntityDef.In()->m_logicDef = *defHolder.m_logicDefs.FindValue(wxString(wxT("manualLogic")));
	} else {
		return FWG_E_OBJECT_NOT_FOUND_ERROR;
	}
	
	if(defHolder.m_inputDefs.Exists(wxString(wxT("testInput"))))
	{
		spEntityDef.In()->m_inputDef = *defHolder.m_inputDefs.FindValue(wxString(wxT("testInput")));
	} else {
		return FWG_E_OBJECT_NOT_FOUND_ERROR;
	}
	
	FWG_RETURN_FAIL(GameNewChecked(spEntityDef->m_transformation.OutRef()));
	
	spEntityDef->m_transformation->m_position[0] = 0.0f;
	spEntityDef->m_transformation->m_position[1] = 0.0f;
	spEntityDef->m_transformation->m_position[2] = 0.0f;
	

	
	if(FWG_FAILED(result = defHolder.InsertDef<EntityDef>( wxString(wxT("entity1")), spEntityDef, defHolder.m_entityDefs )))
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

GameErrorCode GameTestResourceLoader::LoadRenderObj(GameDefinitionHolder& defHolder)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<RenderDef> spRenderDef;
	
	FWG_RETURN_FAIL(GameNewChecked(spRenderDef.OutRef()));
	
	if(defHolder.m_meshDefs.Exists(wxString(wxT("testCubeMesh"))))
	{
		spRenderDef->m_mesh = *defHolder.m_meshDefs.FindValue(wxString(wxT("testCubeMesh")));
	} else {
		return FWG_E_OBJECT_NOT_FOUND_ERROR;
	}
	
	if(defHolder.m_materialDefs.Exists(wxString(wxT("testMaterial"))))
	{
		spRenderDef->m_material = *defHolder.m_materialDefs.FindValue(wxString(wxT("testMaterial")));
	} else {
		return FWG_E_OBJECT_NOT_FOUND_ERROR;
	}
	
	if(FWG_FAILED(result = defHolder.InsertDef<RenderDef>( wxString(wxT("render1")), spRenderDef, defHolder.m_renderDefs )))
	{
		FWGLOG_ERROR_FORMAT(wxT("Add entity failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadMaterials(GameDefinitionHolder& defHolder)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<NameDef> spMaterialDef;
	
	FWG_RETURN_FAIL(GameNewChecked(spMaterialDef.OutRef()));
	
	spMaterialDef->m_name = wxT("Test/ColourTest");
	
	if(FWG_FAILED(result = defHolder.InsertDef<NameDef>( wxString(wxT("testMaterial")), spMaterialDef, defHolder.m_materialDefs )))
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
	
	if(FWG_FAILED(result = defHolder.InsertDef<NameDef>( wxString(wxT("testCubeMesh")), spMeshDef, defHolder.m_meshDefs )))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameTestResourceLoader::LoadMeshes() : add item failed: 0x%08x"),
																m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadInput(GameDefinitionHolder& defHolder)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<InputDef> spInputDef;
	
	FWG_RETURN_FAIL(GameNewChecked(spInputDef.OutRef()));
	
	spInputDef.In()->m_moveUp = OIS::KC_W;
	spInputDef.In()->m_moveDown = OIS::KC_S;
	spInputDef.In()->m_moveLeft = OIS::KC_A;
	spInputDef.In()->m_moveRight = OIS::KC_D;
	
	if(FWG_FAILED(result = defHolder.InsertDef<InputDef>( wxString(wxT("testInput")), spInputDef, defHolder.m_inputDefs )))
	{
		FWGLOG_ERROR_FORMAT(wxT("Add item failed: 0x%08x"),	m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	return FWG_NO_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadLogic(GameDefinitionHolder& defHolder)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<LogicDef> spLogicDef;
	
	FWG_RETURN_FAIL(GameNewChecked(spLogicDef.OutRef()));
	
	spLogicDef.In()->m_logicType = LogicDef::LOGIC_TYPE_MANUAL_TEST;
	
	if(FWG_FAILED(result = defHolder.InsertDef<LogicDef>( wxString(wxT("manualLogic")), spLogicDef, defHolder.m_logicDefs )))
	{
		FWGLOG_ERROR_FORMAT(wxT("Add item failed: 0x%08x"),	m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	return FWG_NO_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadCameras(GameDefinitionHolder& defHolder)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<CameraDef> spCameraDef;
	
	FWG_RETURN_FAIL(GameNewChecked(spCameraDef.OutRef()));
	
	if(FWG_FAILED(result = defHolder.InsertDef<CameraDef>( wxString(wxT("gamePlayCam")), spCameraDef, defHolder.m_cameraDefs )))
	{
		FWGLOG_ERROR_FORMAT(wxT("Add item failed: 0x%08x"),	m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
		
	return FWG_NO_ERROR;
}
