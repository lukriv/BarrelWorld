#include "gpropertymgr.h"
#include "gpropertycomp.h"

PropertyCompManager::PropertyCompManager(GameLogger* pLogger, GameEntityManager* pEntityMgr) : GameManagerBase(pLogger, pEntityMgr)
{
}

PropertyCompManager::~PropertyCompManager()
{
}

GameErrorCode PropertyCompManager::CreateComponent(wxDword compId, PropertyComponent*& pPropComp)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<PropertyComponent> spPropComp;
	FWG_RETURN_FAIL( GameNewChecked(spPropComp.OutRef(), GetLogger()) );
	
	FWG_RETURN_FAIL(InsertToMap(compId, spPropComp));
	
	pPropComp = spPropComp.Detach();

	return result;
}
