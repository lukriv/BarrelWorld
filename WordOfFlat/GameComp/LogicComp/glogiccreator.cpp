#include "glogiccmgr.h"

#include "glogiccomp.h"
#include <GameResHold/gdeftables.h>
#include <GameComp/gentity.h>
#include "glogicman.h"
#include "glogicfreecam.h"


static const wxChar* FACTORY_LOGIC_TYPE_MANUAL_TEST 	= wxT("MANUAL_TEST");
static const wxChar* FACTORY_LOGIC_TYPE_FREE_CAMERA 	= wxT("FREE_CAMERA");
static const wxChar* FACTORY_LOGIC_TYPE_TERRAIN_DECAL 	= wxT("TERRAIN_DECAL");


GameErrorCode LogicCompManager::CreateLogicComp(LogicDef& logicDef, GameEntity* pEntity)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<LogicComponentBase> spLogicComp;
	if(logicDef.m_logicType.Cmp(FACTORY_LOGIC_TYPE_MANUAL_TEST) == 0)
	{
		LogicManualTest *pLogicMan = nullptr;
		FWG_RETURN_FAIL(GameNewChecked(pLogicMan));
		// prevent memory leak
		spLogicComp.Attach(pLogicMan);
		if(FWG_FAILED(result = pLogicMan->Initialize(pEntity)))
		{
			FWGLOG_ERROR_FORMAT(wxT("Initialize manual logic failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
	} else if(logicDef.m_logicType.Cmp(FACTORY_LOGIC_TYPE_FREE_CAMERA) == 0) {
		LogicFreeCamera *pLogicFreeCam = nullptr;
		FWG_RETURN_FAIL(GameNewChecked(pLogicFreeCam));
		// prevent memory leak
		spLogicComp.Attach(pLogicFreeCam);
		if(FWG_FAILED(result = pLogicFreeCam->Initialize(pEntity)))
		{
			FWGLOG_ERROR_FORMAT(wxT("Initialize free camera logic failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
	} else if(logicDef.m_logicType.Cmp(FACTORY_LOGIC_TYPE_TERRAIN_DECAL) == 0) {
		LogicTerrainDecal *pLogicTerrainDecal = nullptr;
		FWG_RETURN_FAIL(GameNewChecked(pLogicTerrainDecal));
		// prevent memory leak
		spLogicComp.Attach(pLogicTerrainDecal);
		if(FWG_FAILED(result = pLogicTerrainDecal->Initialize(m_pComponentManager->GetTerrainManager(), pEntity))
		{
			FWGLOG_ERROR_FORMAT(wxT("Initialize terrain decal logic failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
	} else {
		FWGLOG_ERROR_FORMAT(wxT("Unknown logic type '%s'"),
						m_spLogger, logicDef.m_logicType.GetData().AsInternal(), FWGLOG_ENDVAL);
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	if(FWG_FAILED(result = pEntity->AddComponent(spLogicComp)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Add logic component to entity failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL );
		return result;
	}
	
	if(FWG_FAILED(result = AddLogicComp(spLogicComp.In())))
	{
		FWGLOG_ERROR_FORMAT(wxT("Added logic component to logic manager failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	return result;
}