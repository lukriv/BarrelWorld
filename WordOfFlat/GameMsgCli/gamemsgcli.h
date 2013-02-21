#ifndef __GAME_MESSAGE_CLIENT_H__
#define __GAME_MESSAGE_CLIENT_H__


#include "../GameComm/GameMessage.h"

#include "../GameSystem/refobject.h"
#include "../GameSystem/gerror.h"

class IGameMsgCallback {
public:
	virtual GameErrorCode OnNewMessage(const IGameMessage& msg) = 0;
};

class IGameMsgCli : public IRefObject {
public:
	virtual bool IsLocal() = 0;
	virtual bool IsConnected() = 0;
	virtual GameAddrType GetCliAddress() = 0;
	virtual GameErrorCode SendMsg(const IGameMessage& msg, long timeout) = 0;
	
	virtual GameErrorCode RegisterCallback(IGameMsgCallback *pClbk, GameMessageType msgType) = 0;
	virtual GameErrorCode UnregisterCallback(IGameMsgCallback *pClbk) = 0;
	
};


#endif //__GAME_MESSAGE_CLIENT_H__