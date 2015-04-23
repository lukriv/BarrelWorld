#include "grendercomp.h"

#include <OGRE/OgreNode.h>
#include "grendercmgr.h"
#include "../gentity.h"
#include <GameComp/TransformComp/gtranscomp.h>

RenderComponent::~RenderComponent()
{
	Clear();
	if((m_pSceneNode)&&(m_pOwnerManager)) {
		m_pOwnerManager->GetOgreSceneManager()->destroySceneNode(m_pSceneNode);
		m_pSceneNode = nullptr;
	}
}

void RenderComponent::Clear()
{
	if(m_pSceneNode)
	{
		m_pSceneNode->removeAndDestroyAllChildren();
		m_pSceneNode->getParentSceneNode()->removeChild(m_pSceneNode);
		m_pOwnerManager->GetOgreSceneManager()->destroySceneNode(m_pSceneNode);
		m_pSceneNode = nullptr;
	}
}

GameErrorCode RenderComponent::Initialize(const Ogre::Vector3& translate, const Ogre::Quaternion& rotation)
{
	if(m_pSceneNode != nullptr)
	{
		// component was already initialized
		return FWG_NO_ERROR;
	}
	
	m_pSceneNode = m_pOwnerManager->GetOgreSceneManager()->getRootSceneNode()->createChildSceneNode( translate, rotation);
	
	if(m_pSceneNode == nullptr) {
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}

	return FWG_NO_ERROR;
}

GameErrorCode RenderComponent::ReceiveMessage(TaskMessage& msg)
{
	wxCriticalSectionLocker lock(m_renderLock);
	
	m_receivedMessages.push_back(msg);
	
	return FWG_NO_ERROR;
}

