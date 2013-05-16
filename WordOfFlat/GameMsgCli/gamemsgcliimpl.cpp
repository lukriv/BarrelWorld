#include "gamemsgcliimpl.h"
#include <wx/scopedptr.h>
#include "../GameSystem/gsocket.h"
#include "../GameComm/GameMessageImpl.h"
#include "../GameComm/gamemsgdata.h"

static const long CLIENT_CONNECTION_TIMEOUT = 30;

static void GameClientConnectScopeGuard(GameMsgCli *pMsgCli) 
{
	if (pMsgCli){
		pMsgCli->Destroy();
	}
}

GameErrorCode GameMsgCli::Initialize(GameLogger* pLogger)
{
	GameErrorCode result = FWG_NO_ERROR;
	
	// set hostname
	
	if(FWG_FAILED(result = GameCliClbkWorkerPool::Initialize(2, pLogger))) {
		FWGLOG_ERROR_FORMAT(wxT("GameMsgCli::Initialize() : Worker pool initialization failed: 0x%08x"),
							pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	m_hostName.assign(wxT("127.0.0.1"));
	m_stopRequrest = false;
	m_socketClient.setBlocking(true);
	
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
	
	if (m_connected)
	{
		return FWG_NO_ERROR;
	}
	

	
	wxScopeGuard guard = wxMakeGuard(GameClientConnectScopeGuard, this);
	
	sf::IpAddress ipAddr(m_hostName.GetData().AsChar());

	if(FWG_FAILED(result = GameConvertSocketStatus2GameErr(m_socketClient.connect( ipAddr, GAME_TCP_SERVICE_PORT, sf::milliseconds(30000)))))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameMsgCli::Connect() : Client connect failed: 0x%08x"),
						m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	// create socket worker thread
	if (FWG_FAILED(result = this->Create())) {
		FWGLOG_ERROR_FORMAT(wxT("GameMsgCli::Connect() : Create worker thread failed: 0x%08x"),
						m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	// reset stop request and add client to selector
	m_stopRequrest = false;
	m_socketSelector.add(m_socketClient);
	
	// run socket worker thread
	if (FWG_FAILED(result = this->Run())) {
		FWGLOG_ERROR_FORMAT(wxT("GameMsgCli::Connect() : Start thread failed: 0x%08x"),
						m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(m_cliAddr == GAME_ADDR_UNKNOWN)
	{
		GameMessage msg;
		ClientIdRequestData data;
		wxMemoryOutputStream outputStream;
		sf::Packet packet;
		
		msg.SetTarget(GAME_ADDR_SERVER);
		msg.SetMessage(data, GAME_MSG_TYPE_CLIENT_ID_REQUEST);
		if(FWG_FAILED(result = msg.Store(outputStream))) {
			FWGLOG_ERROR_FORMAT(wxT("GameMsgCli::Connect() : Message sending failed: 0x%08x"),
						m_pLogger, result, FWGLOG_ENDVAL);
			m_socketClient.disconnect();
			return result;
		}
		packet.append(outputStream.GetOutputStreamBuffer()->GetBufferStart(), outputStream.GetSize());
				
		if(FWG_FAILED(result = GameConvertSocketStatus2GameErr(m_socketClient.send(packet))))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameMsgCli::Connect() : Send client address request failed: 0x%08x"),
						m_pLogger, result, FWGLOG_ENDVAL);
			m_socketClient.disconnect();
			return result;
		}
	}
	
	guard.Dismiss();
	
	m_connected = true;	
	return FWG_NO_ERROR;
}

GameErrorCode GameMsgCli::Disconnect()
{
	{
		wxCriticalSectionLocker lock(m_clientLock);
		if(!m_connected)
		{
			return FWG_NO_ERROR;
		}
	
		m_socketClient.disconnect();
		
		StopRequest();
	}	
	Wait();
	
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
		return FWG_E_DISCONNECTED_ERROR;
	}
	
	GameErrorCode result = FWG_NO_ERROR;
	wxMemoryOutputStream outputStream;
	sf::Packet packet;
	wxCriticalSectionLocker lock(m_clientLock);
	
	msg.SetSource(m_cliAddr);
	
	if(FWG_FAILED(result = msg.Store(outputStream))) {
		FWGLOG_ERROR_FORMAT(wxT("GameMsgCli::SendMsg() : Message serialize failed: 0x%08x"),
					m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	packet.append(outputStream.GetOutputStreamBuffer()->GetBufferStart(), outputStream.GetSize());
				
	if(FWG_FAILED(result = GameConvertSocketStatus2GameErr(m_socketClient.send(packet))))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameMsgCli::SendMsg() : Send message failed: 0x%08x"),
					m_pLogger, result, FWGLOG_ENDVAL);
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
	if( IsRunning() || IsPaused()) {
		StopRequest();
		Wait();
	}

	m_socketClient.disconnect();
	
	m_isInitialized = false;
}

GameErrorCode GameMsgCli::SetServerAddress(const wxString& hostName)
{
	m_hostName = hostName;
	return FWG_NO_ERROR;
	
}

void* GameMsgCli::Entry()
{
	GameErrorCode result = FWG_NO_ERROR;
	
	
	
	do {
		
		if( m_socketSelector.wait(sf::milliseconds(2000))) //
		{
			//wxPrintf(wxT("GameMsgCli::Entry() - wait on lock\n"));
			wxCriticalSectionLocker lock(m_clientLock);
			//wxPrintf(wxT("GameMsgCli::Entry() - lock\n"));
			if (m_socketSelector.isReady(m_socketClient))
			{
				sf::Packet packet;
				wxScopedPtr<GameMessage> apMessage;
				
				do {
					if (FWG_FAILED(result = GameConvertSocketStatus2GameErr(m_socketClient.receive(packet))))
					{
						if(result == static_cast<GameErrorCode>(FWG_E_DISCONNECTED_ERROR))
						{
							FWGLOG_INFO_FORMAT(wxT("GameMsgCli::Entry() : Disconnect server %s"), m_pLogger, m_hostName.GetData().AsChar(), FWGLOG_ENDVAL);
							m_stopRequrest = true;
							m_connected = false;
						} else {
							FWGLOG_ERROR_FORMAT(wxT("GameMsgCli::Entry() : Packet receive failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
						}
						break;
					}

					wxMemoryInputStream inputStream(packet.getData(), packet.getDataSize());
					
					apMessage.reset(new (std::nothrow) GameMessage);
					if (apMessage.get() == NULL) {
						FWGLOG_ERROR(wxT("GameMsgCli::Entry() : Memory allocation error"), m_pLogger);
						break;
					}
										
					if (FWG_FAILED(result = apMessage->Load(inputStream)))
					{
						FWGLOG_ERROR_FORMAT(wxT("GameMsgCli::Entry() : Message loading failed: 0x%08x"),
										m_pLogger, result, FWGLOG_ENDVAL);
						break;
					}
					
					switch (apMessage->GetType())
					{
						case GAME_MSG_TYPE_CLIENT_ID_REQUEST:
						{
							ClientIdRequestData data;
							if(FWG_FAILED(result = apMessage->GetMessage(data))) {
								FWGLOG_ERROR_FORMAT(wxT("GameMsgCli::Entry() : Get data failed: 0x%08x"),
											m_pLogger, result, FWGLOG_ENDVAL);
								break;
							}
							
							m_cliAddr = data.GetClientId();
							
							break;
						}	
						default:
							if (FWG_FAILED(result = m_msgQueue.Post(apMessage.release())))
							{
								FWGLOG_ERROR_FORMAT(wxT("GameMsgCli::Entry() : Message post failed: 0x%08x"), 
												m_pLogger, result, FWGLOG_ENDVAL);
								break;
							}
					}
	
					//wxPrintf(wxT("GameMsgCli::Entry() - loop\n"));
				} while (0);
			}
			//wxPrintf(wxT("GameMsgCli::Entry() - unlock\n"));
		}
	} while (!m_stopRequrest);
	
	return 0;
}

void GameMsgCli::StopRequest()
{
	m_stopRequrest = true;
}
