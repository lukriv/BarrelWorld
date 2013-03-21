#include <UnitTest++/UnitTest++.h>


#include <wx/string.h>
#include <wx/log.h>
#include <wx/scopedptr.h>

#include "../../WordOfFlat/GameMsgSrv/gamemsgsrvimpl.h"
#include "../../WordOfFlat/GameComm/gamemessages.h"

class TestMsgCallback : public IGameMsgCallback {
	wxAtomicInt m_callCounter;
	wxAtomicInt m_errorCall;
	wxDword m_expectedValue;
public:
	TestMsgCallback() : m_callCounter(0), m_errorCall(0), m_expectedValue(0) {}
	virtual GameErrorCode OnNewMessage(const IGameMessage& msg)
	{
		if(GAME_MSG_TYPE_TEST == msg.GetType())
		{
			TestMessage* pTestMsg = (TestMessage*) (&msg); // retype
			if(m_expectedValue == pTestMsg->GetTestValue())
			{
				wxAtomicInc(m_callCounter);
			} else {
				wxAtomicInc(m_errorCall);
			}
			
		} else {
			wxAtomicInc(m_errorCall);
		}
			
			
		return FWG_NO_ERROR;
					
	}
	
	void SetExpectedValue(wxDword expectedValue)
	{
		m_expectedValue = expectedValue;
	}
	
	wxInt32 GetCallCount() 
	{
		return m_callCounter;
	}
	
	wxInt32 GetErrorCall()
	{
		return m_errorCall;
	}
	
	void ClearCouters()
	{
		m_callCounter = 0;
		m_errorCall = 0;
	}
};


SUITE(MsgServerClientTest)
{
	TEST(ServerClient_LocalConnectTest)
	{
		wxPrintf(wxT("ServerClient_LocalConnectTest\n"));
		GameMsgSrv serverCli;
		
		CHECK(FWG_SUCCEDED(serverCli.Initialize(NULL)));
		CHECK(false == serverCli.IsConnected());
		
		CHECK(FWG_SUCCEDED(serverCli.Connect()));
		CHECK(true == serverCli.IsConnected());
		CHECK(true == serverCli.IsLocal());
		CHECK(GAME_ADDR_SERVER == serverCli.GetCliAddress());
		
		CHECK(FWG_SUCCEDED(serverCli.Disconnect()));
		CHECK(false == serverCli.IsConnected());
			
	}
	
	TEST(ServerClient_LocalCallbackSendTest)
	{
		GameMsgSrv serverCli;
		TestMessage testMsg;
		wxPrintf(wxT("ServerClient_LocalCallbackSendTest\n"));
		
		const wxDword testValue = 111;
		
		TestMsgCallback testClbk;
		testClbk.SetExpectedValue(testValue);
		
		CHECK(FWG_SUCCEDED(serverCli.Initialize(NULL)));
		CHECK(FWG_SUCCEDED(serverCli.Connect()));
		CHECK(true == serverCli.IsConnected());
		CHECK(true == serverCli.IsLocal());
		
		CHECK(FWG_SUCCEDED(serverCli.RegisterCallback(GAME_MSG_TYPE_TEST, &testClbk)));
		
		testMsg.SetTarget(GAME_ADDR_SERVER);
		testMsg.SetTestValue(testValue);
		CHECK(FWG_SUCCEDED(serverCli.SendMsg(testMsg, 0)));
		wxThread::Sleep(50);
		
		CHECK(1 == testClbk.GetCallCount());
		CHECK(0 == testClbk.GetErrorCall());
		
		testClbk.ClearCouters();
		
		CHECK(0 == testClbk.GetCallCount());
		CHECK(0 == testClbk.GetErrorCall());
		
		CHECK(FWG_SUCCEDED(serverCli.UnregisterCallback(GAME_MSG_TYPE_TEST, &testClbk)));
		
		CHECK(FWG_SUCCEDED(serverCli.SendMsg(testMsg, 0)));
		wxThread::Sleep(50);
		
		CHECK(0 == testClbk.GetCallCount());
		CHECK(0 == testClbk.GetErrorCall());
		
		CHECK(FWG_SUCCEDED(serverCli.RegisterCallback(GAME_MSG_TYPE_TEST, &testClbk)));
		
		testMsg.SetTarget(GAME_ADDR_SERVER);
		testMsg.SetTestValue(testValue);
		CHECK(FWG_SUCCEDED(serverCli.SendMsg(testMsg, 0)));
		CHECK(FWG_SUCCEDED(serverCli.SendMsg(testMsg, 0)));
		CHECK(FWG_SUCCEDED(serverCli.SendMsg(testMsg, 0)));
		CHECK(FWG_SUCCEDED(serverCli.SendMsg(testMsg, 0)));
		wxThread::Sleep(50);
		
		CHECK(4 == testClbk.GetCallCount());
		CHECK(0 == testClbk.GetErrorCall());
		
		CHECK(FWG_SUCCEDED(serverCli.UnregisterCallback(GAME_MSG_TYPE_TEST, &testClbk)));
	
		
	}
}