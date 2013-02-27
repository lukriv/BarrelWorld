#ifndef __GAME_MESSAGE_SERVER_IMPL_H__
#define __GAME_MESSAGE_SERVER_IMPL_H__


#include <map>
#include <wx/stream.h>
#include <wx/thread.h>
#include <wx/vector.h>
#include "../GameSystem/gdefs.h"
#include "../GameSystem/gerror.h"
#include "../GameSystem/circularQueue.h"
#include "../GameComm/GameMessage.h"
#include "../GameMsgCli/gamemsgcli.h"
#include "gamemsgsrv.h"


class GameMsgSrv : public IGameMsgSrv {
protected:
  
	struct ClientInfo {
		bool m_local;
	};
	
	typedef std::set<IGameMsgCallback*> ClbkVecType;
	typedef std::pair<GameMessageType,ClbkVecType> ClbkMapPairType;
	typedef std::map<GameAddrType,ClientInfo> ClientMapType;
	typedef std::map<GameMessageType,ClbkVecType> ClbkMapType;
	typedef CircularQueue<IGameMessage*> MsgQueueType;
	
protected:
    GameAddrType m_addressPool;
	GameAddrType m_address;
	bool m_isConnected;
	ClientMapType m_clientMap;
	ClbkMapType m_callbackMap;
	MsgQueueType m_msgQueue;
	wxCriticalSection m_clientLock;
	wxCriticalSection m_clbkLock;
	
		
// message server methods	
public:
	GameMsgSrv():m_addressPool(GAME_ADDR_SERVER+1),
				m_address(GAME_ADDR_SERVER),
				m_isConnected(false){}
	~GameMsgSrv(){}
	
	/*!
	 * \brief Initialize server and connect the client
	 * 
	 * Initialize local server.
	 * 
	 * \retval FWG_NO_ERROR if success
	 * \retval errorcode on failture
	 */
	GameErrorCode Initialize();
	

// message client interface	implementation
public:
	virtual bool IsLocal();
	virtual bool IsConnected();
    virtual GameErrorCode Connect();	

	virtual GameAddrType GetCliAddress();
	virtual GameErrorCode SendMsg(const IGameMessage& msg, long timeout);
	
	virtual GameErrorCode RegisterCallback(IGameMsgCallback *pClbk, GameMessageType msgType);
	virtual GameErrorCode UnregisterCallback(IGameMsgCallback *pClbk);

// irefobject interface implementation
public:
	virtual void addRef();
	virtual wxInt32 release();
};


#endif //__GAME_MESSAGE_SERVER_IMPL_H__