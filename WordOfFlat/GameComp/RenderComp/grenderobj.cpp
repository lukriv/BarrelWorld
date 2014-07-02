#include "grenderobj.h"
#include <OGRE/OgreSceneManager.h>


RenderObject::~RenderObject()
{
	if(m_pObject != nullptr)
	{
		Ogre::SceneManager *pSceneMgr = m_pObject->_getManager();
		if(pSceneMgr != nullptr)
		{
			pSceneMgr->destroyMovableObject(m_pObject);
			m_pObject = nullptr;
		}
	}
}


