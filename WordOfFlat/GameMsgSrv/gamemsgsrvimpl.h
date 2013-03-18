#ifndef __GAME_MESSAGE_SERVER_IMPL_H__
#define __GAME_MESSAGE_SERVER_IMPL_H__


#include <map>
#include <wx/stream.h>
#include <wx/vector.h>
#include "../GameSystem/gdefs.h"
#include "../GameSystem/gerror.h"
#include "../GameSystem/gthread.h"
#include "../GameSystem/gmsgqueue.h"
#include "../GameSystem/glog.h"
#include "../GameComm/GameMessage.h"
#include "../GameMsgCli/gamemsgcli.h"
#include "gamemsgsrv.h"


class GameMsgSrv : public IGameMsgSrv {
protected:
    
	struct ClientInfo {
		bool m_local;
		bool m_active;
	};
	
	typedef wxVector<ClientInfo> ClientListType;
	typedef std::set<IGameMsgCallback*> ClbkVecType;
	typedef std::pair<GameMessageType,ClbkVecType> ClbkMapPairType;
	typedef std::map<GameMessageType,ClbkVecType> ClbkMapType;
	typedef GameMessageQueue<IGameMessage*> MsgQueueType;
	
	class CallbackThread : public GameThread {
		wxAtomicInt m_isStopRequest;
		GameMsgSrv* m_pOwner;
		bool m_isInitialized;
		
	protected:
		virtual void *Entry();
	public:
		CallbackThread(GameMsgSrv* pOwner): m_isStopRequest(0),
										m_pOwner(pOwner),
										m_isInitialized(false) {}
		GameErrorCode Initialize();
		GameErrorCode StopRequest();

	};
	
	
	
protected:
	wxAtomicInt m_refCount;
	GameLoggerPtr m_pLogger;
    GameAddrType m_addressPool;
	GameAddrType m_address;
	bool m_isConnected;
	
	ClientListType m_clientList;
	wxDword m_clientCount;
	
	ClbkMapType m_callbackMap;
	MsgQueueType m_msgQueue;
	wxCriticalSection m_clientLock;
	wxCriticalSection m_clbkLock;

		
// message server methods	
public:
	GameMsgSrv():m_refCount(1),
				m_pLogger(NULL),
				m_addressPool(GAME_ADDR_SERVER+1),
				m_address(GAME_ADDR_SERVER),
				m_isConnected(false),
				m_clientCount(0){}
	~GameMsgSrv(){}
	
	/*!
	 * \brief Initialize server and connect the client
	 * 
	 * Initialize local server.
	 * 
	 * \retval FWG_NO_ERROR if success
	 * \retval errorcode on failture
	 */
	GameErrorCode Initialize(GameLogger *pLogger);
	
	inline GameLoggerPtr GetLogger() { return m_pLogger; }
	

// message client interface	implementation
public:
	virtual bool IsLocal();
	virtual bool IsConnected();
    virtual GameErrorCode Connect();	

	virtual GameAddrType GetCliAddress();
	virtual GameErrorCode SendMsg(IGameMessage* msg, long timeout);
	
	virtual GameErrorCode RegisterCallback(IGameMsgCallback *pClbk, GameMessageType msgType);
	virtual GameErrorCode UnregisterCallback(IGameMsgCallback *pClbk);

// irefobject interface implementation
public:
	virtual void addRef();
	virtual wxInt32 release();
};


#endif //__GAME_MESSAGE_SERVER_IMPL_H__