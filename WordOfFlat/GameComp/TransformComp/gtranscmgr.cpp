#include "gtranscmgr.h"
#include <GameSystem/new.h>
#include <GameComp/gentity.h>
#include "gtranscomp.h"




TransformCompManager::TransformCompManager(GameLogger* pLogger) : m_spLogger(pLogger)
{}

TransformCompManager::~TransformCompManager()
{}

GameErrorCode TransformCompManager::CreateTransformComponent(TransformComponent *&pTransComp, const btVector3& position, const btVector3& scale, const btQuaternion& rotation)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<TransformComponent> spTransComp;
	FWG_RETURN_FAIL( GameNewChecked(spTransComp.OutRef()) );
	
	if(FWG_FAILED(result = spTransComp->Initialize()))
	{
		FWGLOG_ERROR_FORMAT(wxT("Tranform component initialize failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	spTransComp->GetData()->m_translate = position;
	spTransComp->GetData()->m_scale = scale;
	spTransComp->GetData()->m_rotation = rotation;

	
	pTransComp = spTransComp.Detach();

	return result;
	
}

GameErrorCode TransformCompManager::CreateTransformComponent(TransformComponent*& pTransComp)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<TransformComponent> spTransComp;
	FWG_RETURN_FAIL( GameNewChecked(spTransComp.OutRef()) );
	
	if(FWG_FAILED(result = spTransComp->Initialize()))
	{
		FWGLOG_ERROR_FORMAT(wxT("Tranform component initialize failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	pTransComp = spTransComp.Detach();

	return result;
}
