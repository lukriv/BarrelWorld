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
protected:
	typedef wxVector<TaskMessage> TMessageList;
protected:
	RenderCompManager *m_pOwnerManager;
	Ogre::SceneNode *m_pSceneNode;
	
	wxCriticalSection m_renderLock;
	
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
													, m_alreadyInUpdateQueue(false){}
	~RenderComponent();
	
	GameErrorCode Initialize(const Ogre::Vector3& tranlate, const Ogre::Quaternion& rotation);
	
	/*!
	 * \brief Destroy inner ogre object
	 */
	void Clear();

	/**
	 * \brief Process update if it is necessary
	 */
	virtual void ProcessUpdate() = 0;
	// update methods
	
	virtual GameErrorCode ReceiveMessage(TaskMessage& msg);
	
	virtual GameErrorCode ReinitComponent();
	
};

#endif //__GAME_RENDER_COMPONENT_H__