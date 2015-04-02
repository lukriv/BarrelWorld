#include "gentitycreator.h"

static const wxChar* TERRAIN_DECAL_LOGIC = wxT("TERRAIN_DECAL");

GameErrorCode GameEntityCreator::CreateTerrainDecal(EntityDef*& pTerrDecalDef)
{
	RefObjSmPtr<EntityDef> spEntityDef;
	RefObjSmPtr<ManualObjectDef> spManualRenderDef;
	FWG_RETURN_FAIL (GameNewChecked(spEntityDef.OutRef()));
	
	// create transformation
	FWG_RETURN_FAIL (GameNewChecked(spEntityDef->m_transformation.OutRef()));
	
	FWG_RETURN_FAIL (GameNewChecked(spEntityDef->m_logicDef.OutRef()));
	
	spEntityDef->m_logicDef->m_logicType = wxT("TERRAIN_DECAL");
	
	FWG_RETURN_FAIL (GameNewChecked(spEntityDef->m_renderDef.OutRef()));
	
	FWG_RETURN_FAIL (GameNewChecked(spManualRenderDef.OutRef()));
	
	spManualRenderDef->m_manualObjectType = wxT("TerrainDecal");
	
	FWG_RETURN_FAIL (GameNewChecked(spManualRenderDef->m_material.OutRef()));
	
	spManualRenderDef->m_material->m_name = wxT("crosshair");
	
	spEntityDef->m_renderDef->
	
	
	
	pTerrDecalDef = spEntityDef.Detach();
	return FWG_NO_ERROR;
}
