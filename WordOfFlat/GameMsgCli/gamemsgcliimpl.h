#ifndef __GAME_MESSAGE_CLIENT_IMPL_H__
#define __GAME_MESSAGE_CLIENT_IMPL_H__

#include "../GameSystem/gerror.h"
#include "../GameComm/GameMessage.h"
#include "gamecliworkpool.h"


class GameMsgCli : public IGameMsgCli, GameCliClbkWorkerPool {
	bool m_isInitialized;
protected:
	wxAtomicInt32 m_refCount;
	GameAddrType m_cliAddr;
	//wxSocketServer *m_pServer;
public:
	GameMsgCli() : GameCliClbkWorkerPool(),
			m_isInitialized(false),
			m_refCount(1),
			m_cliAddr(GAME_ADDR_UNKNOWN) {}
	virtual ~GameMsgCli();
	
	GameErrorCode Initialize(GameLogger* pLogger = NULL);
	
	void Destroy();
	
// message client methods
public:
	virtual bool IsLocal();
	virtual bool IsConnected();
	virtual GameErrorCode Connect();
	virtual GameErrorCode Disconnect();
	
	virtual GameAddrType GetCliAddress();
	virtual GameErrorCode SendMsg(IGameMessage &msg, long timeout);
	
	virtual GameErrorCode RegisterCallback(GameMessageType msgType, IGameMsgCallback *pClbk);
	virtual GameErrorCode UnregisterCallback(GameMessageType msgType, IGameMsgCallback *pClbk);

// IRefObject methods	
public:
	virtual void addRef();
	virtual wxInt32 release();
};

#endif //__GAME_MESSAGE_CLIENT_IMPL_H__