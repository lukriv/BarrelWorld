#include "gamemsgcliimpl.h"
#include <wx/sckaddr.h>
#include <wx/sckstrm.h>
#include <wx/scopedptr.h>
#include "../GameSystem/gsocket.h"
#include "../GameComm/GameMessageImpl.h"
#include "../GameComm/gamemsgdata.h"

static const long CLIENT_CONNECTION_TIMEOUT = 30;

static void GameClientScopeGuard(GameMsgCli *pMsgCli) 
{
	if (pMsgCli){
		pMsgCli->Destroy();
	}
}

GameErrorCode GameMsgCli::Initialize(GameLogger* pLogger)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxIPV4address ipAddr;
	
	wxScopeGuard guard = wxMakeGuard(GameClientScopeGuard, this);
	
	m_socketClient = new (std::nothrow) wxSocketClient(wxSOCKET_BLOCK|wxSOCKET_WAITALL);
	if(m_socketClient == NULL) {
		result = FWG_E_MEMORY_ALLOCATION_ERROR;
		FWGLOG_ERROR_FORMAT(wxT("GameMsgCli::Initialize() : Create client socket failed: 0x%08x"),
							m_pLogger, result, FWGLOG_ENDVAL);
	}
	
	m_socketClient->SetTimeout(CLIENT_CONNECTION_TIMEOUT);
	m_socketClient->SetEventHandler(*this);
	// set hostname
	ipAddr.LocalHost();
	m_hostName = ipAddr.Hostname();
	
	guard.Dismiss();
	
	m_isInitialized = true;	
	return result;
}


