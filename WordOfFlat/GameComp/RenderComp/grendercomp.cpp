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
	} else {
		return FWG_E_OBJECT_NOT_EXIST_ERROR;
	}
	
	return FWG_NO_ERROR;
}

void RenderComponent::DisconnectRenderable(Ogre::Renderable* pRenderable)
{
	if(pRenderable != nullptr)
	{
		pRenderable->getUserObjectBindings().setUserAny(Ogre::UserObjectBindings::getEmptyUserAny()); // erase parent
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
}

void RenderComponent::ConnectTransformComp(TransformComponent& transform)
{
}

void RenderComponent::RemoveRenderObject(RenderObject* pObject)
{
}
