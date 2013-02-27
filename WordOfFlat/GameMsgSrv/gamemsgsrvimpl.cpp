#include "gamemsgsrvimpl.h"



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
}

GameErrorCode GameMsgSrv::Initialize()
{
	return FWG_NO_ERROR;
}

GameErrorCode GameMsgSrv::Connect()
{
	
}
