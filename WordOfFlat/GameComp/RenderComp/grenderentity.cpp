#include "grenderentity.h"


void RenderEntity::~RenderEntity()
{
	if(m_pOgreEntity != nullptr)
	{
		Ogre::SceneManager *pSceneMgr = m_pOgreEntity->_getManager();
		if(pSceneMgr != nullptr)
		{
			m_pOwnerManager->GetOgreSceneManager()->destroyMovableObject(m_pOgreEntity);
			m_pOgreEntity = NULL;
		}
	}
}



Ogre::MovableObject* RenderEntity::GetMovableObject() 
{
	return m_pOgreEntity;
}
