#ifndef __GAME_LIGHT_H__
#define __GAME_LIGHT_H__



#include "grenderobj.h"


class GameLight : public RenderObject {
private:
	Ogre::Light *m_pLight;
public:
	GameLight(Ogre::Light *pLight) : RenderObject()
								, m_pLight(pLight) {}
	~GameLight();
	
	inline Ogre::Light* GetOgreLight() { return m_pLight; }
	
	/*!
	 * \brief Get Movable object
	 */
	virtual Ogre::MovableObject* GetMovableObject() override;
	
};




#endif //__GAME_LIGHT_H__