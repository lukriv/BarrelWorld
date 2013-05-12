#ifndef __GAME_MESSAGE_SERVER_IMPL_H__
#define __GAME_MESSAGE_SERVER_IMPL_H__


#include <wx/stream.h>
#include <wx/vector.h>
#include <SFML/Network.hpp>
#include "../GameSystem/gdefs.h"
#include "../GameSystem/gerror.h"
#include "../GameSystem/glog.h"
#include "../GameSystem/gthread.h"
#include "../GameComm/GameMessage.h"
#include "../GameMsgCli/gamemsgcli.h"
#include "../GameMsgCli/gamecliworkpool.h"
#include "gamemsgsrv.h"




class GameMsgSrv : public IGameMsgSrv, public GameCliClbkWorkerPool, protected GameThread {
protected:
    
	class ClientInfo {
	private:
		GameMsgSrv *m_pOwner;
		bool m_local;
		bool m_active;
		GameAddrType m_reservedAddr;
		sf::TcpSocket *m_pSocket;
	public:
		ClientInfo(GameMsgSrv *pOwner) : m_pOwner(pOwner),
									m_local(false),
									m_active(false),
									m_reservedAddr(GAME_ADDR_UNKNOWN),
									m_pSocket(NULL) {}
		~ClientInfo() 
		{
			if(m_pSocket != NULL) {
				m_pSocket->disconnect();
				delete m_pSocket;
				m_pSocket = NULL;
			}
		}
		
		void SetAddress(GameAddrType gameAddr) {m_reservedAddr = gameAddr;}
		GameAddrType GetAddress() {return m_reservedAddr;}
		
		GameErrorCode SendMsg(IGameMessage& msg, long timeout);
		
		GameErrorCode ClientConnect(sf::Socket *pSocket);
		GameErrorCode ClientDisconnect();
		
		inline bool IsActive() {return m_active;}
		inline bool IsLocal() {return m_local;}
		
		inline sf::Socket& GetSocket() {return *m_pSocket;}
	};
	
	typedef wxVector<ClientInfo*> ClientListType;
	
	
protected:
	wxAtomicInt m_refCount;
    GameAddrType m_addressPool;
	GameAddrType m_address;
	bool m_isConnected;
	
	
	
	
	wxDword m_clientCount;
	bool m_isInitialized;
	bool m_stopRequest;
	
	
	ClientListType m_clientList;
	sf::SocketSelector m_socketSelector;
	sf::TcpListener m_socketServer;
	wxCriticalSection m_clientLock;
	
	

// message server protected methods
protected:
	
	virtual void *Entry();
	
	GameErrorCode ConnectRemoteClient(ClientInfo &client, sf::Socket *pSocket);
	ClientInfo* FindNonActiveClient();
	GameErrorCode StopRequest();
	
// message server methods	
public:
	GameMsgSrv():GameThread(wxTHREAD_JOINABLE),
				m_refCount(1),
				m_addressPool(GAME_ADDR_SERVER+1),
				m_address(GAME_ADDR_SERVER),
				m_isConnected(false),
				m_clientCount(0),
				m_isInitialized(false),
				m_stopRequest(false){}
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