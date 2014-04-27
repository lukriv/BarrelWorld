#ifndef __GAME_RENDER_COMPONENT_H__
#define __GAME_RENDER_COMPONENT_H__

#include <OGRE/OgreEntity.h>
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreLight.h>

#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>
#include <GameSystem/refobjectsmptr.h>
#include "gcamera.h"
#include "glight.h"

class GameEntity;
class RenderCompManager;

/*!
 * \class GameEntity
 * \author Lukas
 * \date 12.6.2013
 * \file gentityobj.h
 * \brief Geometric entity with state and transform
 */
class RenderComponent : public RefObjectImpl<IRefObject>, public Ogre::Any {
protected:
	RenderCompManager *m_pOwnerManager;
	GameEntity *m_pParent;
	
	Ogre::Entity *m_pOgreEntity; //!< Main render object (it is optional)
	RefObjSmPtr<GameCamera> m_pOgreCamera; //!< Camera with SceneNode following main object (optional)
	RefObjSmPtr<GameLight>  m_pOgreLight;  //!< Light with SceneNode following main object (optional)

public:
	// Render component can be created and destroyed only by render component manager
	RenderComponent(RenderCompManager* pCompManager) : m_pOwnerManager(pCompManager)
													, m_pParent(NULL)
													, m_pOgreEntity(NULL){}
	~RenderComponent();
	
	inline Ogre::Entity* GetOgreEntity() 
	{
		return m_pOgreEntity;
	}
	
	void SetOgreEntity(Ogre::Entity* pEntity);
	
	inline GameCamera* GetOgreCamera() 
	{
		return m_pOgreCamera;
	}
	
	void SetOgreCamera(GameCamera* pCamera)
	{
		m_pOgreCamera = pCamera;
	}
	
	inline GameLight* GetOgreLight()
	{
		return m_pOgreLight;
	}
	
	inline void SetOgreLight(GameLight* pLight)
	{
		m_pOgreLight = pLight;
	}

	
	inline void SetParent(GameEntity *pParent) { m_pParent = pParent; }
	inline GameEntity* GetParent() { return m_pParent; }
	
	/*!
	 * \brief Destroy inner ogre object
	 */
	void Clear();
	
};


#endif //__GAME_RENDER_COMPONENT_H__