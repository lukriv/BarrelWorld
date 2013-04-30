#include <wx/scopedptr.h>
#include <wx/scopeguard.h>
#include <wx/sckstrm.h>
#include "gamemsgsrvimpl.h"
#include "../GameComm/GameMessageImpl.h"
#include "../GameComm/gamemsgdata.h"
#include "../GameSystem/glog.h"
#include "../GameSystem/gsocket.h"

#define CLIENT_LIST_SIZE 10

static const wxDword CALLBACK_THREAD_NR = 2;


static void ScopeGuardMsgSrv (GameMsgSrv *pSrv)
{
	if (pSrv != NULL) {
		pSrv->Destroy();
	}
}


GameAddrType GameMsgSrv::GetCliAddress()
{
	return m_address;
}

bool GameMsgSrv::IsConnected()
{
	return m_isConnected;
}

bool GameMsgSrv::IsLocal()
{
	return true;
}

GameErrorCode GameMsgSrv::RegisterCallback(GameMessageType msgType, IGameMsgCallback* pClbk)
{ 
	return RegisterCallbackInternal(msgType, pClbk);
}

GameErrorCode GameMsgSrv::SendMsg(IGameMessage &msg, long timeout)
{
	GameErrorCode result = FWG_NO_ERROR;
	msg.SetSource(m_address);
	
	if(!m_isInitialized) {
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	GameAddrType target = msg.GetTarget();
	if (GAME_ADDR_UNKNOWN == target)
	{
		return FWG_E_UNKNOWN_TARGET_ERROR;
	}
	
	if (m_address == target)
	{
		IGameMessage* copy = NULL;
		if(FWG_FAILED(result = msg.CreateCopy(copy)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::SendMsg() : Create message copy failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
		if(FWG_FAILED(result = m_msgQueue.Post(copy)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::SendMsg() : Post message to queue failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
	} else {
		if((m_clientList.size() < msg.GetTarget())||(!m_clientList[msg.GetTarget()-1]->IsActive()))
		{
			return FWG_E_UNKNOWN_TARGET_ERROR;
		}
		
		if(FWG_FAILED(result = m_clientList[msg.GetTarget() - 1]->SendMsg(msg, timeout)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::SendMsg() : Send message failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
	}
	
	return result;
}

GameErrorCode GameMsgSrv::UnregisterCallback(GameMessageType msgType, IGameMsgCallback* pClbk)
{
	return UnregisterCallbackInternal(msgType, pClbk);
}

GameErrorCode GameMsgSrv::Initialize(GameLogger* pLogger)
{
	//TODO: scope guard
	GameErrorCode result = FWG_NO_ERROR;
	ClientListType::iterator iter;
	ClientInfo *pClientInfo = NULL;
	
	if(m_isInitialized) {
		return FWG_NO_ERROR;
	}
	
	wxScopeGuard scopeGuard = wxMakeGuard(ScopeGuardMsgSrv, this);
	
	//initialize client list
	for (wxDword i = 0; i < CLIENT_LIST_SIZE; ++i)
	{
		pClientInfo = new (std::nothrow) ClientInfo(this);
		if (pClientInfo == NULL) {
			FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::Initialize() : Create client info failed: 0x%08x"),
							pLogger, FWG_E_MEMORY_ALLOCATION_ERROR, FWGLOG_ENDVAL);
			return FWG_E_MEMORY_ALLOCATION_ERROR;
		}
		pClientInfo->SetAddress((GameAddrType) (i+1));
		m_clientList.push_back(pClientInfo);
	}
	
	if(FWG_FAILED(result = GameCliClbkWorkerPool::Initialize(CALLBACK_THREAD_NR, pLogger)))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::Initialize() : Initialization GameCliClbkWorkerPool failed: 0x%08x"),
							pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	// initialize socket server
	wxIPV4address addr;
	addr.Service(GAME_SERVICE_PORT);
	
	/*m_pSocketServer = new wxSocketServer(addr, wxSOCKET_BLOCK);
	if (!m_pSocketServer) {
		FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::Initialize() : Creation of m_pSocketServer failed: 0x%08x"),
							pLogger, FWG_E_MEMORY_ALLOCATION_ERROR, FWGLOG_ENDVAL);
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	if (m_pSocketServer->Error()) {
		result = GameConvertWxSocketErr2GameErr(m_pSocketServer->LastError());
		FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::Initialize() : Socket server initialization failed: 0x%08x"),
						pLogger, result, FWGLOG_ENDVAL);
		return result;	
	}
	
	Bind(wxEVT_SOCKET, &GameMsgSrv::SocketReceiver, this, wxID_ANY);
	
	// set socket server event handling
	m_pSocketServer->SetEventHandler(*this);
	m_pSocketServer->SetNotify(wxSOCKET_CONNECTION_FLAG);
	m_pSocketServer->Notify(true);
	
	if (!m_pSocketServer->IsOk())
	{
		FWGLOG_ERROR(wxT("GameMsgSrv::Initialize() : Initialization of socket server failed: Server is not ready"),	pLogger);
		return FWG_E_MISC_ERROR;
	}*/
	
	scopeGuard.Dismiss();
	
	m_isInitialized = true;
	
	return FWG_NO_ERROR;
}


GameErrorCode GameMsgSrv::Connect()
{
	GameErrorCode result = FWG_NO_ERROR;
	
	if(!m_isInitialized) {
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	//activate server-client in client list
	if (FWG_FAILED(result = m_clientList[m_address-1]->ClientConnect(NULL)))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::Connect() : Client connect failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
		
	//set is connected
	m_isConnected = true;
	
	return result;
}

GameErrorCode GameMsgSrv::Disconnect()
{
	GameErrorCode result = FWG_NO_ERROR;
		
	if(!m_isInitialized) {
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	if(FWG_FAILED(result = m_clientList[m_address-1]->ClientDisconnect()))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::Disconnect() : Client disconnecting failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
	}
	
	m_isConnected = false;
	
	return result;
}

void GameMsgSrv::addRef()
{
	wxAtomicInc(m_refCount);
}

wxInt32 GameMsgSrv::release()
{
	wxInt32 refCount = wxAtomicDec(m_refCount);
	if (refCount == 0)
	{
		delete this;
	}
	
	return refCount;
}


GameMsgSrv::~GameMsgSrv()
{
	Destroy();
}


void GameMsgSrv::Destroy()
{
	if (m_pSocketServer)
	{
		m_pSocketServer->Destroy();
	}
	
	for (wxDword i = 0; i < m_clientList.size(); ++i)
	{
		delete m_clientList[i];
		m_clientList[i] = NULL;
	}
	
}

void GameMsgSrv::SocketReceiver(wxSocketEvent& event)
{
	GameErrorCode result = FWG_NO_ERROR;
	switch(event.GetSocketEvent())
	{
		case wxSOCKET_CONNECTION:
   
			// Check if the server socket
			if (m_pSocketServer == (wxSocketServer*) event.GetSocket())
			{
				wxDword freeIndex = 0;
				wxSocketBase *  pSocket = m_pSocketServer->Accept(true);
				
				wxCriticalSectionLocker locker(m_clientLock);
				
				for (freeIndex = 1; freeIndex < m_clientList.size(); freeIndex++)
				{
					if(!m_clientList[freeIndex]->IsActive())
					{
						if (FWG_FAILED(result = m_clientList[freeIndex]->ClientConnect(pSocket)))
						{
							FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::SocketReceiver() : Connect client failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
						}
						break;
					}
				}
				
				if (freeIndex == m_clientList.size())
				{
					ClientInfo *pClient = new (std::nothrow) ClientInfo(this);
					if (pClient) {
						m_clientList.push_back(pClient);
						if (FWG_FAILED(result = pClient->ClientConnect(pSocket)))
						{
							FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::SocketReceiver() : Connect client failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
						}
					} else {
						FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::SocketReceiver() : Client creation failed: 0x%08x"),
								m_pLogger, FWG_E_MEMORY_ALLOCATION_ERROR, FWGLOG_ENDVAL);
					}
				}
				
				
			}
			break;
		default:
			FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::SocketReceiver() : Unknown event: %d"), m_pLogger, event.GetEventType(), FWGLOG_ENDVAL);
			break;
	}
	
}



//--------------------------------------------------------------
//--------------------GameMsgSrv::ClientInfo--------------------
//--------------------------------------------------------------
void GameMsgSrv::ClientInfo::SocketEvent(wxSocketEvent& event)
{
	GameErrorCode result = FWG_NO_ERROR;
	switch(event.GetSocketEvent())
	{
	case wxSOCKET_INPUT:
		if(event.GetSocket() == m_pSocket)
		{
			wxSocketInputStream socketStream(*m_pSocket);
			GameMessage* pMessage = NULL;
			pMessage = new (std::nothrow) GameMessage;
			if (pMessage == NULL) {
				FWGLOG_ERROR(wxT("GameMsgSrv::ClientInfo::SocketEvent() : Memory allocation error"), m_pOwner->GetLogger());
				return;
			}
			
			wxScopedPtr<GameMessage> apMessage(pMessage);
			
			if (FWG_FAILED(result = pMessage->Load(socketStream)))
			{
				FWGLOG_ERROR(wxT("GameMsgSrv::ClientInfo::SocketEvent() : Message loading error"), m_pOwner->GetLogger());
				return;
			}
			
			if (GAME_MSG_TYPE_CLIENT_ID_REQUEST == apMessage->GetType())
			{
				wxSocketOutputStream socketStream(*m_pSocket);
				ClientIdRequestData data;
				data.SetClientId(m_reservedAddr);
				apMessage->SetTarget(m_reservedAddr);
				apMessage->SetSource(GAME_ADDR_SERVER);
				apMessage->SetMessage(data, GAME_MSG_TYPE_CLIENT_ID_REQUEST);
				if (FWG_FAILED(result = apMessage->Store(socketStream)))
				{
					FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::ClientInfo::SocketEvent() : Message loading error"),
								m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
					return;
				}
			}
			
			m_pOwner->m_msgQueue.Post(apMessage.release());
			
		} else {
			FWGLOG_WARNING(wxT("GameMsgSrv::ClientInfo::SocketEvent() : Unknown socket"), m_pOwner->GetLogger());
		}
		break;
	case wxSOCKET_LOST:
		if(event.GetSocket() == m_pSocket)
		{
			if(!m_pSocket->Destroy())
			{
				result = GameConvertWxSocketErr2GameErr(m_pSocket->LastError());
				FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::ClientInfo::SocketEvent() : Memory allocation error"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
			}
			Unbind(wxEVT_SOCKET, &ClientInfo::SocketEvent, this, wxID_ANY);
			m_pSocket = NULL;
			
		}
		break;
	default:
		FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::ClientInfo::SocketEvent() : Unknown event: %d"), m_pOwner->GetLogger(), event.GetEventType(), FWGLOG_ENDVAL);
		break;
	}
}

GameErrorCode GameMsgSrv::ClientInfo::ClientConnect(wxSocketBase* pSocket)
{
	GameErrorCode result = FWG_NO_ERROR;
	if (pSocket == NULL)
	{
		m_active = true;
		m_local = true;
		if (m_pSocket != NULL)
		{
			m_pSocket->Destroy();
			m_pSocket = NULL;
			Unbind(wxEVT_SOCKET, &ClientInfo::SocketEvent, this, wxID_ANY);
		}
		
	} else {
		m_active = true;
		m_local = false;
		if (m_pSocket != NULL)
		{
			m_pSocket->Destroy();
			m_pSocket = NULL;
			Unbind(wxEVT_SOCKET, &ClientInfo::SocketEvent, this, wxID_ANY);
		}
		m_pSocket = pSocket;
		
		Bind(wxEVT_SOCKET, &ClientInfo::SocketEvent, this, wxID_ANY);
		// set socket server event handling
		pSocket->SetEventHandler(*this);
		pSocket->SetNotify(wxSOCKET_INPUT_FLAG|wxSOCKET_LOST_FLAG);
		pSocket->Notify(true);

	}
	return result;
}

GameErrorCode GameMsgSrv::ClientInfo::ClientDisconnect()
{
	if (m_pSocket != NULL)
	{
		m_pSocket->Destroy();
		m_pSocket = NULL;
		Unbind(wxEVT_SOCKET, &ClientInfo::SocketEvent, this, wxID_ANY);
	}
	
	m_active = false;
	m_local = false;
	return FWG_NO_ERROR;
}


GameErrorCode GameMsgSrv::ClientInfo::SendMsg(IGameMessage& msg, long timeout)
{
	if((!m_active)||(m_pSocket != NULL))
	{
		return FWG_E_INVALID_SOCKET_ERROR;
	}
	
	GameErrorCode result = FWG_NO_ERROR;
	wxSocketOutputStream socketOutStream(*m_pSocket);
	
	if(FWG_FAILED(result = msg.Store(socketOutStream)))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::ClientInfo::SendMsg() : Message store failed : 0x%08x"), 
					m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	FWGLOG_TRACE_FORMAT(wxT("GameMsgSrv::ClientInfo::SendMsg() : Message sended, type( %d ), target ( %d )"),
					m_pOwner->GetLogger(), msg.GetType(), msg.GetTarget(), FWGLOG_ENDVAL);
	
	return result;
}
