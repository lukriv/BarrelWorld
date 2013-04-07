#include <wx/scopedptr.h>
#include <wx/scopeguard.h>
#include "gamemsgsrvimpl.h"
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
		
		return result;
	} else {
		return FWG_E_NOT_IMPLEMENTED_ERROR;
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
	m_clientList.resize(CLIENT_LIST_SIZE);

	
	if(m_isInitialized) {
		return FWG_NO_ERROR;
	}
	
	wxScopeGuard scopeGuard = wxMakeGuard(ScopeGuardMsgSrv, this);
	
	//initialize client list
	for (iter = m_clientList.begin(); iter != m_clientList.end(); iter++)
	{
		iter->m_active = false;
	}
	
	if(FWG_FAILED(result = GameCliClbkWorkerPool::Initialize(CALLBACK_THREAD_NR, pLogger)))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::Initialize() : Initialization GameCliClbkWorkerPool failed: 0x%08x"),
							pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	// initialize socket server
	wxIPV4address addr;
	if(addr.Service(GAME_SERVICE_PORT))
	{
		return FWG_E_PORT_ERROR;
	}
	
	m_pSocketServer = new wxSocketServer(addr, wxSOCKET_BLOCK);
	if (!m_pSocketServer) {
		FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::Initialize() : Creation of m_pSocketServer failed: 0x%08x"),
							pLogger, FWG_E_MEMORY_ALLOCATION_ERROR, FWGLOG_ENDVAL);
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	if (m_pSocketServer.Error()) {
		result = GameConvertWxSocketErr2GameErr(m_pSocketServer.LastError());
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
	}
	
	scopeGuard.Dismiss();
	
	m_isInitialized = true;
	
	return FWG_NO_ERROR;
}


GameErrorCode GameMsgSrv::Connect()
{
	GameErrorCode result = FWG_NO_ERROR;
	ClientInfo* pCliInfo = NULL;
	
	if(!m_isInitialized) {
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	//activate server-client in client list
	pCliInfo = &m_clientList[m_address-1];
	
	pCliInfo->m_local = true;
	pCliInfo->m_active = true;
	
	//set is connected
	m_isConnected = true;
	
	return result;
}

GameErrorCode GameMsgSrv::Disconnect()
{
	GameErrorCode result = FWG_NO_ERROR;
	ClientInfo* pCliInfo = NULL;
	
	if(!m_isInitialized) {
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	pCliInfo = &m_clientList[m_address-1];
	
	pCliInfo->m_active = false;
	
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
				wxSocketBase *  pSocket = pServerSocket->Accept(true);
				
				wxCriticalSectionLocker locker(m_clientLock);
				
				for (freeIndex = 1; freeIndex < m_clientList.size(); freeIndex++)
				{
					if(m_clientList[freeIndex].m_active == false)
					{
						if (FWG_FAILED(result = ConnectRemoteClient(m_clientList[freeIndex], pSocket)))
						{
							FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::SocketReceiver() : Connect client failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
						}
						break;
					}
				}
				
				if (freeIndex == m_clientList.size())
				{
					m_clientList.push_back(GameMsgSrv::ClientInfo());
					if (FWG_FAILED(result = ConnectRemoteClient(m_clientList[freeIndex], pSocket)))
					{
						FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::SocketReceiver() : Connect client failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
					}
				}
				
				
			}
			break;
		default:
			FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::SocketReceiver() : Unknown event: %d"), m_pLogger, event.GetEventType(), FWGLOG_ENDVAL);
			break;
	}
	
}

GameErrorCode GameMsgSrv::ConnectRemoteClient(ClientInfo& client, wxSocketBase* pSocket)
{
	// set client count
	client.m_spSocket.reset(pSocket);
	client.m_active = true;
	client.m_local = false;
	// add client count
	m_clientCount++;
	// set events
	client.Bind(wxEVT_SOCKET, &ClientInfo::SocketEvent, client, wxID_ANY);
	// set socket server event handling
	pSocket->SetEventHandler(client);
	pSocket->SetNotify(wxSOCKET_INPUT_FLAG|wxSOCKET_LOST_FLAG);
	pSocket->Notify(true);
	
	return FWG_NO_ERROR;
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
		
		break;
	case wxSOCKET_LOST:
		break;
	default:
		FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::ClientInfo::SocketEvent() : Unknown event: %d"), m_pLogger, event.GetEventType(), FWGLOG_ENDVAL);
		break;
	}
}
