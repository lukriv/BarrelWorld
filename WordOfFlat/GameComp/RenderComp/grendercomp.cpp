#include "grendercomp.h"

#include <OGRE/OgreNode.h>
#include "grendercmgr.h"
#include "../gentity.h"
#include "grenderobj.h"
#include <GameComp/TransformComp/gtranscomp.h>

RenderComponent::~RenderComponent()
{
	Clear();
	if((m_pSceneNode)&&(m_pOwnerManager)) {
		m_pOwnerManager->GetOgreSceneManager()->destroySceneNode(m_pSceneNode);
		m_pSceneNode = nullptr;
	}
}



GameErrorCode RenderComponent::ConnectRenderComponent(Ogre::MovableObject* pObject)
{
	if(pObject != nullptr) {
		if(!pObject->getUserObjectBindings().getUserAny().isEmpty()) {
			GameErrorCode result = FWG_E_RENDER_OBJECT_ALREADY_ATTACHED_ERROR;
			FWGLOG_ERROR_FORMAT( wxT("Connect renderable failed: 0x%08x"), m_pOwnerManager->GetLogger(), result, FWGLOG_ENDVAL);
			return result;
		}
		
		m_pSceneNode->attachObject(pObject);
		FWGLOG_DEBUG(wxT("attach objects successful"), m_pOwnerManager->GetLogger());
		
		pObject->getUserObjectBindings().setUserAny(*this);
		FWGLOG_DEBUG(wxT("user bind object successful"), m_pOwnerManager->GetLogger());

	} else {
		return FWG_E_OBJECT_NOT_EXIST_ERROR;
	}

	return FWG_NO_ERROR;
}

void RenderComponent::DisconnectRenderComponent(Ogre::MovableObject* pObject)
{
	if(pObject != nullptr) {
		// erase parent
		pObject->getUserObjectBindings().setUserAny(Ogre::UserObjectBindings::getEmptyUserAny());
		m_pSceneNode->detachObject(pObject);
	}
}

GameErrorCode RenderComponent::AttachRenderObject(RenderObject* pObject)
{
	GameErrorCode result = FWG_NO_ERROR;

	// connect render object to the render component
	if(FWG_FAILED(result = ConnectRenderComponent(pObject->GetMovableObject()))) {
		FWGLOG_ERROR_FORMAT( wxT("Connect render object to render component failed: 0x%08x"), m_pOwnerManager->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}

	// insert into set
	if(FWG_FAILED(result = m_renderObjectList.Insert( pObject))) {
		FWGLOG_ERROR_FORMAT( wxT("Insert to list failed: 0x%08x"), m_pOwnerManager->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	} else {
		// add reference on success
		pObject->addRef();
	}

	return FWG_NO_ERROR;
}

void RenderComponent::Clear()
{
	TRenderObjectList::Iterator iter;
	for(iter = m_renderObjectList.Begin(); iter != m_renderObjectList.End(); ++iter) 
	{
		DisconnectRenderComponent((*iter)->GetMovableObject());
		(*iter)->release();
	}
	
	m_renderObjectList.Clear();
}

void RenderComponent::RemoveRenderObject(RenderObject* pObject)
{
	TRenderObjectList::Iterator iter;
	iter = m_renderObjectList.Find(pObject);
	if(iter != m_renderObjectList.End()) {
		DisconnectRenderComponent((*iter)->GetMovableObject());
		(*iter)->release();
	}
}

GameErrorCode RenderComponent::Initialize(GameEntity* pParentEntity)
{
	if(m_pSceneNode != nullptr)
	{
		// component was already initialized
		return FWG_NO_ERROR;
	}
	
	// parent entity cannot be null
	if(pParentEntity == nullptr)
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	m_spTransform = reinterpret_cast<TransformComponent*> (pParentEntity->GetComponent(GAME_COMP_TRANSFORM));
	if(m_spTransform.IsEmpty())
	{
		// transform component cannot be null - add transform component to entity at first
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	// initialize parent entity
	m_pParent = pParentEntity;
	
	TransformData* transData = m_spTransform->GetData();
	
	m_pSceneNode = m_pOwnerManager->GetOgreSceneManager()->getRootSceneNode()->createChildSceneNode(
		Ogre::Vector3(transData->m_translate.getX(), transData->m_translate.getY(), transData->m_translate.getZ())
		, Ogre::Quaternion(transData->m_rotation.getW(), transData->m_rotation.getX(), transData->m_rotation.getY(), transData->m_rotation.getZ()));
	
	if(m_pSceneNode == nullptr) {
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}

	return FWG_NO_ERROR;
}

GameErrorCode RenderComponent::ReceiveMessage(TaskMessage& msg)
{
	switch(msg.GetTaskType())
	{
		case GAME_TASK_TRANSFORM_UPDATE:
		{
			Update();
			break;
		}
		default:
			break;
	}
	return FWG_NO_ERROR;
}

GameErrorCode RenderComponent::ReinitComponent(GameEntity* pNewParentEntity)
{
	// lock component
	wxCriticalSectionLocker lock(m_renderLock);
	// get pointer to transform component if it is there
	if(pNewParentEntity == nullptr)
	{
		m_spTransform.Release();
		m_pParent = nullptr;
	} else {
		m_pParent = pNewParentEntity;
		m_spTransform = reinterpret_cast<TransformComponent*> (pNewParentEntity->GetComponent(GAME_COMP_TRANSFORM));
	}
	return FWG_NO_ERROR;
}

GameErrorCode RenderComponent::Update()
{
	if(!m_alreadyInUpdateQueue)
	{
		//FWGLOG_DEBUG_FORMAT(wxT("Add to update queue: %s"), m_pOwnerManager->GetLogger(), m_pParent->GetName().GetData().AsInternal(), FWGLOG_ENDVAL);
		m_alreadyInUpdateQueue = true;
		return m_pOwnerManager->AddToUpdateQueue(this);
	}
	return FWG_NO_ERROR;
}

void RenderComponent::ProcessUpdate()
{
	wxCriticalSectionLocker lock(m_renderLock);
	if(!m_spTransform.IsEmpty())
	{
		//todo: upgrade updating scene node transform
		m_pSceneNode->setPosition(m_spTransform->GetOgreTranlate());
		m_pSceneNode->setOrientation(m_spTransform->GetOgreRotation());
	}
	
	m_alreadyInUpdateQueue = false;
}
