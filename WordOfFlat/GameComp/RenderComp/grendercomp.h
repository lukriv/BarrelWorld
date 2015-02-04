#ifndef __GAME_RENDER_COMPONENT_H__
#define __GAME_RENDER_COMPONENT_H__

#include <OGRE/OgreAny.h>
#include <wx/vector.h>
#include "wx/thread.h"
#include <GameSystem/gerror.h>
#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>
#include <GameSystem/refobjectsmptr.h>
#include <GameSystem/gset.h>
#include "../gcompbase.h"
#include "../transformComp/gtranscomp.h"

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
class RenderComponent : public ComponentBase, public Ogre::Any {
private:
	typedef GameBasSet<RenderObject*> TRenderObjectList;
	typedef wxVector<TaskMessage> TMessageList;
protected:
	RenderCompManager *m_pOwnerManager;
	Ogre::SceneNode *m_pSceneNode;
	GameEntity *m_pParent;
	wxCriticalSection m_renderLock;
	
	RefObjSmPtr<TransformComponent> m_spTransform;
	
	TRenderObjectList m_renderObjectList;
	
	TMessageList m_receivedMessages;
	bool m_alreadyInUpdateQueue;
protected:
	GameErrorCode ConnectRenderComponent(Ogre::MovableObject* pObject);
	void DisconnectRenderComponent(Ogre::MovableObject* pObject);
public:

	// Render component can be created and destroyed only by render component manager
	RenderComponent(RenderCompManager* pCompManager) : ComponentBase(GAME_COMP_RENDER)
													, m_pOwnerManager(pCompManager)
													, m_pSceneNode(nullptr)
													, m_pParent(nullptr)
													, m_alreadyInUpdateQueue(false){}
	~RenderComponent();
	
	GameErrorCode Initialize(GameEntity* pParentEntity);
	
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

	/**
	 * \brief Process update if it is necessary
	 */
	void ProcessUpdate();
	// update methods
	
	virtual GameErrorCode ReceiveMessage(TaskMessage& msg);
	
	virtual GameErrorCode ReinitComponent(GameEntity* pNewParentEntity);
	
	virtual GameErrorCode Update();
	
};

#endif //__GAME_RENDER_COMPONENT_H__