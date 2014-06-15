#ifndef __GAME_RENDER_COMPONENT_H__
#define __GAME_RENDER_COMPONENT_H__

#include <OGRE/OgreEntity.h>
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreLight.h>

#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>
#include <GameSystem/refobjectsmptr.h>
#include "grendercamera.h"
#include "grenderlight.h"

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
	RenderComponentType m_renderCompType;
	RenderCompManager *m_pOwnerManager;
	GameEntity *m_pParent;
	
	
	void DisconnectRenderable(Ogre::Renderable *pRenderable);
	void ConnectRenderable(Ogre::Renderable *pRenderable);
	
public:
	// Render component can be created and destroyed only by render component manager
	RenderComponent(RenderCompManager* pCompManager) : m_pOwnerManager(pCompManager)
													, m_pParent(NULL) {}
	~RenderComponent();
	
	inline RenderComponentType GetCompType() { return m_renderCompType; }

	inline void SetParent(GameEntity *pParent) { m_pParent = pParent; }
	inline GameEntity* GetParent() { return m_pParent; }
	
	void AddRenderObject(RenderObject* pObject);
	
	/*!
	 * \brief Destroy inner ogre object
	 */
	virtual void Clear() = 0;
	
	virtual void ConnectTransformComp(TransformComponent &transform) = 0;
	
	
};

#endif //__GAME_RENDER_COMPONENT_H__