#include <UnitTest++/UnitTest++.h>

#include <wx/string.h>
#include <wx/log.h>
#include <wx/scopedptr.h>

#include "../../WordOfFlat/GameComm/gamemessages.h"

SUITE(MessagesTests)
{
	TEST(TestMessageTests)
	{
		wxMemoryOutputStream oMemStream;
		
		TestMessage testMessage, loadedMessage;
		IGameMessage *pTestMsg = NULL;
		wxScopedPtr<IGameMessage> spTestMsg;
		
		
		GameAddrType tarAddr = 3;
		GameAddrType tarId = 5;
		GameAddrType srcAddr = 7;
		GameAddrType srcId = 8;
		wxDword value = 55;
		
		wxPrintf(wxT("TestMessageTests\n"));
		testMessage.SetSource(srcAddr);
		testMessage.SetSourceId(srcId);
		testMessage.SetTarget(tarAddr);
		testMessage.SetTargetId(tarId);
		testMessage.SetTestValue(value);
		
		CHECK(GAME_MSG_TYPE_TEST == testMessage.GetType());
		CHECK(1 == testMessage.GetVersion());
		
		CHECK(FWG_FAILED(testMessage.Store(oMemStream)));
		
		wxMemoryInputStream iMemStream(oMemStream);
		CHECK(FWG_FAILED(loadedMessage.Load(iMemStream)));
		
		CHECK(tarAddr == loadedMessage.GetTarget());
		CHECK(tarId == loadedMessage.GetTargetId());
		CHECK(srcAddr == loadedMessage.GetSource());
		CHECK(srcId == loadedMessage.GetSourceId());
		CHECK(value == loadedMessage.GetVersion());
		
		CHECK(FWG_FAILED(loadedMessage.CreateCopy(pTestMsg)));
		spTestMsg.reset(pTestMsg);
		
		CHECK(GAME_MSG_TYPE_TEST == spTestMsg->GetType());
				
		
	}
}