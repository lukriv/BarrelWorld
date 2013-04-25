#ifndef __GAME_MESSAGES_DATA_IMPL_H__
#define __GAME_MESSAGES_DATA_IMPL_H__


#include "GameDataImpl.h"
#include "../GameSystem/gstream.h"

// client id request message data
class ClientIdRequestData : public GameDataBase {
	GameAddrType m_clientId;
public:
	static const GameVersionType CLIENT_ID_REQUEST_VERSION = 1;
public:
	ClientIdRequestData() : GameDataBase(CLIENT_ID_REQUEST_VERSION), m_clientId(GAME_ADDR_UNKNOWN) {}
		
	void SetClientId(GameAddrType cliId) { m_clientId = cliId; }
	GameAddrType GetClientId() {return m_clientId; }
	
protected:
	virtual GameErrorCode LoadData(wxDataInputStream &iDataStream)
	{
		m_clientId = (wxUint32) iDataStream.Read32();
		return FWG_NO_ERROR;
	}	

	virtual GameErrorCode StoreData(wxDataOutputStream &oDataStream) const 
	{
		oDataStream.Write32((wxUint32) m_clientId);
		return FWG_NO_ERROR;
	}
};

// testing message data
class TestMessageData : public GameDataBase {
	wxDword m_testValue;
public:
	static const GameVersionType TEST_MESSAGE_VERSION = 1;
public:
	
	TestMessageData() : GameDataBase(TEST_MESSAGE_VERSION), m_testValue(0) {}
	
	void SetTestValue(wxDword test) { m_testValue = test; }
	wxDword GetTestValue() { return m_testValue; }
	
protected:
	virtual GameErrorCode LoadData(wxDataInputStream &iDataStream)
	{
		m_testValue = (wxDword) iDataStream.Read32();
		return FWG_NO_ERROR;
	}	

	virtual GameErrorCode StoreData(wxDataOutputStream &oDataStream) const 
	{
		oDataStream.Write32((wxUint32) m_testValue);
		return FWG_NO_ERROR;
	}
};





#endif //__GAME_MESSAGES_IMPL_H__