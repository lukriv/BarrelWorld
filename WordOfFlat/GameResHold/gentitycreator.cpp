#include "gentitycreator.h"

GameErrorCode GameEntityCreator::CreateTerrainDecal(EntityDef*& pTerrDecalDef, const Ogre::Vector3 &initialPosition)
{
	RefObjSmPtr<EntityDef> spEntityDef;
	FWG_RETURN_FAIL (GameNewChecked(spEntityDef.OutRef()));
	
	// create transformation
	FWG_RETURN_FAIL (GameNewChecked(spEntityDef->m_transformation.OutRef()));
	
	spEntityDef->m_transformation->GetData()->m_translate = initialPosition;
	
	FWG_RETURN_FAIL (GameNewChecked(spEntityDef->))
	
	
	pTerrDecalDef = spEntityDef.Detach();
	return FWG_NO_ERROR;
}
