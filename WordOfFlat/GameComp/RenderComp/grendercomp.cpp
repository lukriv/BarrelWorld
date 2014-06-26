#include "grendercomp.h"
#include "grendercmgr.h"

RenderComponent::~RenderComponent()
{
	Clear();
}



GameErrorCode RenderComponent::ConnectRenderable(Ogre::Renderable* pRenderable)
{
	if(pRenderable != nullptr)
	{
		if(pObject->GetMovableObject()->getUserObjectBindings().getUserAny() != Ogre::UserObjectBindings::getEmptyUserAny())
		{
			GameErrorCode result = FWG_E_RENDER_OBJECT_ALREADY_ATTACHED_ERROR;
			FWGLOG_ERROR_FORMAT( wxT("Connect renderable failed: 0x%08x"), m_pOwnerManager->GetLogger(), result, FWGLOG_ENDVAL);
			return result;
		}
		pRenderable->getUserObjectBindings().setUserAny(*this);
		
		// connect to existing scene node component
		if(m_pParent != nullptr)&&(m_pParent->GetTransformComp() != nullptr)
		{
			m_pParent->GetTransformComp()->GetSceneNode()->attachObject(pRenderable);
		}
	} else {
		return FWG_E_OBJECT_NOT_EXIST_ERROR;
	}
	
	return FWG_NO_ERROR;
}

void RenderComponent::DisconnectRenderable(Ogre::Renderable* pRenderable)
{
	if(pRenderable != nullptr)
	{
		// erase parent
		pRenderable->getUserObjectBindings().setUserAny(Ogre::UserObjectBindings::getEmptyUserAny());
		// remove from transform component
		if(m_pParent != nullptr)&&(m_pParent->GetTransformComp() != nullptr)
		{
			m_pParent->GetTransformComp()->GetSceneNode()->detachObject(pRenderable);
		}
	}
}

GameErrorCode RenderComponent::AttachRenderObject(RenderObject* pObject)
{
	GameErrorCode result = FWG_NO_ERROR;
	
	// connect render object to the render component
	if(FWG_FAILED(result = ConnectRenderable(pObject->GetMovableObject())))
	{
		FWGLOG_ERROR_FORMAT( wxT("Connect render object to render component failed: 0x%08x"), m_pOwnerManager->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	// insert into set
	if(FWG_FAILED(result = m_renderObjectList.Insert(pObject)))
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
		DisconnectRenderable((*iter)->GetMovableObject());
		(*iter)->release();
	}
	
	m_renderObjectList.Clear();
}

void RenderComponent::ConnectTransformComp(TransformComponent& transform)
{
	TRenderObjectList::Iterator iter;
	for(iter = m_renderObjectList.Begin(); iter != m_renderObjectList.End(); iter++)
	{
		DisconnectRenderable((*iter)->GetMovableObject());
		(*iter)->release();
	}
}

void RenderComponent::RemoveRenderObject(RenderObject* pObject)
{
	
}
