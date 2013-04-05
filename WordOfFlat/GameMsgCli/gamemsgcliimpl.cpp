#include "gamemsgcliimpl.h"
#include <wx/sckaddr.h>


static void GameClientScopeGuard(GameMsgCli *pMsgCli) 
{
	if (pMsgCli){
		pMsgCli->Destroy();
	}
}

GameErrorCode GameMsgCli::Initialize(GameLogger* pLogger)
{
	GameErrorCode result = FWG_NO_ERROR;
	
	wxScopeGuard guard = wxMakeGuard(GameClientScopeGuard, this);
	
	
	guard.Dismiss();
	
	m_isInitialized = true;	
	return result;
}


GameErrorCode GameMsgCli::Connect()
{
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}

GameErrorCode GameMsgCli::Disconnect()
{
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}

GameAddrType GameMsgCli::GetCliAddress()
{
	return m_cliAddr;
}

bool GameMsgCli::IsConnected()
{
	return false;
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
	return FWG_E_NOT_IMPLEMENTED_ERROR;
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
}
