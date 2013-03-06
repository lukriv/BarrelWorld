#include "gamemsgsrvimpl.h"

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
		newVec.push_back(pClbk);
		m_callbackMap.insert(ClbkMapPairType(msgType,newVec));
	}
	
	return result;
}

GameErrorCode GameMsgSrv::SendMsg(const IGameMessage& msg, long timeout)
{
}

GameErrorCode GameMsgSrv::UnregisterCallback(IGameMsgCallback* pClbk)
{
	GameErrorCode result = FWG_NO_ERROR;

	ClbkMapType::iterator mapIter;
	ClbkVecType::iterator vecIter;

	wxCriticalSectionLocker locker(m_clbkLock);
	
	mapIter = m_callbackMap.find(msgType);
	if (mapIter == m_callbackMap.end())
	{
		return FWG_NO_ERROR;
	} else {
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

GameErrorCode GameMsgSrv::Initialize()
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
