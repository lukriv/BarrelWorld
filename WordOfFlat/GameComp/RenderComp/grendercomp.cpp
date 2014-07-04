#include "grendercomp.h"

#include "grendercmgr.h"
#include "../gentity.h"
#include "grenderobj.h"
#include <GameComp/TransformComp/gtranscomp.h>

RenderComponent::~RenderComponent()
{
	Clear();
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
	}
}

GameErrorCode RenderComponent::ConnectTransformComponent(Ogre::MovableObject* pObject)
{
	if(pObject != nullptr)
	{
		// connect to existing scene node component
		if((m_pParent != nullptr)&&(m_pParent->GetTransformComp() != nullptr))
		{
			m_pParent->GetTransformComp()->GetSceneNode()->attachObject(pObject);
		}
	} else {
		return FWG_E_OBJECT_NOT_EXIST_ERROR;
	}
	
	return FWG_NO_ERROR;
}

void RenderComponent::DisconnectTransformComponent(Ogre::MovableObject* pObject)
{
	if(pObject != nullptr)
	{
		// remove from transform component
		if((m_pParent != nullptr)&&(m_pParent->GetTransformComp() != nullptr))
		{
			m_pParent->GetTransformComp()->GetSceneNode()->detachObject(pObject);
		}
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
	
	// connect render object to the transform component
	if(FWG_FAILED(result = ConnectTransformComponent(pObject->GetMovableObject())))
	{
		FWGLOG_ERROR_FORMAT( wxT("Connect render object to transform component failed: 0x%08x"), m_pOwnerManager->GetLogger(), result, FWGLOG_ENDVAL);
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
		DisconnectTransformComponent((*iter)->GetMovableObject());
		(*iter)->release();
	}
	
	m_renderObjectList.Clear();
}

void RenderComponent::ConnectTransformComp(TransformComponent& transform)
{
	TRenderObjectList::Iterator iter;
	for(iter = m_renderObjectList.Begin(); iter != m_renderObjectList.End(); iter++)
	{
		ConnectTransformComponent((*iter)->GetMovableObject());
	}
}

void RenderComponent::RemoveRenderObject(RenderObject* pObject)
{
	TRenderObjectList::Iterator iter;
	iter = m_renderObjectList.Find(pObject);
	if(iter != m_renderObjectList.End())
	{
		DisconnectRenderComponent((*iter)->GetMovableObject());
		DisconnectTransformComponent((*iter)->GetMovableObject());
		(*iter)->release();
	}
}


