#ifndef __GAME_CAMERA_H__
#define __GAME_CAMERA_H__


#include "grenderobj.h"


class GameCamera : public RenderObject {
private:
	Ogre::Camera *m_pCamera;
public:
	GameCamera(Ogre::Camera *pCamera) : m_pCamera(pCamera) {}
	
	inline Ogre::Camera* GetOgreCamera() { return m_pCamera; }
	
	
	virtual Ogre::MovableObject* GetMovableObject() override;
	

	
};




#endif //__GAME_CAMERA_H__