#include <UnitTest++/UnitTest++.h>


#include <wx/string.h>
#include <wx/log.h>
#include <wx/scopedptr.h>
#include <iostream>

#include "../../WordOfFlat/GameSystem/glog.h"
#include "../../WordOfFlat/GameMsgSrv/gamemsgsrvimpl.h"
#include "../../WordOfFlat/GameMsgCli/gamemsgcliimpl.h"
#include "../../WordOfFlat/GameComm/gamemsgdata.h"
#include "../../WordOfFlat/GameComm/GameMessageImpl.h"

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
			TestMessageData testMsgData;
			msg.GetMessage(testMsgData);
			if(m_expectedValue == testMsgData.GetTestValue())
			{
				//wxPrintf(wxT("OnNewMessage succed: value = %d\n"), testMsgData.GetTestValue());
				wxAtomicInc(m_callCounter);
			} else {
				//wxPrintf(wxT("OnNewMessage failed: expected = %d ; from message = %d\n"), m_expectedValue, testMsgData.GetTestValue());
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
		TestMessageData testMsgData;
		GameMessage testMsg;
		wxPrintf(wxT("ServerClient_LocalCallbackSendTest\n"));
		
		GameLogger* pLogger;
		GameLoggerCreator::CreateLogger(pLogger);
		
		wxDword testValue = 111;
		
		TestMsgCallback testClbk;
		testClbk.SetExpectedValue(testValue);
		
		CHECK(FWG_SUCCEDED(serverCli.Initialize(pLogger)));
		CHECK(FWG_SUCCEDED(serverCli.Connect()));
		CHECK(true == serverCli.IsConnected());
		CHECK(true == serverCli.IsLocal());
		
		CHECK(FWG_SUCCEDED(serverCli.RegisterCallback(GAME_MSG_TYPE_TEST, &testClbk)));
		
		testMsg.SetTarget(GAME_ADDR_SERVER);
		testMsgData.SetTestValue(testValue);
		CHECK(FWG_SUCCEDED(testMsg.SetMessage(testMsgData, GAME_MSG_TYPE_TEST)));
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
		
		//change value
		testValue = 222;
		testClbk.SetExpectedValue(testValue);
		testMsg.SetTarget(GAME_ADDR_SERVER);
		testMsgData.SetTestValue(testValue);
		CHECK(FWG_SUCCEDED(testMsg.SetMessage(testMsgData, GAME_MSG_TYPE_TEST)));
		CHECK(FWG_SUCCEDED(serverCli.SendMsg(testMsg, 0)));
		wxThread::Sleep(50);
		testValue = 333;
		testClbk.SetExpectedValue(testValue);
		testMsg.SetTarget(GAME_ADDR_SERVER);
		testMsgData.SetTestValue(testValue);
		CHECK(FWG_SUCCEDED(testMsg.SetMessage(testMsgData, GAME_MSG_TYPE_TEST)));
		CHECK(FWG_SUCCEDED(serverCli.SendMsg(testMsg, 0)));
		wxThread::Sleep(50);
		testValue = 444;
		testClbk.SetExpectedValue(testValue);
		testMsg.SetTarget(GAME_ADDR_SERVER);
		testMsgData.SetTestValue(testValue);
		CHECK(FWG_SUCCEDED(testMsg.SetMessage(testMsgData, GAME_MSG_TYPE_TEST)));
		CHECK(FWG_SUCCEDED(serverCli.SendMsg(testMsg, 0)));
		wxThread::Sleep(50);
		testValue = 555;
		testClbk.SetExpectedValue(testValue);
		testMsg.SetTarget(GAME_ADDR_SERVER);
		testMsgData.SetTestValue(testValue);
		CHECK(FWG_SUCCEDED(testMsg.SetMessage(testMsgData, GAME_MSG_TYPE_TEST)));
		CHECK(FWG_SUCCEDED(serverCli.SendMsg(testMsg, 0)));
		wxThread::Sleep(50);
		
		CHECK(4 == testClbk.GetCallCount());
		CHECK(0 == testClbk.GetErrorCall());
		
		CHECK(FWG_SUCCEDED(serverCli.UnregisterCallback(GAME_MSG_TYPE_TEST, &testClbk)));
	
		
	}
	
	
	TEST(ClientServer_BasicRemoteConnectionTest)
	{
		RefObjSmPtr<GameLogger> spLogger;
		if (FWG_FAILED(GameLoggerCreator::CreateLogger(spLogger.OutRef(),wxT("default"))))
		{	
			printf("Failed to initialize the logger, aborting.");
		
		}
		
		wxPrintf(wxT("ClientServer_BasicRemoteConnectionTest\n"));
		
		GameMsgSrv server;
		GameMsgCli client;
		CHECK(FWG_SUCCEDED(server.Initialize(spLogger.In())));
		CHECK(FWG_SUCCEDED(client.Initialize(spLogger.In())));
		
		CHECK(FWG_SUCCEDED(server.Connect()));
		CHECK(server.IsConnected());
		
		CHECK(FWG_SUCCEDED(client.Connect()));
		CHECK(client.IsConnected());
		
		//std::cin.get();
		wxThread::Sleep(1000);
		CHECK(client.GetCliAddress() == GAME_ADDR_SERVER + 1);
		CHECK(client.IsLocal() == false);
	
		CHECK(FWG_SUCCEDED(client.Disconnect()));
		wxThread::Sleep(1000);

		//std::cin.get();
		CHECK(FWG_SUCCEDED(server.Disconnect()));
	}
	
	
	TEST(ClientServer_RemoteConnectionSendTest)
	{
		wxPrintf(wxT("ClientServer_RemoteConnectionSendTest\n"));
		
		wxDword serverTestValue = 111;
		wxDword clientTestValue = 222;
		
		TestMessageData testMsgData;
		GameMessage testMsg;
		
		TestMsgCallback serverTestClbk, clientTestClbk;
		serverTestClbk.SetExpectedValue(serverTestValue);
		clientTestClbk.SetExpectedValue(clientTestValue);
		
		GameMsgSrv server;
		GameMsgCli client;
		CHECK(FWG_SUCCEDED(server.Initialize(NULL)));
		CHECK(FWG_SUCCEDED(client.Initialize(NULL)));
		
		CHECK(FWG_SUCCEDED(server.Connect()));
		CHECK(server.IsConnected());
		
		CHECK(FWG_SUCCEDED(client.Connect()));
		CHECK(client.IsConnected());
		
		//std::cin.get();
		wxThread::Sleep(1000);
		CHECK(FWG_SUCCEDED(client.RegisterCallback(GAME_MSG_TYPE_TEST, &clientTestClbk)));
		CHECK(FWG_SUCCEDED(server.RegisterCallback(GAME_MSG_TYPE_TEST, &serverTestClbk)));
		CHECK(client.GetCliAddress() == GAME_ADDR_SERVER + 1);
		
		for (int i = 0; i < 5; i++)
		{
			testMsg.SetTarget(GAME_ADDR_SERVER);
			testMsgData.SetTestValue(serverTestValue);
			CHECK(FWG_SUCCEDED(testMsg.SetMessage(testMsgData, GAME_MSG_TYPE_TEST)));
			CHECK(FWG_SUCCEDED(client.SendMsg(testMsg, 0)));
			
			testMsg.SetTarget(GAME_ADDR_SERVER + 1);
			testMsgData.SetTestValue(clientTestValue);
			CHECK(FWG_SUCCEDED(testMsg.SetMessage(testMsgData, GAME_MSG_TYPE_TEST)));
			CHECK(FWG_SUCCEDED(server.SendMsg(testMsg, 0)));
		}
		
		wxThread::Sleep(1000);
		CHECK(FWG_SUCCEDED(client.UnregisterCallback(GAME_MSG_TYPE_TEST, &clientTestClbk)));
		CHECK(FWG_SUCCEDED(server.UnregisterCallback(GAME_MSG_TYPE_TEST, &serverTestClbk)));
		CHECK(5 == serverTestClbk.GetCallCount());
		CHECK(0 == serverTestClbk.GetErrorCall());
		
		CHECK(5 == clientTestClbk.GetCallCount());
		CHECK(0 == clientTestClbk.GetErrorCall());
	}
	
}