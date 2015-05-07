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
	wxCriticalSection m_renderLock;
	
	TMessageList m_receivedMessages;
	bool m_alreadyInUpdateQueue;
protected:
	GameErrorCode ConnectRenderComponent(Ogre::MovableObject* pObject);
	void DisconnectRenderComponent(Ogre::MovableObject* pObject);
public:

	// Render component can be created and destroyed only by render component manager
	RenderComponent(GameComponentType compType, RenderCompManager* pCompManager) : ComponentBase(compType)
													, m_pOwnerManager(pCompManager)
													, m_alreadyInUpdateQueue(false){}
	~RenderComponent();
	
	/**
	 * \brief Process update if it is necessary
	 */
	virtual void ProcessUpdate() = 0;
	// update methods
	
	virtual GameErrorCode ReceiveMessage(TaskMessage& msg);
};

#endif //__GAME_RENDER_COMPONENT_H__