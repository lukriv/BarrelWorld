#include "grenderpositionmgr.h"

#include "grendersystem.h"
#include "grenderposition.h"
#include "../TransformComp/gtranscomp.h"

#include <GameSystem/new.h>


RenderPositionManager::RenderPositionManager(GameRenderSystem *pRenderSystem, GameEntityManager *pEntityMgr) : GameManagerBase(pRenderSystem->GetLogger(), pEntityMgr),
	m_pRenderSystem(pRenderSystem)
{
}

RenderPositionManager::~RenderPositionManager()
{
}

GameErrorCode RenderPositionManager::CreateComponent(wxDword compId, TransformComponent* pTrans, RenderPosition*& pRenderComp)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<RenderPosition> spRenderPosComp;
	FWG_RETURN_FAIL( GameNewChecked(spRenderPosComp.OutRef(), m_pRenderSystem) );
	
	if(FWG_FAILED(result = spRenderPosComp->Initialize(pTrans)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Render position component initialize failed: 0x%08x"), GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	FWG_RETURN_FAIL(InsertToMap(compId, spRenderPosComp));
	
	pRenderComp = spRenderPosComp.Detach();

	return result;
}
