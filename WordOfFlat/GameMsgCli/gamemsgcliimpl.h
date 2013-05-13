#ifndef __GAME_MESSAGE_CLIENT_IMPL_H__
#define __GAME_MESSAGE_CLIENT_IMPL_H__

#include "../GameSystem/gerror.h"
#include "../GameComm/GameMessage.h"
#include "gamecliworkpool.h"
#include <SFML/Network.hpp>


class GameMsgCli : public IGameMsgCli, public GameCliClbkWorkerPool, protected GameThread {
	bool m_isInitialized;
protected:
	wxAtomicInt32 m_refCount;
	GameAddrType m_cliAddr;
	bool m_connected;
	bool m_stopRequrest;
	sf::TcpSocket m_socketClient;
	sf::SocketSelector m_socketSelector;
	wxString m_hostName;
	wxCriticalSection m_clientLock;
	
protected:
	virtual void *Entry();
	void StopRequest();
	
public:
	GameMsgCli() : GameCliClbkWorkerPool(),
			GameThread(wxTHREAD_JOINABLE),
			m_isInitialized(false),
			m_refCount(1),
			m_cliAddr(GAME_ADDR_UNKNOWN),
			m_connected(false),
			m_stopRequrest(false),
			m_clientLock(wxCRITSEC_DEFAULT) {}
	virtual ~GameMsgCli();
	
	GameErrorCode Initialize(GameLogger* pLogger = NULL);
	
	/*!
	 * \brief Sets host name
	 * 
	 * \param hostName Hostname in IP4 format
	 * \retval FWG_NO_ERROR on success.
	 * \retval FWG_E_INVALID_ADDRESS_ERROR if wrong address format is putted in.
	 */
	GameErrorCode SetServerAddress(const wxString & hostName);
	
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