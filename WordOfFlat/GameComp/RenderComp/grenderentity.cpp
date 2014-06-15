#include "grenderentity.h"


void RenderEntity::~RenderEntity()
{
	if(m_pOgreEntity != nullptr)
	{
		m_pOwnerManager->GetOgreSceneManager()->destroyMovableObject(m_pOgreEntity);
		m_pOgreEntity = NULL;
	}
}





void ConnectTransformComp(TransformComponent &transform)
{
	transform.GetSceneNode()->attachObject(m_pOgreEntity);
}

Ogre::MovableObject* RenderEntity::GetMovableObject() 
{
	return m_pOgreEntity;
}
