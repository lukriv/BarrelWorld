#include "gamemsgsrvimpl.h"



GameAddrType GameMsgSrv::GetCliAddress()
{
}

bool GameMsgSrv::IsConnected()
{
}

bool GameMsgSrv::IsLocal()
{
}

GameErrorCode GameMsgSrv::RegisterCallback(IGameMsgCallback* pClbk, GameMessageType msgType)
{
}

GameErrorCode GameMsgSrv::SendMsg(const IGameMessage& msg, long timeout)
{
}

GameErrorCode GameMsgSrv::UnregisterCallback(IGameMsgCallback* pClbk)
{
}
