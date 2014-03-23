#include "gtestloader.h"
#include <map>
#include <wx/scopedptr.h>
#include <sfml/Graphics.hpp>
#include "../GameObjects/gsfmlgeom.h"


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
	spEntityDef.Attach(new (std::nothrow) EntityDef);
	if(spEntityDef.IsEmpty())
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	if(defHolder.m_meshDefs.ItemExists(wxString(wxT("TestingCube"))))
	{
		spEntityDef->m_mesh = defHolder.m_meshDefs.GetItem(wxString(wxT("TestingCube")));
	} else {
		return FWG_E_OBJECT_NOT_FOUND_ERROR;
	}
	
	if(defHolder.m_materialDefs.ItemExists(wxString(wxT("testMaterial"))))
	{
		spEntityDef->m_material = defHolder.m_materialDefs.GetItem(wxString(wxT("testMaterial")));
	} else {
		return FWG_E_OBJECT_NOT_FOUND_ERROR;
	}
	
	spEntityDef->m_transformation.Attach(new (std::nothrow) TransformDef());
	if(spEntityDef->m_transformation.IsEmpty())
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	spEntityDef->m_transformation->m_position[0] = 0.0f;
	spEntityDef->m_transformation->m_position[1] = 0.0f;
	spEntityDef->m_transformation->m_position[2] = 0.0f;
	
	
	
	spEntityDef.Attach(new (std::nothrow) EntityDef);
	if(spEntityDef.IsEmpty())
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode GameTestResourceLoader::LoadMaterials(GameDefinitionHolder& defHolder)
{
	RefObjSmPtr<NameDef> spMaterialDef;
	spMaterialDef.Attach(new (std::nothrow) NameDef);
	if(spMaterialDef.IsEmpty())
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	spMaterialDef->m_name = wxT("Test/ColourTest");
	
	if(FWG_FAILED(result = defHolder.m_materialDefs.AddItem( wxString(wxT("testMaterial")), 1, spMaterialDef )))
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
	spMeshDef.Attach(new (std::nothrow) NameDef);
	if(spMeshDef.IsEmpty())
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	spMeshDef->m_name = wxT("TestingCube");
	
	if(FWG_FAILED(result = defHolder.m_meshDefs.AddItem( wxString(wxT("testCubeMesh")), 1, spMeshDef )))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameTestResourceLoader::LoadMeshes() : add item failed: 0x%08x"),
																m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	return FWG_NO_ERROR;
}
