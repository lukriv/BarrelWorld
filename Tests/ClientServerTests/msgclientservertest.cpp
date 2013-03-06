#include <UnitTest++/UnitTest++.h>


#include <wx/string.h>
#include <wx/log.h>

#include "../GameMsgSrv/gamemsgsrvimpl.h"


class TestMsgCallback : public IGameMsgCallback {
public:
	virtual GameErrorCode OnNewMessage(const IGameMessage& msg)
	{
		
	}
};

SUITE(MsgServerClientTest)
{
	TEST(LocalServerClientTest)
	{
		wxPrintf(wxT("Testing log\n"));
		GameMsgSrv serverCli;
		
	}
}