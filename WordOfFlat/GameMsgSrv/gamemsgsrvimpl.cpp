#include <wx/scopedptr.h>
#include <wx/scopeguard.h>
#include "gamemsgsrvimpl.h"
#include "../GameSystem/glog.h"

#define CLIENT_LIST_SIZE 10

#define CALLBACK_THREAD_NR 2


static void ScopeGuardMsgSrv(GameMsgSrv *srv)
{
	if (srv != NULL) { srv->DestroyThreads(); }
}


GameAddrType GameMsgSrv::GetCliAddress()
{
	return m_address;
}

bool GameMsgSrv::IsConnected()
{
	return m_isConnected;
}

bool GameMsgSrv::IsLocal()
{
	return true;
}

GameErrorCode GameMsgSrv::RegisterCallback(GameMessageType msgType, IGameMsgCallback* pClbk)
{ 
	GameErrorCode result = FWG_NO_ERROR;

	ClbkMapType::iterator mapIter;
	ClbkVecType::iterator vecIter;
	
	if(!m_isInitialized) {
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}

	wxCriticalSectionLocker locker(m_clbkLock);
	
	mapIter = m_callbackMap.find(msgType);
	if (mapIter != m_callbackMap.end())
	{
		mapIter->second.insert(pClbk);
	} else {
		ClbkVecType newVec;
		newVec.insert(pClbk);
		m_callbackMap.insert(ClbkMapPairType(msgType,newVec));
	}
	
	return result;
}

GameErrorCode GameMsgSrv::SendMsg(IGameMessage &msg, long timeout)
{
	GameErrorCode result = FWG_NO_ERROR;
	msg.SetSource(m_address);
	
	if(!m_isInitialized) {
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	GameAddrType target = msg.GetTarget();
	if (GAME_ADDR_UNKNOWN == target)
	{
		return FWG_E_UNKNOWN_TARGET_ERROR;
	}
	
	if (m_address == target)
	{
		IGameMessage* copy = NULL;
		if(FWG_FAILED(result = msg.CreateCopy(copy)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::SendMsg() : Create message copy failed: 0x%08x"), m_pLogger, result);
			return result;
		}
		
		if(FWG_FAILED(result = m_msgQueue.Post(copy)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::SendMsg() : Post message to queue failed: 0x%08x"), m_pLogger, result);
			return result;
		}
		
		return result;
	} else {
		return FWG_E_NOT_IMPLEMENTED_ERROR;
	}
	
	return result;
}

GameErrorCode GameMsgSrv::UnregisterCallback(GameMessageType msgType, IGameMsgCallback* pClbk)
{
	GameErrorCode result = FWG_NO_ERROR;

	ClbkMapType::iterator mapIter;
	ClbkVecType::iterator vecIter;
	
	if(!m_isInitialized) {
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}

	wxCriticalSectionLocker locker(m_clbkLock);
	
	mapIter = m_callbackMap.find(msgType);
	if(mapIter != m_callbackMap.end())
	{
		vecIter = mapIter->second.find(pClbk);
		if (vecIter != mapIter->second.end())
		{
			mapIter->second.erase(vecIter);
			if (mapIter->second.empty())
			{
				m_callbackMap.erase(mapIter);
			}
		}
	}
	
	return result;
}

GameErrorCode GameMsgSrv::Initialize(GameLogger* pLogger)
{
	//TODO: scope guard
	GameErrorCode result = FWG_NO_ERROR;
	ClientListType::iterator iter;
	m_clientList.resize(CLIENT_LIST_SIZE);
	wxScopedPtr<CallbackThread> spWorkerThr;
	CallbackThread* pWorkerThr = NULL;
	
	if(m_isInitialized) {
		return FWG_NO_ERROR;
	}
	//initialize client list
	for (iter = m_clientList.begin(); iter != m_clientList.end(); iter++)
	{
		iter->m_active = false;
	}
	
	wxScopeGuard guard = wxMakeGuard(ScopeGuardMsgSrv, this);
	  
	//worker threads initialization
	for (wxDword i = 0; i < CALLBACK_THREAD_NR; ++i)
	{
		pWorkerThr = new (std::nothrow) CallbackThread(this);
		if (pWorkerThr != NULL) {
			spWorkerThr.reset(pWorkerThr);
		}
		if(FWG_FAILED(result = spWorkerThr->Initialize()))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::Initialize() : Run thread failed: 0x%08x"), m_pLogger, result);
			return result;	
		}
		m_clbkThreads.push_back(spWorkerThr.release());
	}
	
	guard.Dismiss();
	
	m_isInitialized = true;
	
	return FWG_NO_ERROR;
}

void GameMsgSrv::DestroyThreads()
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
	for (wxDword i = 0; i < CALLBACK_THREAD_NR; i++)
	{
		if(FWG_FAILED(result = m_msgQueue.Post(NULL)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::Destroy() : Post NULL message failed: 0x%08x"), m_pLogger, result);
		}
	}
	
	//wait for end
	for (iter = m_clbkThreads.begin(); iter != m_clbkThreads.end(); iter++)
	{
		if(FWG_FAILED(result = (**iter).Delete(&resultThr, wxTHREAD_WAIT_BLOCK)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::Destroy() : Delete thread failed: 0x%08x"), m_pLogger, result);
		}
		
		if (FWG_FAILED((GameErrorCode)resultThr))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::Destroy() : Thread exit with error: 0x%08x"), m_pLogger, (GameErrorCode)resultThr);
		}
	}
	
	
}

GameErrorCode GameMsgSrv::Connect()
{
	GameErrorCode result = FWG_NO_ERROR;
	ClientInfo* pCliInfo = NULL;
	
	if(!m_isInitialized) {
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	//activate server-client in client list
	pCliInfo = &m_clientList[m_address-1];
	
	pCliInfo->m_local = true;
	pCliInfo->m_active = true;
	
	//set is connected
	m_isConnected = true;
	
	return result;
}

GameErrorCode GameMsgSrv::Disconnect()
{
	GameErrorCode result = FWG_NO_ERROR;
	ClientInfo* pCliInfo = NULL;
	
	if(!m_isInitialized) {
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	pCliInfo = &m_clientList[m_address-1];
	
	pCliInfo->m_active = false;
	
	m_isConnected = false;
	
	return result;
}

void GameMsgSrv::addRef()
{
	wxAtomicInc(m_refCount);
}

wxInt32 GameMsgSrv::release()
{
	wxInt32 refCount = wxAtomicDec(m_refCount);
	if (refCount == 0)
	{
		delete this;
	}
	
	return refCount;
}


GameMsgSrv::~GameMsgSrv()
{
	DestroyThreads();
}


GameErrorCode GameMsgSrv::MessageProcess(IGameMessage* pMsg)
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
				FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::MessageProcess() : Callback failed: 0x%08x"), m_pLogger, result);
			}
		}
	} else {
		FWGLOG_DEBUG_FORMAT(wxT("GameMsgSrv::MessageProcess() : Callback not found for message type: %u"), m_pLogger, pMsg->GetType());
	}
	
	return result;
}



