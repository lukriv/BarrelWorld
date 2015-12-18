#include "grendercompbase.h"

#include <OGRE/OgreNode.h>
#include "grendersystem.h"
#include "../gentity.h"
#include <GameComp/TransformComp/gtranscomp.h>

RenderComponentBase::~RenderComponentBase()
{}

GameErrorCode RenderComponentBase::ReceiveMessage(TaskMessage& msg)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxCriticalSectionLocker lock(m_renderLock);
	
	m_receivedMessages.push_back(msg);
	
	if(!m_alreadyInUpdateQueue)
	{
		if(FWG_FAILED(result = m_pOwnerManager->AddToUpdateQueue(this)))
		{
			FWGLOG_ERROR_FORMAT(wxT("Add to update queue failed: 0x%08x"), m_pOwnerManager->GetLogger()
														, result, FWGLOG_ENDVAL);
			return result;
		}
		
		m_alreadyInUpdateQueue = true;
	}
	
	return result;
}

