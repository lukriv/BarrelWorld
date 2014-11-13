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
	
	// testing entity
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
	
	//camera entity
	FWG_RETURN_FAIL(GameNewChecked(spEntityDef.OutRef()));
	
	if(defHolder.m_renderDefs.Exists(wxString(wxT("mainCamera"))))
	{
		spEntityDef.In()->m_renderDef = *defHolder.m_renderDefs.FindValue(wxString(wxT("mainCamera")));
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
		spEntityDef.In()->m_inputDef = *defHolder.m_inputDefs.FindValue(wxString(wxT("cameraInput")));
	} else {
		return FWG_E_OBJECT_NOT_FOUND_ERROR;
	}
	
	FWG_RETURN_FAIL(GameNewChecked(spEntityDef->m_transformation.OutRef()));
	
	spEntityDef->m_transformation->m_position[0] = 0.0f;
	spEntityDef->m_transformation->m_position[1] = 0.0f;
	spEntityDef->m_transformation->m_position[2] = 0.0f;
	
	if(FWG_FAILED(result = defHolder.InsertDef<EntityDef>( wxString(wxT("mainCameraEntity")), spEntityDef, defHolder.m_entityDefs )))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameTestResourceLoader::LoadEntities() : add entity failed: 0x%08x"),
																m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	return FWG_NO_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadRenderDef(GameDefinitionHolder& defHolder)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<RenderDef> spRenderDef;
	
	// Create Entity
	FWG_RETURN_FAIL(GameNewChecked(spRenderDef.OutRef()));
	
	if(defHolder.m_renderEntityDefs.Exists(wxString(wxT("renderEnt1"))))
	{
		spRenderDef->m_entities.push_back(*defHolder.m_renderEntityDefs.FindValue(wxString(wxT("renderEnt1"))));
	} else {
		return FWG_E_OBJECT_NOT_FOUND_ERROR;
	}
	
	if(FWG_FAILED(result = defHolder.InsertDef<RenderDef>( wxString(wxT("render1")), spRenderDef, defHolder.m_renderDefs )))
	{
		FWGLOG_ERROR_FORMAT(wxT("Add entity failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	// Create Camera
	FWG_RETURN_FAIL(GameNewChecked(spRenderDef.OutRef()));
	
	if(defHolder.m_cameraDefs.Exists(wxString(wxT("MainCamera"))))
	{
		spRenderDef->m_cameras.push_back(*defHolder.m_cameraDefs.FindValue(wxString(wxT("MainCamera"))));
	} else {
		return FWG_E_OBJECT_NOT_FOUND_ERROR;
	}
	
	if(FWG_FAILED(result = defHolder.InsertDef<RenderDef>( wxString(wxT("mainCamera")), spRenderDef, defHolder.m_renderDefs )))
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
	
	FWG_RETURN_FAIL(spInputDef.In()->m_inputMap.Insert(wxT("up"), OIS::KC_W));
	FWG_RETURN_FAIL(spInputDef.In()->m_inputMap.Insert(wxT("down"), OIS::KC_S));
	FWG_RETURN_FAIL(spInputDef.In()->m_inputMap.Insert(wxT("left"), OIS::KC_A));
	FWG_RETURN_FAIL(spInputDef.In()->m_inputMap.Insert(wxT("right"), OIS::KC_D));
	
	if(FWG_FAILED(result = defHolder.InsertDef<InputDef>( wxString(wxT("testInput")), spInputDef, defHolder.m_inputDefs )))
	{
		FWGLOG_ERROR_FORMAT(wxT("Add item failed: 0x%08x"),	m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	// camera input
	FWG_RETURN_FAIL(GameNewChecked(spInputDef.OutRef()));
	
	FWG_RETURN_FAIL(spInputDef.In()->m_inputMap.Insert(wxT("up"), OIS::KC_I));
	FWG_RETURN_FAIL(spInputDef.In()->m_inputMap.Insert(wxT("down"), OIS::KC_K));
	FWG_RETURN_FAIL(spInputDef.In()->m_inputMap.Insert(wxT("left"), OIS::KC_J));
	FWG_RETURN_FAIL(spInputDef.In()->m_inputMap.Insert(wxT("right"), OIS::KC_L));
	
	if(FWG_FAILED(result = defHolder.InsertDef<InputDef>( wxString(wxT("cameraInput")), spInputDef, defHolder.m_inputDefs )))
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
	
	spLogicDef.In()->m_logicType = wxT("MANUAL_TEST");
	
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
	
	spCameraDef.In()->m_position = Ogre::Vector3(0, 10, -5);
	spCameraDef.In()->m_direction = Ogre::Vector3(0,-10, 5);
	
	if(FWG_FAILED(result = defHolder.InsertDef<CameraDef>( wxString(wxT("MainCamera")), spCameraDef, defHolder.m_cameraDefs )))
	{
		FWGLOG_ERROR_FORMAT(wxT("Add item failed: 0x%08x"),	m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
		
	return FWG_NO_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadRenderEntities(GameDefinitionHolder& defHolder)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<RenderEntityDef> spRenderEntityDef;
	
	// Create Render Entity
	FWG_RETURN_FAIL(GameNewChecked(spRenderEntityDef.OutRef()));
	
	if(defHolder.m_meshDefs.Exists(wxString(wxT("testCubeMesh"))))
	{
		spRenderEntityDef->m_mesh = *defHolder.m_meshDefs.FindValue(wxString(wxT("testCubeMesh")));
	} else {
		return FWG_E_OBJECT_NOT_FOUND_ERROR;
	}
	
	if(defHolder.m_materialDefs.Exists(wxString(wxT("testMaterial"))))
	{
		spRenderEntityDef->m_material = *defHolder.m_materialDefs.FindValue(wxString(wxT("testMaterial")));
	} else {
		return FWG_E_OBJECT_NOT_FOUND_ERROR;
	}
	
	if(FWG_FAILED(result = defHolder.InsertDef<RenderEntityDef>( wxString(wxT("renderEnt1")), spRenderEntityDef, defHolder.m_renderEntityDefs )))
	{
		FWGLOG_ERROR_FORMAT(wxT("Add render entity failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	return FWG_NO_ERROR;
}