GameErrorCode GameMsgCli::Connect()
{
	GameErrorCode result = FWG_NO_ERROR;
	wxCriticalSectionLocker lock(m_clientLock);
	
	if(!m_isInitialized)
	{
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	if (m_isConnecting || m_connected)
	{
		return FWG_NO_ERROR;
	}
	
	wxIPV4address ipAddr;
    ipAddr.Hostname(wxT("127.0.0.1"));
    ipAddr.Service(GAME_SERVICE_PORT);

	Bind(wxEVT_SOCKET, &GameMsgCli::OnSocketEvent, this, wxID_ANY);
	
	
    m_socketClient->SetNotify(wxSOCKET_CONNECTION_FLAG|wxSOCKET_LOST_FLAG|wxSOCKET_OUTPUT_FLAG|wxSOCKET_INPUT_FLAG);
    m_socketClient->Notify(true);
	
     //wxLogMessage(wxT("EventWorker: Connecting....."));
    m_socketClient->Connect(ipAddr,true);
	m_socketClient->WaitOnConnect(CLIENT_CONNECTION_TIMEOUT);
	if(m_socketClient->Error())
	{
		if(FWG_FAILED(result = GameConvertWxSocketErr2GameErr(m_socketClient->LastError())))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameMsgCli::Connect() : Connection failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
	}
	
	
	m_isConnecting = true;	
	
	if(m_cliAddr == GAME_ADDR_UNKNOWN)
	{
		GameMessage msg;
		ClientIdRequestData data;
		wxSocketOutputStream socketOutStream(*m_socketClient);
		msg.SetTarget(GAME_ADDR_SERVER);
		msg.SetMessage(data, GAME_MSG_TYPE_CLIENT_ID_REQUEST);
		if(FWG_FAILED(result = msg.Store(socketOutStream))) {
			FWGLOG_ERROR_FORMAT(wxT("GameMsgCli::OnSocketEvent() : Message sending failed: 0x%08x"),
						m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
	}
	
	m_connected = true;
	return FWG_NO_ERROR;
}

GameErrorCode GameMsgCli::Disconnect()
{
	wxCriticalSectionLocker lock(m_clientLock);
	
	if((!m_connected)&&(!m_isConnecting))
	{
		return FWG_NO_ERROR;
	}

	m_isConnecting = false;
	m_connected = false;
	
	Unbind(wxEVT_SOCKET, &GameMsgCli::OnSocketEvent, this, wxID_ANY);
	
	m_socketClient->Notify(false);
	
	if(!m_socketClient->Close())
	{
		return GameConvertWxSocketErr2GameErr(m_socketClient->LastError());
	}
	
	return FWG_NO_ERROR;
}

GameAddrType GameMsgCli::GetCliAddress()
{
	return m_cliAddr;
}

bool GameMsgCli::IsConnected()
{
	return m_connected;
}

bool GameMsgCli::IsLocal()
{
	return false;
}

GameErrorCode GameMsgCli::RegisterCallback(GameMessageType msgType, IGameMsgCallback* pClbk)
{
	return RegisterCallbackInternal(msgType, pClbk);
}

GameErrorCode GameMsgCli::SendMsg(IGameMessage& msg, long timeout)
{
	if(!m_isInitialized)
	{
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	if(!m_connected)
	{
		return FWG_E_NOT_CONNECTED_ERROR;
	}
	
	GameErrorCode result = FWG_NO_ERROR;
	wxCriticalSectionLocker lock(m_clientLock);
	wxSocketOutputStream socketOutStream(*m_socketClient);
	
	if(FWG_FAILED(result = msg.Store(socketOutStream)))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameMsgCli::SendMsg() : Send message failed: 0x%08x"), m_pLogger,
							result, FWGLOG_ENDVAL);
		return result;
	}
	
	return result;
}

GameErrorCode GameMsgCli::UnregisterCallback(GameMessageType msgType, IGameMsgCallback* pClbk)
{
	return UnregisterCallbackInternal(msgType, pClbk);
}

void GameMsgCli::addRef()
{
	wxAtomicInc(m_refCount);
}

wxInt32 GameMsgCli::release()
{
	wxInt32 refCount = wxAtomicDec(m_refCount);
	if(refCount == 0)
	{
		delete this;
	}
	return refCount;
}


GameMsgCli::~GameMsgCli()
{
	Destroy();
}

void GameMsgCli::Destroy()
{
	if(m_socketClient != NULL)
	{
		m_socketClient->Destroy();
		m_socketClient = NULL;
	}
	
	m_isInitialized = false;
}

GameErrorCode GameMsgCli::SetServerAddress(const wxString& hostName)
{
	wxIPV4address ipAddr;
	if (!ipAddr.Hostname(hostName))
	{
		return FWG_E_INVALID_ADDRESS_ERROR;
	}
	
	m_hostName = hostName;
	return FWG_NO_ERROR;
	
}

void GameMsgCli::OnSocketEvent(wxSocketEvent& event)
{
	GameErrorCode result = FWG_NO_ERROR;
	// lock
	wxCriticalSectionLocker lock(m_clientLock);
	switch(event.GetSocketEvent()) 
	{
    case wxSOCKET_INPUT:
	{
		FWGLOG_TRACE(wxT("GameMsgCli::OnSocketEvent() : Game client input event"), m_pLogger);
		wxSocketInputStream socketStream(*m_socketClient);
		
		GameMessage* pMessage = NULL;
		pMessage = new (std::nothrow) GameMessage;
		if (pMessage == NULL) {
			FWGLOG_ERROR(wxT("GameMsgCli::OnSocketEvent() : Memory allocation error"), m_pLogger);
			return;
		}
		
		wxScopedPtr<GameMessage> apMessage(pMessage);
		
		if (FWG_FAILED(result = pMessage->Load(socketStream)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameMsgCli::OnSocketEvent() : Message loading failed: 0x%08x"),
							m_pLogger, result, FWGLOG_ENDVAL);
			return;
		}
			
		if (FWG_FAILED(result = m_msgQueue.Post(apMessage.release())))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameMsgCli::OnSocketEvent() : Message post failed: 0x%08x"), 
							m_pLogger, result, FWGLOG_ENDVAL);
			return;
		}
		
		break;
	}
    case wxSOCKET_OUTPUT:
	{
		FWGLOG_INFO(wxT("GameMsgCli::OnSocketEvent() : Game client output event"), m_pLogger);

		if(m_socketClient->Error())
		{
			result = GameConvertWxSocketErr2GameErr(m_socketClient->LastError());
			FWGLOG_ERROR_FORMAT(wxT("GameMsgCli::OnSocketEvent() : Socket client failed: 0x%08x"), 
					m_pLogger, result, FWGLOG_ENDVAL);
		}
		
		if(m_cliAddr == GAME_ADDR_UNKNOWN)
		{
			GameMessage msg;
			ClientIdRequestData data;
			wxSocketOutputStream socketOutStream(*m_socketClient);
			msg.SetTarget(GAME_ADDR_SERVER);
			msg.SetMessage(data, GAME_MSG_TYPE_CLIENT_ID_REQUEST);
			if(FWG_FAILED(result = msg.Store(socketOutStream))) {
				FWGLOG_ERROR_FORMAT(wxT("GameMsgCli::OnSocketEvent() : Message sending failed: 0x%08x"),
							m_pLogger, result, FWGLOG_ENDVAL);
			}
		}
		
		break;
	}
    case wxSOCKET_CONNECTION:
        FWGLOG_INFO(wxT("GameMsgCli::OnSocketEvent() : Game client connected"), m_pLogger);
		if (m_socketClient->LastError() == wxSOCKET_NOERROR) {
			m_isConnecting = false;
			m_connected = true;
		}
		break;

    case wxSOCKET_LOST:
		FWGLOG_WARNING(wxT("GameMsgCli::OnSocketEvent() : Game client connection lost"), m_pLogger);
		m_connected = false;
		m_isConnecting = false;
        break;
    }
}
