#include "gtranscmgr.h"
#include <GameSystem/new.h>
#include <GameComp/gentity.h>

#include <bullet/LinearMath/btQuaternion.h>

#include "gtranscomp.h"




TransformCompManager::TransformCompManager(GameLogger* pLogger) : GameManagerBase(pLogger)
{}

TransformCompManager::~TransformCompManager()
{}

GameErrorCode TransformCompManager::CreateComponent(wxDword compId, TransformComponent*& pTransComp)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<TransformComponent> spTransComp;
	FWG_RETURN_FAIL( GameNewChecked(spTransComp.OutRef()) );
	
	if(FWG_FAILED(result = spTransComp->Initialize()))
	{
		FWGLOG_ERROR_FORMAT(wxT("Tranform component initialize failed: 0x%08x"), GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	FWG_RETURN_FAIL(InsertToMap(compId, spTransComp));
	
	pTransComp = spTransComp.Detach();

	return result;
}
