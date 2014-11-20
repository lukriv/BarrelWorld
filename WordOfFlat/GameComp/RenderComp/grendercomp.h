#ifndef __GAME_RENDER_COMPONENT_H__
#define __GAME_RENDER_COMPONENT_H__

#include <OGRE/OgreAny.h>
#include <GameSystem/gerror.h>
#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>
#include <GameSystem/refobjectsmptr.h>
#include <GameSystem/gset.h>

class RenderObject;
class RenderCompManager;
class GameEntity;

/*!
 * \class GameEntity
 * \author Lukas
 * \date 12.6.2013
 * \file gentityobj.h
 * \brief Geometric entity with state and transform
 */
class RenderComponent : public RefObjectImpl<IRefObject>, public Ogre::Any {
private:
	typedef GameBasSet<RenderObject*> TRenderObjectList;
protected:
	RenderCompManager *m_pOwnerManager;
	GameEntity *m_pParent;
	Ogre::SceneNode *m_pSceneNode;
	
	TRenderObjectList m_renderObjectList;

protected:
	/*!
	 * \brief Disconnect ogre renderable from transform component
	 *
	 * Disconnect ogre renderable from render component 
	 * 
	 * \param pRenderable
	 */
	void DisconnectRenderComponent(Ogre::MovableObject *pObject);
	
	/*!
	 * \brief Connect ogre renderable to other objects
	 * 
	 * It binds ogre object to this render component.
	 * It connects ogre object to render component (it there is some within game entity)
	 * 
	 * \param pRenderable 
	 * \return 
	 */
	GameErrorCode ConnectRenderComponent(Ogre::MovableObject *pObject);
	
	
public:
	// Render component can be created and destroyed only by render component manager
	RenderComponent(RenderCompManager* pCompManager) : m_pOwnerManager(pCompManager)
													, m_pParent(nullptr)
													, m_pSceneNode(nullptr){}
	~RenderComponent();
	
	GameErrorCode Initialize();
	
	inline void SetParent(GameEntity *pParent) { m_pParent = pParent; }
	inline GameEntity* GetParent() { return m_pParent; }
	
	/*!
	 * \brief Add new Render Object to Render Component
	 * 
	 * To Render Component can be attached more than one object.
	 * 
	 * \param pObject Render Object
	 * \retval FWG_E_RENDER_OBJECT_ALREADY_ATTACHED_ERROR If Render Object is already attached to another Render Component.
	 * \retval Other error on some memory error
	 * \retval FWG_NO_ERROR Object was successfully attached.
	 */
	GameErrorCode AttachRenderObject(RenderObject* pObject);
	
	/*!
	 * \brief Remove render object from render component
	 * \param pObject
	 */
	void RemoveRenderObject(RenderObject* pObject);
	
	/*!
	 * \brief Destroy inner ogre object
	 */
	void Clear();

	// update mehtods
	GameErrorCode Update();
	
	GameErrorCode ProcessUpdate();
	
	
};

#endif //__GAME_RENDER_COMPONENT_H__