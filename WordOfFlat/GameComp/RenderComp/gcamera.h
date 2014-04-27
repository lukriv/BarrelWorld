#ifndef __GAME_CAMERA_H__
#define __GAME_CAMERA_H__


#include <OGRE/OgreCamera.h>
#include <OGRE/OgreSceneManager.h>
#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>


class GameCamera : public RefObjectImpl<IRefObject> {
private:
	Ogre::Camera *m_pCamera;
public:
	GameCamera(Ogre::Camera *pCamera) : m_pCamera(pCamera) {}
	
	inline Ogre::Camera* GetOgreCamera() { return m_pCamera; }
	
	inline Ogre::SceneNode* GetCameraNode() { return m_pCamera->getParentSceneNode(); }
	
protected:
	~GameCamera() 
	{
		if(m_pCamera != NULL)
		{
			// remove scene node from its parent
			Ogre::SceneNode* pSceneNode = m_pCamera->getParentSceneNode();
			if(pSceneNode->getParentSceneNode() != NULL)
			{
				pSceneNode->getParentSceneNode()->removeChild(pSceneNode);
			}
			// delete parent scene node
			m_pCamera->getSceneManager()->destroySceneNode(pSceneNode);
			// delete camera
			m_pCamera->getSceneManager()->destroyCamera(m_pCamera);
			m_pCamera = NULL;
		}
	}
	
};




#endif //__GAME_CAMERA_H__