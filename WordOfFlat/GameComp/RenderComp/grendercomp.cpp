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

void RenderComponent::AddRenderObject(Ogre::MovableObject* pObject)
{
	


}
