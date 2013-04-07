#ifndef __GAME_MESSAGE_SERVER_IMPL_H__
#define __GAME_MESSAGE_SERVER_IMPL_H__


#include <wx/stream.h>
#include <wx/vector.h>
#include <wx/socket.h>
#include "../GameSystem/gdefs.h"
#include "../GameSystem/gerror.h"
#include "../GameSystem/glog.h"
#include "../GameSystem/gthread.h"
#include "../GameComm/GameMessage.h"
#include "../GameMsgCli/gamemsgcli.h"
#include "../GameMsgCli/gamecliworkpool.h"
#include "gamemsgsrv.h"




class GameMsgSrv : public IGameMsgSrv, public GameCliClbkWorkerPool, public wxEvtHandler {
protected:
    
	struct ClientInfo : public wxEvtHandler {
		bool m_local;
		bool m_active;
		wxObjectDataPtr<wxSocketBase> m_spSocket;
		
		ClientInfo() : m_local(false), m_active(false) {}
		
		void SocketEvent(wxSocketEvent &event);
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
	wxSocketServer *m_pSocketServer;

// message server protected methods
protected:
	GameErrorCode ConnectRemoteClient(ClientInfo &client, wxSocketBase *pSocket);
	
// message server methods	
public:
	GameMsgSrv():m_refCount(1),
				m_addressPool(GAME_ADDR_SERVER+1),
				m_address(GAME_ADDR_SERVER),
				m_isConnected(false),
				m_clientCount(0),
				m_isInitialized(false),
				m_pSocketServer(NULL){}
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
	
	/*!
	 * \brief Destroy the object without memory leaks.
	 * 
	 * It is called from destructor and it uses also scope guard in initialize function.
	 */
	void Destroy();
	
	void SocketReceiver(wxSocketEvent &event);
	
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