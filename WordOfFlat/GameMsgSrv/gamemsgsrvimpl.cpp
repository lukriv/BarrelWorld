#include "gamemsgsrvimpl.h"
#include "../GameSystem/glog.h"

#define CLIENT_LIST_SIZE 10

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

GameErrorCode GameMsgSrv::RegisterCallback(IGameMsgCallback* pClbk, GameMessageType msgType)
{ 
	GameErrorCode result = FWG_NO_ERROR;

	ClbkMapType::iterator mapIter;
	ClbkVecType::iterator vecIter;

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

GameErrorCode GameMsgSrv::SendMsg(IGameMessage* msg, long timeout)
{
	GameErrorCode result = FWG_NO_ERROR;
	msg->SetSource(m_address);
	GameAddrType target = msg->GetTarget();
	if (GAME_ADDR_UNKNOWN == target)
	{
		return FWG_E_UNKNOWN_TARGET_ERROR;
	}
	
	if (m_address == target)
	{
		IGameMessage* copy = NULL;
		if(FWG_FAILED(result = msg->CreateCopy(copy)))
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

GameErrorCode GameMsgSrv::UnregisterCallback(IGameMsgCallback* pClbk)
{
	GameErrorCode result = FWG_NO_ERROR;

	ClbkMapType::iterator mapIter;
	ClbkVecType::iterator vecIter;

	wxCriticalSectionLocker locker(m_clbkLock);
	
	for (mapIter = m_callbackMap.begin(); mapIter != m_callbackMap.end(); mapIter++)
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
	ClientListType::iterator iter;
	m_clientList.resize(CLIENT_LIST_SIZE);
	
	//initialize client list
	for (iter = m_clientList.begin(); iter != m_clientList.end(); iter++)
	{
		iter->m_active = false;
	}
	return FWG_NO_ERROR;
}

GameErrorCode GameMsgSrv::Connect()
{
	GameErrorCode result = FWG_NO_ERROR;
	ClientInfo* pCliInfo = NULL;
	//activate server-client in client list
	pCliInfo = &m_clientList[m_address-1];
	
	pCliInfo->m_local = true;
	pCliInfo->m_active = true;
	
	//set is connected
	m_isConnected = true;
	
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

//--------------------------------------------------------------------
//--------------- GameMsgSrv::CallbackThread -------------------------
//--------------------------------------------------------------------

void* GameMsgSrv::CallbackThread::Entry()
{
	GameErrorCode result = FWG_NO_ERROR;
	IGameMessage *pMsg = NULL;
	while (m_isStopRequest == 0)
	{
		if (FWG_FAILED(result = m_pOwner->m_msgQueue.Receive(pMsg)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::CallbackThread::Entry() : Receive message failed: 0x%08x"), m_pOwner->GetLogger(), result);
			return (void*) result;
		}
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
	
	return result;
}

GameErrorCode GameMsgSrv::CallbackThread::StopRequest()
{
	GameErrorCode result = FWG_NO_ERROR;
	
	wxAtomicInc(m_isStopRequest);
	if (FWG_FAILED(result = m_pOwner->m_msgQueue.Post(NULL)))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::CallbackThread::StopRequest() : Post message failed: 0x%08x"), m_pOwner->GetLogger(), result);
		return result;
	}
	
	return result;
}
