#ifndef __GAME_MESSAGE_SERVER_H__
#define __GAME_MESSAGE_SERVER_H__



#include <wx/stream.h>
#include "../GameSystem/gdefs.h"
#include "../GameSystem/gerror.h"
#include "../GameComm/GameMessage.h"
#include "../GameMsgCli/gamemsgcli.h"
#include "gamemsgsrv.h"



class GameMsgSrv: public IGameMsgSrv {
protected:
    GameAddrType m_addressPool;
	GameAddrType m_address;
	

// message server methods	
public:
	GameMsgSrv():m_addressPool(2),m_address(1){}
	~GameMsgSrv(){}

// message client interface	implementation
public:
	virtual bool IsLocal();
	virtual bool IsConnected();
	virtual GameAddrType GetCliAddress();
	virtual GameErrorCode SendMsg(const IGameMessage& msg, long timeout);
	
	virtual GameErrorCode RegisterCallback(IGameMsgCallback *pClbk, GameMessageType msgType);
	virtual GameErrorCode UnregisterCallback(IGameMsgCallback *pClbk);

// irefobject interface implementation
public:
	virtual void addRef() = 0;
	virtual wxInt32 release() = 0;
};


#endif //__GAME_MESSAGE_SERVER_H__