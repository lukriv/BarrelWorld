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

static const wxChar* FACTORY_INPUT_UP		 = wxT("up");
static const wxChar* FACTORY_INPUT_DOWN		 = wxT("down");
static const wxChar* FACTORY_INPUT_LEFT		 = wxT("left");
static const wxChar* FACTORY_INPUT_RIGHT	 = wxT("right");
static const wxChar* FACTORY_INPUT_FORWARD	 = wxT("forward");
static const wxChar* FACTORY_INPUT_BACKWARD	 = wxT("backward");


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
	if(m_spInputSystem.IsEmpty()) return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	
	if(compMgr.GetRenderManager().GetOgreSceneManager() != NULL)
	{
		GameErrorCode result = FWG_NO_ERROR;
		if(!entityDef.m_renderDef.IsEmpty())
		{

			
			spRenderComp->SetParent(&entity);
			entity.SetRenderComp(spRenderComp);
		}
		
		if(!entityDef.m_transformation.IsEmpty())
		{
			RefObjSmPtr<TransformComponent> spTransform = NULL;
			Ogre::SceneNode *pSceneNode = compMgr.GetRenderManager().GetOgreSceneManager()->getRootSceneNode()->createChildSceneNode();
			pSceneNode->setPosition(entityDef.m_transformation->m_position);
			pSceneNode->setOrientation(entityDef.m_transformation->m_rotation);
			pSceneNode->setScale(entityDef.m_transformation->m_scale);
			
			FWG_RETURN_FAIL(GameNewChecked(spTransform.OutRef()));
			spTransform->SetSceneNode(pSceneNode);
			
			entity.SetTransformComp(spTransform);
		}
		
		if(!entityDef.m_inputDef.IsEmpty())
		{
			RefObjSmPtr<InputComponent> spInputComp;
			FWG_RETURN_FAIL(GameNewChecked(spInputComp.OutRef()));
			
			for(InputDef::TInputMap::ConstIterator iter = entityDef.m_inputDef->m_inputMap.Begin();
				iter != entityDef.m_inputDef->m_inputMap.End();
				iter++)
			{
			
				if(iter->first.Cmp(FACTORY_INPUT_UP) == 0)
				{
					if(FWG_FAILED(result = m_spInputSystem.In()->RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																			, spInputComp.In()
																			, &InputComponent::SetMoveUp))) 
					{
						FWGLOG_ERROR_FORMAT(wxT("Register input callback moveUp failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
						return result;
					}
				} else if (iter->first.Cmp(FACTORY_INPUT_DOWN) == 0) {
					if(FWG_FAILED(result = m_spInputSystem->RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																			, spInputComp.In()
																			, &InputComponent::SetMoveDown))) 
					{
						FWGLOG_ERROR_FORMAT(wxT("Register input callback moveDown failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
						return result;
					}
				} else if (iter->first.Cmp(FACTORY_INPUT_LEFT) == 0) {
					if(FWG_FAILED(result = m_spInputSystem->RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																			, spInputComp.In()
																			, &InputComponent::SetMoveLeft))) 
					{
						FWGLOG_ERROR_FORMAT(wxT("Register input callback moveLeft failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
						return result;
					}
				} else if (iter->first.Cmp(FACTORY_INPUT_RIGHT) == 0) {
					if(FWG_FAILED(result = m_spInputSystem->RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																			, spInputComp.In()
																			, &InputComponent::SetMoveRight))) 
					{
						FWGLOG_ERROR_FORMAT(wxT("Register input callback moveRight failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
						return result;
					}
				} else if (iter->first.Cmp(FACTORY_INPUT_FORWARD) == 0) {
					if(FWG_FAILED(result = m_spInputSystem->RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																			, spInputComp.In()
																			, &InputComponent::SetMoveForward))) 
					{
						FWGLOG_ERROR_FORMAT(wxT("Register input callback moveForward failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
						return result;
					}
				} else if (iter->first.Cmp(FACTORY_INPUT_BACKWARD) == 0) {
					if(FWG_FAILED(result = m_spInputSystem->RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																			, spInputComp.In()
																			, &InputComponent::SetMoveBackward))) 
					{
						FWGLOG_ERROR_FORMAT(wxT("Register input callback moveBackward failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
						return result;
					}
				} else {
					FWGLOG_WARNING_FORMAT(wxT("Cannot register unknown input action '%s'"), m_spLogger, iter->first.GetData().AsInternal(), FWGLOG_ENDVAL);
				}
			}
			
			entity.SetInputComp(spInputComp);
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
			
			entity.SetLogicComp(spLogicComp.In());
			if(FWG_FAILED(result = compMgr.GetLogicManager().AddLogicComp(spLogicComp.In())))
			{
				FWGLOG_ERROR_FORMAT(wxT("Added logic component to logic manager failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
		}
		
	}
	
	return FWG_NO_ERROR;
}


