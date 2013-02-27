#include <UnitTest++/UnitTest++.h>


#include <wx/string.h>
#include <wx/log.h>

#include "../GameMsgSrv/gamemsgsrvimpl.h"


SUITE(MsgServerClientTest)
{
	TEST(LocalServerClientTest)
	{
		wxPrintf(wxT("Testing log\n"));
		GameMsgSrv serverCli;
		
	}
}