//--------------------------------------------------------------------
//--------------- GameMsgSrv::CallbackThread -------------------------
//--------------------------------------------------------------------

void* GameMsgSrv::CallbackThread::Entry()
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
			FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::CallbackThread::Entry() : Receive message failed: 0x%08x"), m_pOwner->GetLogger(), result);
			return (void*) result;
		}
		
		// do not process message if is NULL poiter (end message)
		if (pMsg == NULL) continue;
		
		if(FWG_FAILED(result = m_pOwner->MessageProcess(pMsg)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::CallbackThread::Entry() : Callback failed: 0x%08x"), m_pOwner->GetLogger(), result);
		}
		
		// discard message
		delete pMsg;
		pMsg = NULL;
		
	}
	
	return (void*) result;
}

GameErrorCode GameMsgSrv::CallbackThread::Initialize()
{
	GameErrorCode result = FWG_NO_ERROR;
	if (FWG_FAILED(result = Create()))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::CallbackThread::Initialize() : Thread create failed: 0x%08x"), m_pOwner->GetLogger(), result);
		return result;
	}
	
	if (FWG_FAILED(result = Run()))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::CallbackThread::Initialize() : Run thread failed: 0x%08x"), m_pOwner->GetLogger(), result);
		return result;
	}
	
	m_isInitialized = true;
	
	return result;
}

GameErrorCode GameMsgSrv::CallbackThread::StopRequest()
{
	GameErrorCode result = FWG_NO_ERROR;
	if(!m_isInitialized) {
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	wxAtomicInc(m_isStopRequest);
	
	return result;
}


