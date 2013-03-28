#ifndef __GAME_MESSAGE_SERVER_IMPL_H__
#define __GAME_MESSAGE_SERVER_IMPL_H__


#include <wx/stream.h>
#include <wx/vector.h>
#include "../GameSystem/gdefs.h"
#include "../GameSystem/gerror.h"
#include "../GameSystem/glog.h"
#include "../GameComm/GameMessage.h"
#include "../GameMsgCli/gamemsgcli.h"
#include "../GameMsgCli/gamecliworkpool.h"
#include "gamemsgsrv.h"




class GameMsgSrv : public IGameMsgSrv, GameCliClbkWorkerPool {
protected:
    
	struct ClientInfo {
		bool m_local;
		bool m_active;
	};
	
	typedef wxVector<ClientInfo> ClientListType;
	
protected:
	wxAtomicInt m_refCount;
    GameAddrType m_addressPool;
	GameAddrType m_address;
	bool m_isConnected;
	
	ClientListType m_clientList;
	wxDword m_clientCount;
	bool m_isInitialized;
	
	wxCriticalSection m_clientLock;
	
// message server methods	
public:
	GameMsgSrv():m_refCount(1),
				m_addressPool(GAME_ADDR_SERVER+1),
				m_address(GAME_ADDR_SERVER),
				m_isConnected(false),
				m_clientCount(0),
				m_isInitialized(false){}
	virtual ~GameMsgSrv();
	
	/*!
	 * \brief Initialize server and connect the client
	 * 
	 * Initialize local server.
	 * 
	 * \retval FWG_NO_ERROR if success
	 * \retval errorcode on failture
	 */
	GameErrorCode Initialize(GameLogger *pLogger);
	

	
// message client interface	implementation
public:
	virtual bool IsLocal();
	virtual bool IsConnected();
    virtual GameErrorCode Connect();
	virtual GameErrorCode Disconnect();

	virtual GameAddrType GetCliAddress();
	virtual GameErrorCode SendMsg(IGameMessage &msg, long timeout);
	
	virtual GameErrorCode RegisterCallback(GameMessageType msgType, IGameMsgCallback *pClbk);
	virtual GameErrorCode UnregisterCallback(GameMessageType msgType, IGameMsgCallback *pClbk);

// irefobject interface implementation
public:
	virtual void addRef();
	virtual wxInt32 release();
};


#endif //__GAME_MESSAGE_SERVER_IMPL_H__