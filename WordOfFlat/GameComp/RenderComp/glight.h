#ifndef __GAME_LIGHT_H__
#define __GAME_LIGHT_H__



#include <OGRE/OgreLight.h>
#include <OGRE/OgreSceneManager.h>
#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>


class GameLight : public RefObjectImpl<IRefObject> {
private:
	Ogre::Light *m_pLight;
public:
	GameLight(Ogre::Light *pLight) : m_pLight(pLight) {}
	
	inline Ogre::Light* GetOgreLight() { return m_pLight; }
	
	inline Ogre::SceneNode* GetLightNode() { return m_pLight->getParentSceneNode(); }
	
protected:
	~GameLight()
	{
		if(m_pLight != NULL)
		{
			// remove scene node from its parent
			Ogre::SceneNode* pSceneNode = m_pLight->getParentSceneNode();
			if(pSceneNode->getParentSceneNode() != NULL)
			{
				pSceneNode->getParentSceneNode()->removeChild(pSceneNode);
			}
			// delete parent scene node
			m_pLight->_getManager()->destroySceneNode(pSceneNode);
			// delete camera
			m_pLight->_getManager()->destroyLight(m_pLight);
			m_pLight = NULL;
		}
	}
	
};




#endif //__GAME_LIGHT_H__