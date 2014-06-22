#include "grendercamera.h"



GameCamera::~GameCamera() 
{
	if(m_pCamera != nullptr)
	{
		// remove scene node from its parent
		Ogre::SceneManager *pSceneManager = m_pCamera->getSceneManager();
		if(pSceneManager != nullptr)
		{
			pSceneManager->destroyCamera(m_pCamera);
		}
		m_pCamera = nullptr;
	}
}

Ogre::MovableObject* GameCamera::GetMovableObject()
{
	return m_pCamera;
}
