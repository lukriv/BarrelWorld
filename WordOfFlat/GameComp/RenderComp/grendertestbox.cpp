#include "grendertestbox.h"

RenderTestBox::RenderTestBox()
{
}

RenderTestBox::~RenderTestBox()
{
}


GameErrorCode RenderTestBox::Initialize(TransformComponent* pTransform)
{
	m_spTransform = pTransform;
	if(m_spTransform.IsEmpty())
	{
		// transform component cannot be null - add transform component to entity at first
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	TransformData* transData = m_spTransform->GetData();

	if(FWG_FAILED(RenderComponent::Initialize( Ogre::Vector3(transData->m_translate.getX(), transData->m_translate.getY(), transData->m_translate.getZ())
		, Ogre::Quaternion(transData->m_rotation.getW(), transData->m_rotation.getX(), transData->m_rotation.getY(), transData->m_rotation.getZ()));
	


	return FWG_NO_ERROR;
}





void RenderComponent::ProcessUpdate()
{
	wxCriticalSectionLocker lock(m_renderLock);
	
	TMessageList::iterator iter;
	for(iter = m_receivedMessages.begin(); iter != m_receivedMessages.end(); ++iter)
	{
		switch(iter->GetTaskType())
		{
		case GAME_TASK_TRANSFORM_UPDATE:
			if(!m_spTransform.IsEmpty())
			{
				//todo: upgrade updating scene node transform
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
