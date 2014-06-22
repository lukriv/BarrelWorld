#include "grendercomp.h"
#include "grendercmgr.h"

RenderComponent::~RenderComponent()
{
	Clear();
}



void RenderComponent::ConnectRenderable(Ogre::Renderable* pRenderable)
{
	if(pRenderable != nullptr)
	{
		pRenderable->getUserObjectBindings().setUserAny(*this);
	}
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
