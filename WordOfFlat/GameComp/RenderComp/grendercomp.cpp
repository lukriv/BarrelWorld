#include "grendercomp.h"

#include <OGRE/OgreNode.h>
#include "grendercmgr.h"
#include "../gentity.h"
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

GameErrorCode RenderComponent::Initialize(const Ogre::Vector3& translate, const Ogre::Quaternion& rotation)
{
	if(m_pSceneNode != nullptr)
	{
		// component was already initialized
		return FWG_NO_ERROR;
	}
	
	m_pSceneNode = m_pOwnerManager->GetOgreSceneManager()->getRootSceneNode()->createChildSceneNode( translate, rotation);
	
	if(m_pSceneNode == nullptr) {
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}

	return FWG_NO_ERROR;
}

GameErrorCode RenderComponent::ReceiveMessage(TaskMessage& msg)
{
	wxCriticalSectionLocker lock(m_renderLock);
	
	m_receivedMessages.push_back(msg);
	
	return FWG_NO_ERROR;
}


GameErrorCode RenderComponent::Update()
{
	wxCriticalSectionLocker lock(m_renderLock);
	if(!m_alreadyInUpdateQueue)
	{
		//FWGLOG_DEBUG_FORMAT(wxT("Add to update queue: %s"), m_pOwnerManager->GetLogger(), m_pParent->GetName().GetData().AsInternal(), FWGLOG_ENDVAL);
		m_alreadyInUpdateQueue = true;
		return m_pOwnerManager->AddToUpdateQueue(this);
	}
	return FWG_NO_ERROR;
}



