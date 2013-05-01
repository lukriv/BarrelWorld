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
	virtual GameErrorCode Connect() = 0;
	virtual GameErrorCode Disconnect() = 0;
	
	virtual GameAddrType GetCliAddress() = 0;
	virtual GameErrorCode SendMsg(IGameMessage &msg, long timeout) = 0;
	
	virtual GameErrorCode RegisterCallback(GameMessageType msgType, IGameMsgCallback *pClbk) = 0;
	virtual GameErrorCode UnregisterCallback(GameMessageType msgType, IGameMsgCallback *pClbk) = 0;
	
};


//static const unsigned short int GAME_SERVICE_PORT = 9567;
static const unsigned short int GAME_SERVICE_PORT = 3000;

#endif //__GAME_MESSAGE_CLIENT_H__