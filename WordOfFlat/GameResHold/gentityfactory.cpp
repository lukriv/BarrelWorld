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


void GameEntityFactory::addRef()
{
	wxAtomicInc(m_refCount);
}

wxInt32 GameEntityFactory::release()
{
	wxInt32 refCount = wxAtomicDec(m_refCount);
	if (refCount == 0)
	{
		delete this;
	}
	return refCount;
}

GameErrorCode GameEntityFactory::Initialize(GameLogger* pLogger, GameInputSystem *pInputSystem)
{
	m_spLogger = pLogger;
	m_spInputSystem = pInputSystem;	
	return FWG_NO_ERROR;
}

void GameEntityFactory::Uninitialize()
{
	m_spInputSystem.Release();
	m_spLogger.Release();
}

GameErrorCode GameEntityFactory::CreateAllEntities(GameDefinitionHolder &defHolder, GameCompManager& compMgr)
{
	TEntityDefMap::Iterator iter;
	
	for (iter = defHolder.m_entityDefs.Begin(); iter != defHolder.m_entityDefs.End(); iter++)
	{
		GameEntity *pEntity = NULL;
		FWG_RETURN_FAIL(compMgr.GetEntityManager().CreateEntity(*(iter->second->GetName()), pEntity));
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
			RefObjSmPtr<RenderDef> spRenderDef = entityDef.m_renderDef;
			
			RefObjSmPtr<RenderComponent> spRenderComp;
			FWG_RETURN_FAIL(compMgr.GetRenderManager().CreateEmptyRenderComponent(spRenderComp.OutRef()));
			
			spRenderComp->SetParent(&entity);
			entity.SetRenderComp(spRenderComp);
			
			if(!spRenderDef->m_entities.empty())
			{
				wxVector< RefObjSmPtr<RenderEntityDef> >::iterator iter;
				for(iter = spRenderDef->m_entities.begin(); iter != spRenderDef->m_entities.end(); iter++)
				{
					// render entity is defined
					Ogre::Entity *pEntity = nullptr;
					RenderObject *pRenderObject = nullptr; 
					RefObjSmPtr<RenderObject> spRenderObject;
					
					
					if(entityDef.GetName() != nullptr)
					{
						pEntity = compMgr.GetRenderManager().GetOgreSceneManager()->createEntity((*iter)->GetName()->ToStdString(), (*iter)->m_mesh->m_name.ToStdString());
					} else {
						return FWG_E_NOT_IMPLEMENTED_ERROR;
					}
					
					//pEntity->setMaterialName(entityDef.m_material->m_name.ToStdString());
					pEntity->setMaterialName((*iter)->m_material->m_name.ToStdString());
					
					
					FWG_RETURN_FAIL(GameNewChecked(pRenderObject, pEntity));
					spRenderObject.Attach(pRenderObject);
					spRenderComp->AttachRenderObject(spRenderObject);
			
				}
							
			}
			
			if(!spRenderDef->m_cameras.empty())
			{
				
			}
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
			if(entityDef.m_inputDef.In()->m_moveUp != 0)
			{
				if(FWG_FAILED(result = m_spInputSystem.In()->RegisterCallback(static_cast<OIS::KeyCode>(entityDef.m_inputDef.In()->m_moveUp)
																		, spInputComp.In()
																		, &InputComponent::SetMoveUp))) 
				{
					FWGLOG_ERROR_FORMAT(wxT("Register input callback moveUp failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
					return result;
				}
			}
			
			if(entityDef.m_inputDef.In()->m_moveDown != 0)
			{
				if(FWG_FAILED(result = m_spInputSystem->RegisterCallback(static_cast<OIS::KeyCode>(entityDef.m_inputDef.In()->m_moveDown)
																		, spInputComp.In()
																		, &InputComponent::SetMoveDown))) 
				{
					FWGLOG_ERROR_FORMAT(wxT("Register input callback moveDown failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
					return result;
				}
			}
			
			if(entityDef.m_inputDef.In()->m_moveLeft != 0)
			{
				if(FWG_FAILED(result = m_spInputSystem->RegisterCallback(static_cast<OIS::KeyCode>(entityDef.m_inputDef.In()->m_moveLeft)
																		, spInputComp.In()
																		, &InputComponent::SetMoveLeft))) 
				{
					FWGLOG_ERROR_FORMAT(wxT("Register input callback moveLeft failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
					return result;
				}
			}
			
			if(entityDef.m_inputDef.In()->m_moveRight != 0)
			{
				if(FWG_FAILED(result = m_spInputSystem->RegisterCallback(static_cast<OIS::KeyCode>(entityDef.m_inputDef.In()->m_moveRight)
																		, spInputComp.In()
																		, &InputComponent::SetMoveRight))) 
				{
					FWGLOG_ERROR_FORMAT(wxT("Register input callback moveRight failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
					return result;
				}
			}
			
			if(entityDef.m_inputDef.In()->m_moveForward != 0)
			{
				if(FWG_FAILED(result = m_spInputSystem->RegisterCallback(static_cast<OIS::KeyCode>(entityDef.m_inputDef.In()->m_moveForward)
																		, spInputComp.In()
																		, &InputComponent::SetMoveForward))) 
				{
					FWGLOG_ERROR_FORMAT(wxT("Register input callback moveForward failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
					return result;
				}
			}
			
			if(entityDef.m_inputDef.In()->m_moveBackward != 0)
			{
				if(FWG_FAILED(result = m_spInputSystem->RegisterCallback(static_cast<OIS::KeyCode>(entityDef.m_inputDef.In()->m_moveBackward)
																		, spInputComp.In()
																		, &InputComponent::SetMoveBackward))) 
				{
					FWGLOG_ERROR_FORMAT(wxT("Register input callback moveBackward failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
					return result;
				}
			}
			
			entity.SetInputComp(spInputComp);
		}
		
		if(!entityDef.m_logicDef.IsEmpty())
		{
			RefObjSmPtr<LogicComponentBase> spLogicComp;
			switch(entityDef.m_logicDef.In()->m_logicType)
			{
				case LogicDef::LOGIC_TYPE_MANUAL_TEST:
				{
					LogicManualTest *pLogicMan = nullptr;
					FWG_RETURN_FAIL(GameNewChecked(pLogicMan));
					spLogicComp.Attach(pLogicMan);
					break;
				}
				default:
					break;
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

const wxChar* GameEntityFactory::ConvertCameraType2Name(CameraDef::CameraDefType cameraType) const
{
	switch(cameraType)
	{
		case CameraDef::GAMEDEF_CAM_MAIN:
			return MAIN_CAMERA_NAME;
		case CameraDef::GAMEDEF_CAM_SECONDARY:
			return SECONDARY_CAMERA_NAME;
		default:
			return nullptr;
	}
}
