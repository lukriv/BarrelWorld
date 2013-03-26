#include <UnitTest++/UnitTest++.h>

#include <wx/string.h>
#include <wx/log.h>
#include <wx/scopedptr.h>
#include <wx/mstream.h>

#include "../../WordOfFlat/GameComm/GameMessage.h"
#include "../../WordOfFlat/GameComm/gamemessageimpl.h"
#include "../../WordOfFlat/GameComm/gamemsgdata.h"

SUITE(MessagesTests)
{
	
	TEST(GameMessageEmptyTest)
	{
		wxMemoryOutputStream oMemStream;
		
		GameMessage testMessage, loadedMessage;
		
		IGameMessage *pTestMsg = NULL;
		wxScopedPtr<GameMessage> spTestMsg;
		
		
		GameAddrType tarAddr = 3;
		GameAddrType tarId = 5;
		GameAddrType srcAddr = 7;
		GameAddrType srcId = 8;
		//wxDword value = 55;
		
		wxPrintf(wxT("GameMessageEmptyTest\n"));
		testMessage.SetSource(srcAddr);
		testMessage.SetSourceId(srcId);
		testMessage.SetTarget(tarAddr);
		testMessage.SetTargetId(tarId);
		//testMessage.SetTestValue(value);
		
		CHECK(GAME_MSG_TYPE_UNKNOWN == testMessage.GetType());
		CHECK(GameMsgVersion == testMessage.GetVersion());
		
		CHECK(FWG_SUCCEDED(testMessage.Store(oMemStream)));
		
		wxMemoryInputStream iMemStream(oMemStream);
		CHECK(FWG_SUCCEDED(loadedMessage.Load(iMemStream)));
		
		CHECK(tarAddr == loadedMessage.GetTarget());
		CHECK(tarId == loadedMessage.GetTargetId());
		CHECK(srcAddr == loadedMessage.GetSource());
		CHECK(srcId == loadedMessage.GetSourceId());
		//CHECK(value == loadedMessage.GetTestValue());
		
		CHECK(FWG_SUCCEDED(loadedMessage.CreateCopy(pTestMsg)));
		spTestMsg.reset((GameMessage*) pTestMsg);
		
		CHECK(GAME_MSG_TYPE_UNKNOWN == pTestMsg->GetType());
		
		CHECK(tarAddr == spTestMsg->GetTarget());
		CHECK(tarId == spTestMsg->GetTargetId());
		CHECK(srcAddr == spTestMsg->GetSource());
		CHECK(srcId == spTestMsg->GetSourceId());
		//CHECK(value == spTestMsg->GetTestValue());
		
	}
	
	TEST(GameMessageDataTest)
	{
		wxMemoryOutputStream oMemStream;
		
		GameMessage testMessage, loadedMessage;
		
		TestMessageData testDataOrig, testDataLoaded;
		
		IGameMessage *pTestMsg = NULL;
		wxScopedPtr<GameMessage> spTestMsg;
		
		
		wxDword value = 55;
		
		wxPrintf(wxT("GameMessageDataTest\n"));
		
		// set get test
		testDataOrig.SetTestValue(value);
		CHECK(value == testDataOrig.GetTestValue());
		
		// type and message set test
		CHECK(GAME_MSG_TYPE_UNKNOWN == testMessage.GetType());
		CHECK(FWG_SUCCEDED(testMessage.SetMessage(testDataOrig, GAME_MSG_TYPE_TEST)));
		CHECK(GAME_MSG_TYPE_TEST == testMessage.GetType());
		
		// message get test
		CHECK(FWG_SUCCEDED(testMessage.GetMessage(testDataLoaded)));
		CHECK(value == testDataLoaded.GetTestValue());
		CHECK(TestMessageData::TEST_MESSAGE_VERSION == testDataLoaded.GetVersion());
		
		// reset testDataLoaded
		testDataLoaded.SetTestValue(0);
				
		CHECK(GameMsgVersion == testMessage.GetVersion());
		
		//load store message
		CHECK(FWG_SUCCEDED(testMessage.Store(oMemStream)));
		wxMemoryInputStream iMemStream(oMemStream);
		CHECK(FWG_SUCCEDED(loadedMessage.Load(iMemStream)));
		
		//load data test
		CHECK(GAME_MSG_TYPE_TEST == loadedMessage.GetType());
		CHECK(FWG_SUCCEDED(loadedMessage.GetMessage(testDataLoaded)));
		CHECK(value == testDataLoaded.GetTestValue());
		
		// reset testDataLoaded
		testDataLoaded.SetTestValue(0);
		
		// create copy
		CHECK(FWG_SUCCEDED(loadedMessage.CreateCopy(pTestMsg)));
		spTestMsg.reset((GameMessage*)pTestMsg);
		
		CHECK(GAME_MSG_TYPE_TEST == pTestMsg->GetType());
		
		//load data test
		CHECK(GAME_MSG_TYPE_TEST == pTestMsg->GetType());
		CHECK(FWG_SUCCEDED(pTestMsg->GetMessage(testDataLoaded)));
		CHECK(value == testDataLoaded.GetTestValue());
	}
	
}