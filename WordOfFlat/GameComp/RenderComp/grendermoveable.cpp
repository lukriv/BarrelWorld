#include "grendermoveable.h"
#include "grendercmgr.h"
#include <OGRE/OgreSceneManager.h>

RenderMoveable::RenderMoveable(RenderCompManager *pCompMgr) : RenderComponent(GAME_COMP_RENDER_MOVEABLE, pCompMgr)
{
}

RenderMoveable::~RenderMoveable()
{
	Clear();
}

GameErrorCode RenderMoveable::Initialize(TransformComponent* pTransform)
{
	if(m_pSceneNode != nullptr) {
		// component was already initialized
		return FWG_NO_ERROR;
	}
	
	m_spTransform = pTransform;
	if(m_spTransform.IsEmpty()) 
	{
		// transform component cannot be null - add transform component to entity at first
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	TransformData* transData = m_spTransform->GetData();

    m_pSceneNode = m_pOwnerManager->GetOgreSceneManager()->getRootSceneNode()->createChildSceneNode(
					Ogre::Vector3(transData->m_translate.getX(), transData->m_translate.getY(), transData->m_translate.getZ())
					, Ogre::Quaternion(transData->m_rotation.getW(), transData->m_rotation.getX(), transData->m_rotation.getY(), transData->m_rotation.getZ()));

    if(m_pSceneNode == nullptr) {
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}

	return FWG_NO_ERROR;
}

void RenderMoveable::ProcessUpdate()
{
	wxCriticalSectionLocker lock(m_renderLock);

    TMessageList::iterator iter;
    for(iter = m_receivedMessages.begin(); iter != m_receivedMessages.end(); ++iter) {
		switch(iter->GetTaskType()) 
		{
		case GAME_TASK_TRANSFORM_UPDATE:
			if(!m_spTransform.IsEmpty()) {
				// todo: upgrade updating scene node transform
				m_pSceneNode->setPosition(m_spTransform->GetOgreTranslate());
				m_pSceneNode->setOrientation(m_spTransform->GetOgreRotation());
			}
			break;
		default:
			break;
		}
    }

    // clear received messages
    m_receivedMessages.clear();

    // not yet in update queue
    m_alreadyInUpdateQueue = false;
}

void RenderMoveable::Clear()
{
	if(m_pSceneNode)
	{
		m_pSceneNode->removeAndDestroyAllChildren();
		m_pSceneNode->getParentSceneNode()->removeChild(m_pSceneNode);
		m_pOwnerManager->GetOgreSceneManager()->destroySceneNode(m_pSceneNode);
		m_pSceneNode = nullptr;
	}
}
