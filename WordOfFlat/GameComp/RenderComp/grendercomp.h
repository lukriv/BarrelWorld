#ifndef __GAME_RENDER_COMPONENT_H__
#define __GAME_RENDER_COMPONENT_H__

#include <OGRE/OgreEntity.h>
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreLight.h>

class GameEntity;
class RenderCompManager;

enum RenderComponentType {
	RENDER_COMP_UNDEFINED 	= 0,
	RENDER_COMP_ENTITY		= 1,
	RENDER_COMP_CAMERA		= 2,
	RENDER_COMP_LIGHT		= 3
};


/*!
 * \class GameEntity
 * \author Lukas
 * \date 12.6.2013
 * \file gentityobj.h
 * \brief Geometric entity with state and transform
 */
class RenderComponent : public Ogre::Any {
protected:
	RenderCompManager *m_pOwnerManager;
	RenderComponentType m_compType;
	GameEntity *m_pParent;
	Ogre::MovableObject *m_pOgreObject;

protected:
	inline void SetOgreObject(Ogre::MovableObject *pOgreObject) 
	{ 
		if(m_pOgreObject != NULL)
		{
			m_pOgreObject->getUserObjectBindings().setUserAny(Ogre::UserObjectBindings::getEmptyUserAny()); // erase parent
		}
		
		m_pOgreObject = pOgreObject; // set new entity
		
		if(pOgreObject != NULL)
		{
			m_pOgreObject->getUserObjectBindings().setUserAny(*this); // set parent
		}
	}
	
public:
	RenderComponent(RenderCompManager* pCompManager) : m_pOwnerManager(pCompManager)
													, m_compType(RENDER_COMP_UNDEFINED)
													, m_pParent(NULL)
													, m_pOgreObject(NULL)  {}
	
	~RenderComponent() {}
	

	
	inline Ogre::MovableObject* GetOgreObject() { return m_pOgreObject; }
	
	inline Ogre::Entity* GetOgreEntity() 
	{
		if(m_compType == RENDER_COMP_ENTITY)
		{
			return static_cast<Ogre::Entity*>(m_pOgreObject);
		} else {
			return NULL;
		}
	}
	
	inline void SetOgreEntity(Ogre::Entity* pEntity)
	{
		if(pEntity != NULL)
		{
			m_compType = RENDER_COMP_ENTITY;
		} else {
			m_compType = RENDER_COMP_UNDEFINED;
		}
		SetOgreObject(pEntity);
	}
	
	inline Ogre::Camera* GetOgreCamera() 
	{
		if(m_compType == RENDER_COMP_CAMERA)
		{
			return static_cast<Ogre::Camera*>(m_pOgreObject);
		} else {
			return NULL;
		}
	}
	
	inline void SetOgreCamera(Ogre::Camera* pCamera)
	{
		if(pCamera != NULL)
		{
			m_compType = RENDER_COMP_CAMERA;
		} else {
			m_compType = RENDER_COMP_UNDEFINED;
		}
		SetOgreObject(pCamera);
	}
	
	inline Ogre::Light* GetOgreLight()
	{
		if(m_compType == RENDER_COMP_LIGHT)
		{
			return static_cast<Ogre::Light*>(m_pOgreObject);
		} else {
			return NULL;
		}
	}
	
	inline void SetOgreLight(Ogre::Light* pLight)
	{
		if(pLight != NULL)
		{
			m_compType = RENDER_COMP_LIGHT;
		} else {
			m_compType = RENDER_COMP_UNDEFINED;
		}
		SetOgreObject(pLight);
	}

	
	inline void SetParent(GameEntity *pParent) { m_pParent = pParent; }
	inline GameEntity* GetParent() { return m_pParent; }
	
};


#endif //__GAME_RENDER_COMPONENT_H__