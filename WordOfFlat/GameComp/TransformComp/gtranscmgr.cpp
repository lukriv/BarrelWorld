#include "gtranscmgr.h"
#include <GameSystem/new.h>
#include "gtranscomp.h"



TransformCompManager::TransformCompManager(GameLogger* pLogger) : m_spLogger(pLogger)
{}

TransformCompManager::~TransformCompManager()
{}

GameErrorCode TransformCompManager::CreateEmptyTransformComponent(TransformComponent*& pTransformComp)
{
	RefObjSmPtr<TransformComponent> spTransComp;
	GameErrorCode result = FWG_NO_ERROR;
	FWG_RETURN_FAIL( GameNewChecked(spTransComp.OutRef()) );
	if(FWG_FAILED(result = spTransComp->Initialize()))
	{
		FWGLOG_ERROR_FORMAT(wxT("Tranform component initialize failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	pTransformComp = spTransComp.Detach();
	
	return FWG_NO_ERROR;
}

GameErrorCode TransformCompManager::CreateTransformComponent(const TransformDef& transCompDef, TransformComponent*& pTransformComp)
{
	RefObjSmPtr<TransformComponent> spTransComp;
	
	FWG_RETURN_FAIL(CreateEmptyTransformComponent(spTransComp.OutRef()));
	
	spTransComp->GetData()->m_translate = Vectormath::Aos::Vector3(transCompDef.m_position.x, transCompDef.m_position.y, transCompDef.m_position.z);
	spTransComp->GetData()->m_scale = Vectormath::Aos::Vector3(transCompDef.m_scale.x, transCompDef.m_scale.y, transCompDef.m_scale.z);
	
	spTransComp->GetData()->m_rotation.setX(transCompDef.m_rotation.x);
	spTransComp->GetData()->m_rotation.setY(transCompDef.m_rotation.y);
	spTransComp->GetData()->m_rotation.setZ(transCompDef.m_rotation.z);
	spTransComp->GetData()->m_rotation.setW(transCompDef.m_rotation.w);
	
	pTransformComp = spTransComp.Detach();

	return FWG_NO_ERROR;
	
}