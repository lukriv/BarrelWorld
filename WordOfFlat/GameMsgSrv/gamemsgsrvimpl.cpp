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
	
	if (FWG_FAILED(result = GameConvertSocketStatus2GameErr(m_socketServer.listen(GAME_TCP_SERVICE_PORT))))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::Initialize() : Set socket listener failed: 0x%08x"),
							pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	// initialize selector
	m_socketSelector.add(m_socketServer);
	
	// create socket worker thread
	this->Create();
	
	// run socket worker thread
	this->Run();
	
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
	StopRequest();
	
	if (m_socketServer.)
	{
		m_socketServer.close();
	}
	
	Wait();
	
	for (wxDword i = 0; i < m_clientList.size(); ++i)
	{
		delete m_clientList[i];
		m_clientList[i] = NULL;
	}
	
}

GameErrorCode GameMsgSrv::ConnectRemoteClient(ClientInfo& client, sf::Socket* pSocket)
{
}

void* GameMsgSrv::Entry()
{
	//prepare selector
	GameErrorCode result = FWG_NO_ERROR;
	m_socketSelector.add(m_socketServer);
	
	do {
		if(m_socketSelector.wait(sf::milliseconds(5000))){
			if(m_socketSelector.isReady(m_socketServer))
			{
				// add new connection
				// The listener is ready: there is a pending connection
				sf::TcpSocket* client = new sf::TcpSocket;
				if (FWG_SUCCEDED(result = GameConvertSocketStatus2GameErr(m_socketSelector.accept(*client))))
				{
					// Add the new client to the clients list
					ClientInfo* pClinetInfo = FindNonActiveClient();
					
					if (FWG_SUCCEDED(result = ClientInfo->ClientConnect(client))) {
						selector.add(*client);
						FWGLOG_INFO_FORMAT(wxT("GameMsgSrv::Entry() : Client succesfully connected: %d"),
										m_pLogger, ClientInfo->GetAddress(), FWGLOG_ENDVAL);
					} else {
						FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::Entry() : Client connection failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
						delete client;
					}
					
				} else {
					// Error, we won't get a new connection, delete the socket
					FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::Entry() : Accept socket failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
					delete client;
				}
			} else {
				// try find what happen
				wxDword i = 0;
				ClientListType::iterator iter;
				for (iter = m_clientList.begin(); iter != m_clientList.end(); iter++)
				{
					if(m_socketSelector.isReady((*iter).GetSocket()))
					{
						sf::Packet packet;
						if(FWG_SUCCEDED(result = GameConvertSocketStatus2GameErr((*iter).GetSocket().receive(packet))))
						{
							//todo: Receive packet
							wxScopedPtr<GameMessage> spMessage;
							wxMemoryInputStream packetStream(packet.getData(), packet.getDataSize());
							spMessage.reset(new (std::nothrow) GameMessage);
							
							if (FWG_FAILED(result = message.Load(packetStream))) {
								FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::Entry() : Load message from packet failed: 0x%08x"), m_pLogger,
														result,
														FWGLOG_ENDVAL);
							} else {
								if (FWG_FAILED(result = m_msgQueue.Post(spMessage.release()))) {
									FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::Entry() : Post message to queue failed: 0x%08x"), m_pLogger,
															result,
															FWGLOG_ENDVAL);
								} else {
									FWGLOG_TRACE_FORMAT(wxT("GameMsgSrv::Entry() : Message was succesfully received: %d"), m_pLogger,
															(*iter).GetAddress(), FWGLOG_ENDVAL);
								}
							}
						} else {
							FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::Entry() : Packet receive failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
						}
					}
				}
			}
		}
		
	} while (!m_stopRequest);
	
}

GameErrorCode GameMsgSrv::StopRequest()
{
	m_stopRequest = true;	
}


ClientInfo* GameMsgSrv::FindNonActiveClient()
{
	GameErrorCode result = FWG_NO_ERROR;
	ClientListType iter;
	for (iter = m_clientList.begin(); iter != m_clientList.end(); iter++) 
	{
		if(!(*iter).IsActive()) return iter;
	}
	
	return NULL;
}



//--------------------------------------------------------------
//--------------------GameMsgSrv::ClientInfo--------------------
//--------------------------------------------------------------

GameErrorCode GameMsgSrv::ClientInfo::ClientConnect(sf::Socket* pSocket)
{
	GameErrorCode result = FWG_NO_ERROR;
	m_active = true;
	m_local = (pSocket == NULL);
	m_pSocket = pSocket;
	return result;
}

GameErrorCode GameMsgSrv::ClientInfo::ClientDisconnect()
{
	if (m_pSocket != NULL)
	{
		m_pSocket->close();
		delete m_pSocket;
		m_pSocket = NULL;
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
	
	wxMemoryOutputStream outputStream;
	wxStreamBuffer *streamBuffer = NULL;
	sf::Packet packet;
	
	if (FWG_FAILED(result = msg.Store(outputStream)))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::ClientInfo::SendMsg() : Message serialize failed: 0x%08x"),
						m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	streamBuffer = outputStream.GetOutputStreamBuffer();
	
	packet.append(streamBuffer->GetBufferStart(), streamBuffer->GetBufferSize());
	
	if(FWG_FAILED(result = GameConvertSocketStatus2GameErr(m_pSocket->send(packet))))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameMsgSrv::ClientInfo::SendMsg() : Send packet failed: 0x%08x"),
						m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
	}
	
	FWGLOG_TRACE_FORMAT(wxT("GameMsgSrv::ClientInfo::SendMsg() : Message sended, type( %d ), target ( %d )"),
					m_pOwner->GetLogger(), msg.GetType(), msg.GetTarget(), FWGLOG_ENDVAL);
	
	return result;
}


