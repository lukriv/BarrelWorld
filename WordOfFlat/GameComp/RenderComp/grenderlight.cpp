#include "grenderlight.h"



GameLight::~GameLight()
{
	Ogre::SceneManager* pSceneManager =  m_pLight->_getManager();
	if(pSceneManager)
	{
		pSceneManager->destroyMovableObject(m_pLight);
		m_pLight = nullptr;
	}
}


Ogre::MovableObject* GameLight::GetMovableObject()
{
	return m_pLight;
}