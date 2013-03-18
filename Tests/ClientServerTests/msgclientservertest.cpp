#include <UnitTest++/UnitTest++.h>


#include <wx/string.h>
#include <wx/log.h>
#include <wx/scopedptr.h>

#include "../../WordOfFlat/GameMsgSrv/gamemsgsrvimpl.h"
#include "../../WordOfFlat/GameComm/gamemessages.h"

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