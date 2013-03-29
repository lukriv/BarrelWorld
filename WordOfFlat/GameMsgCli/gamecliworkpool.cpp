#include "gamecliworkpool.h"
#include <wx/scopedptr.h>
#include <wx/scopeguard.h>
#include "../GameSystem/glog.h"


static void ScopeGuardCliWorkPool(GameCliClbkWorkerPool *srv)
{
	if (srv != NULL) { srv->DestroyThreads(); }
}

void GameCliClbkWorkerPool::DestroyThreads()
{
	GameErrorCode result = FWG_NO_ERROR;
	void* resultThr = 0;
	CallbackWorkerPool::iterator iter;
	//send stop request for all
	for (iter = m_clbkThreads.begin(); iter != m_clbkThreads.end(); iter++)
	{
		(**iter).StopRequest();
	}
	
	//call initiate end messages
	for (wxDword i = 0; i < m_threadsNr; i++)
	{
		if(FWG_FAILED(result = m_msgQueue.Post(NULL)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameCliClbkWorkerPool::Destroy() : Post NULL message failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
		}
	}
	
	//wait for end
	for (iter = m_clbkThreads.begin(); iter != m_clbkThreads.end(); iter++)
	{
		if(FWG_FAILED(result = (**iter).Delete(&resultThr, wxTHREAD_WAIT_BLOCK)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameCliClbkWorkerPool::Destroy() : Delete thread failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
		}
		
		if (FWG_FAILED((GameErrorCode)resultThr))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameCliClbkWorkerPool::Destroy() : Thread exit with error: 0x%08x"), m_pLogger, (GameErrorCode)resultThr, FWGLOG_ENDVAL);
		}
		
		m_threadsNr--;
	}
}

GameErrorCode GameCliClbkWorkerPool::Initialize(wxDword threadsNr, GameLogger* pLogger)
{
	//TODO: scope guard
	GameErrorCode result = FWG_NO_ERROR;
	wxScopedPtr<CallbackThread> spWorkerThr;
	CallbackThread* pWorkerThr = NULL;
	
	if(m_isInitialized) {
		return FWG_NO_ERROR;
	}
	
	m_pLogger = pLogger;
	
	wxScopeGuard guard = wxMakeGuard(ScopeGuardCliWorkPool, this);
	

	//worker threads initialization
	for (m_threadsNr = 0; m_threadsNr < threadsNr; ++m_threadsNr)
	{
		pWorkerThr = new (std::nothrow) CallbackThread(this);
		if (pWorkerThr != NULL) {
			spWorkerThr.reset(pWorkerThr);
		}
		if(FWG_FAILED(result = spWorkerThr->Initialize()))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::Initialize() : Run thread failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
			return result;	
		}
		m_clbkThreads.push_back(spWorkerThr.release());
	}
	
	guard.Dismiss();
	
	m_isInitialized = true;
	
	return FWG_NO_ERROR;
}

GameErrorCode GameCliClbkWorkerPool::MessageProcess(IGameMessage* pMsg)
{
	GameErrorCode result = FWG_NO_ERROR;
	ClbkMapType::iterator iterMap;
	
	wxCriticalSectionLocker locker(m_clbkLock);
	iterMap = m_callbackMap.find(pMsg->GetType());
	
	if (iterMap != m_callbackMap.end())
	{
		ClbkVecType::iterator iterClbk;
		for(iterClbk = iterMap->second.begin(); iterClbk != iterMap->second.end(); iterClbk++)
		{
			if(FWG_FAILED(result = (**iterClbk).OnNewMessage(*pMsg)))
			{
				FWGLOG_ERROR_FORMAT(wxT("GameCliClbkWorkerPool::MessageProcess() : Callback failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
			}
		}
	} else {
		FWGLOG_DEBUG_FORMAT(wxT("GameCliClbkWorkerPool::MessageProcess() : Callback not found for message type: %u"), m_pLogger, pMsg->GetType());
	}
	
	return result;
}

GameCliClbkWorkerPool::~GameCliClbkWorkerPool()
{
	DestroyThreads();
	m_msgQueue.Clear();
}
//--------------------------------------------------------------------
//------------ GameCliClbkWorkerPool::CallbackThread -----------------
//--------------------------------------------------------------------

void* GameCliClbkWorkerPool::CallbackThread::Entry()
{
	GameErrorCode result = FWG_NO_ERROR;
	IGameMessage *pMsg = NULL;
	
	if(!m_isInitialized) {
		return (void*) FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	while (m_isStopRequest == 0)
	{
		if (FWG_FAILED(result = m_pOwner->m_msgQueue.Receive(pMsg)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameCliClbkWorkerPool::CallbackThread::Entry() : Receive message failed: 0x%08x"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
			return (void*) result;
		}
		
		// do not process message if is NULL poiter (end message)
		if (pMsg == NULL) continue;
		
		if(FWG_FAILED(result = m_pOwner->MessageProcess(pMsg)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameCliClbkWorkerPool::CallbackThread::Entry() : Callback failed: 0x%08x"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
		}
		
		// discard message
		delete pMsg;
		pMsg = NULL;
		
	}
	
	return (void*) result;
}

GameErrorCode GameCliClbkWorkerPool::CallbackThread::Initialize()
{
	GameErrorCode result = FWG_NO_ERROR;
	if (FWG_FAILED(result = Create()))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameCliClbkWorkerPool::CallbackThread::Initialize() : Thread create failed: 0x%08x"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	if (FWG_FAILED(result = Run()))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameCliClbkWorkerPool::CallbackThread::Initialize() : Run thread failed: 0x%08x"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	m_isInitialized = true;
	
	return result;
}

GameErrorCode GameCliClbkWorkerPool::CallbackThread::StopRequest()
{
	GameErrorCode result = FWG_NO_ERROR;
	if(!m_isInitialized) {
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	wxAtomicInc(m_isStopRequest);
	
	return result;
}
