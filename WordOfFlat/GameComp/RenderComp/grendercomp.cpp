#include "grendercomp.h"

#include "grendercmgr.h"
#include "../gentity.h"
#include "grenderobj.h"
#include <GameComp/TransformComp/gtranscomp.h>

RenderComponent::~RenderComponent()
{
	Clear();
	if((m_pSceneNode)&&(m_pOwnerManager))
	{
		m_pOwnerManager->GetOgreSceneManager()->destroySceneNode(m_pSceneNode);
		m_pSceneNode = nullptr;
	}
}



GameErrorCode RenderComponent::ConnectRenderComponent(Ogre::MovableObject* pObject)
{
	if(pObject != nullptr)
	{
		if(!pObject->getUserObjectBindings().getUserAny().isEmpty())
		{
			GameErrorCode result = FWG_E_RENDER_OBJECT_ALREADY_ATTACHED_ERROR;
			FWGLOG_ERROR_FORMAT( wxT("Connect renderable failed: 0x%08x"), m_pOwnerManager->GetLogger(), result, FWGLOG_ENDVAL);
			return result;
		}
		pObject->getUserObjectBindings().setUserAny(*this);
		m_pSceneNode->attachObject(pObject);
		
	} else {
		return FWG_E_OBJECT_NOT_EXIST_ERROR;
	}
	
	return FWG_NO_ERROR;
}

void RenderComponent::DisconnectRenderComponent(Ogre::MovableObject* pObject)
{
	if(pObject != nullptr)
	{
		// erase parent
		pObject->getUserObjectBindings().setUserAny(Ogre::UserObjectBindings::getEmptyUserAny());
		m_pRenderNode->detachObject(pObject);
	}
}

GameErrorCode RenderComponent::AttachRenderObject(RenderObject* pObject)
{
	GameErrorCode result = FWG_NO_ERROR;
	
	// connect render object to the render component
	if(FWG_FAILED(result = ConnectRenderComponent(pObject->GetMovableObject())))
	{
		FWGLOG_ERROR_FORMAT( wxT("Connect render object to render component failed: 0x%08x"), m_pOwnerManager->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	// insert into set
	if(FWG_FAILED(result = m_renderObjectList.Insert( pObject)))
	{
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
	for(iter = m_renderObjectList.Begin(); iter != m_renderObjectList.End(); iter++)
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
	if(iter != m_renderObjectList.End())
	{
		DisconnectRenderComponent((*iter)->GetMovableObject());
		(*iter)->release();
	}
}

GameErrorCode RenderComponent::Initialize()
{
	m_pSceneNode = m_pOwnerManager->GetOgreSceneManager()->getRootSceneNode()->createChildSceneNode();
	if(m_pSceneNode == nullptr)
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode RenderComponent::ProcessUpdate()
{
}
