#include "gentityfactory.h"

#include <wx/scopedptr.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreSceneManager.h>
#include <GameSystem/refobjectsmptr.h>
#include <GameComp/RenderComp/grendercomp.h>
#include <GameComp/RenderComp/grenderobj.h>
#include <GameComp/InputComp/ginputcomp.h>
#include <GameComp/LogicComp/glogicman.h>
#include <GameSystem/new.h>




static const wxChar* FACTORY_LOGIC_TYPE_MANUAL_TEST = wxT("MANUAL_TEST");



GameErrorCode GameEntityFactory::CreateAllEntities(GameDefinitionHolder &defHolder, GameCompManager& compMgr)
{
	TEntityDefMap::Iterator iter;
	
	for (iter = defHolder.m_entityDefs.Begin(); iter != defHolder.m_entityDefs.End(); iter++)
	{
		GameEntity *pEntity = NULL;
		FWG_RETURN_FAIL(compMgr.GetEntityManager().CreateEntity(iter->second->GetName(), pEntity));
		FWG_RETURN_FAIL(CreateEntity(*(iter->second), compMgr, *pEntity));
	}
	
	return FWG_NO_ERROR;
}


GameErrorCode GameEntityFactory::CreateEntity( EntityDef& entityDef, GameCompManager& compMgr, GameEntity& entity)
{
	
	if(compMgr.GetRenderManager().GetOgreSceneManager() != nullptr)
	{
		GameErrorCode result = FWG_NO_ERROR;
		if(!entityDef.m_renderDef.IsEmpty())
		{
			RefObjSmPtr<RenderComponent> spRenderComp;
			if(FWG_FAILED(result = compMgr.GetRenderManager().CreateRenderComponent(*entityDef.m_renderDef, spRenderComp.OutRef())))
			{
				FWGLOG_ERROR_FORMAT(wxT("Create Render component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL );
				return result;
			}
        
			if(FWG_FAILED(result = entity.AddComponent(spRenderComp)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Add render component to entity failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL );
				return result;
			}
		}
		
		if(!entityDef.m_transformation.IsEmpty())
		{
			RefObjSmPtr<TransformComponent> spTransform;
			if(FWG_FAILED(result = compMgr.GetTransformManager().CreateTransformComponent(*entityDef.m_transformation, spTransform.OutRef())))
			{
				FWGLOG_ERROR_FORMAT(wxT("Create transform component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL );
				return result;
			}
			
			if(FWG_FAILED(result = entity.AddComponent(spTransform)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Add transform component to entity failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL );
				return result;
			}
		}
		
		if(!entityDef.m_inputDef.IsEmpty())
		{
			RefObjSmPtr<InputComponent> spInputComp;
			if(FWG_FAILED(result = compMgr.GetInputSystem().CreateAndRegisterInputComponent(*entityDef.m_inputDef, spInputComp.OutRef())))
			{
				FWGLOG_ERROR_FORMAT(wxT("Create input component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL );
				return result;
			}
			
			if(FWG_FAILED(result = entity.AddComponent(spInputComp)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Add input component to entity failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL );
				return result;
			}
		}
		
		if(!entityDef.m_logicDef.IsEmpty())
		{
			RefObjSmPtr<LogicComponentBase> spLogicComp;
			if(entityDef.m_logicDef->m_logicType.Cmp(FACTORY_LOGIC_TYPE_MANUAL_TEST) == 0)
			{
					LogicManualTest *pLogicMan = nullptr;
					FWG_RETURN_FAIL(GameNewChecked(pLogicMan));
					spLogicComp.Attach(pLogicMan);
			} else {
				FWGLOG_ERROR_FORMAT(wxT("Unknown logic type '%s'"),
								m_spLogger, entityDef.m_logicDef->m_logicType.GetData().AsInternal(), FWGLOG_ENDVAL);
				return FWG_E_INVALID_PARAMETER_ERROR;
			}
			
			if(FWG_FAILED(result = entity.AddComponent(spLogicComp)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Add logic component to entity failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL );
				return result;
			}
			
			if(FWG_FAILED(result = compMgr.GetLogicManager().AddLogicComp(spLogicComp.In())))
			{
				FWGLOG_ERROR_FORMAT(wxT("Added logic component to logic manager failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
		}
		
		// it is necessary to reinit component
		if(FWG_FAILED(result = entity.ReinitComponents()))
		{
			FWGLOG_ERROR_FORMAT(wxT("Reinit entity components failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
	}
	
	return FWG_NO_ERROR;
}


