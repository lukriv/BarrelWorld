#include <wx/scopedptr.h>
#include <wx/scopeguard.h>
#include "gamemsgsrvimpl.h"
#include "../GameSystem/glog.h"

#define CLIENT_LIST_SIZE 10

static const wxDword CALLBACK_THREAD_NR = 2;


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
			FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::SendMsg() : Create message copy failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
		if(FWG_FAILED(result = m_msgQueue.Post(copy)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::SendMsg() : Post message to queue failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
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

	
	if(m_isInitialized) {
		return FWG_NO_ERROR;
	}
	//initialize client list
	for (iter = m_clientList.begin(); iter != m_clientList.end(); iter++)
	{
		iter->m_active = false;
	}
	
	if(FWG_FAILED(result = GameCliClbkWorkerPool::Initialize(CALLBACK_THREAD_NR, pLogger)))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::Initialize() : Initialization GameCliClbkWorkerPool failed: 0x%08x"),
							pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	m_isInitialized = true;
	
	return FWG_NO_ERROR;
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

}


