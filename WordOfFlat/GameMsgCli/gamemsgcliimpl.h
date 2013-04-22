#ifndef __GAME_MESSAGE_CLIENT_IMPL_H__
#define __GAME_MESSAGE_CLIENT_IMPL_H__

#include "../GameSystem/gerror.h"
#include "../GameComm/GameMessage.h"
#include "gamecliworkpool.h"
#include <wx/event.h>
#include <wx/socket.h>


class GameMsgCli : public IGameMsgCli, public GameCliClbkWorkerPool, public wxEvtHandler {
	bool m_isInitialized;
protected:
	wxAtomicInt32 m_refCount;
	GameAddrType m_cliAddr;
	bool m_connected;
	bool m_isConnecting;
	wxSocketClient* m_socketClient;
	wxString m_hostName;
	wxCriticalSection m_clientLock;
public:
	GameMsgCli() : GameCliClbkWorkerPool(),
			m_isInitialized(false),
			m_refCount(1),
			m_cliAddr(GAME_ADDR_UNKNOWN),
			m_connected(false),
			m_isConnecting(false),
			m_socketClient(NULL),
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
	
	
	/*!
	 * \brief On new socket event handler
	 * \param event Socket event
	 */
	void OnSocketEvent(wxSocketEvent &event);
	
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