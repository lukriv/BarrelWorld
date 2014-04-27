#include "grendercomp.h"
#include "grendercmgr.h"

RenderComponent::~RenderComponent()
{
	Clear();
}


void RenderComponent::Clear()
{
	if(m_pOgreEntity != NULL)
	{
		m_pOwnerManager->GetOgreSceneManager()->destroyMovableObject(m_pOgreEntity);
		m_pOgreEntity = NULL;
	}
	
	m_pOgreCamera.Release();
	m_pOgreLight.Release();
}


void RenderComponent::SetOgreEntity(Ogre::Entity* pEntity)
{
	if(m_pOgreEntity != NULL)
	{
		m_pOgreEntity->getUserObjectBindings().setUserAny(Ogre::UserObjectBindings::getEmptyUserAny()); // erase parent
		m_pOwnerManager->GetOgreSceneManager()->destroyMovableObject(m_pOgreEntity);
	}
	
	m_pOgreEntity = pEntity; // set new entity
	
	if(pEntity != NULL)
	{
		m_pOgreEntity->getUserObjectBindings().setUserAny(*this); // set parent
	}
}