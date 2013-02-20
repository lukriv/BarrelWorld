#include <UnitTest++/UnitTest++.h>


#include <wx/string.h>
#include <wx/log.h>


SUITE(MsgServerClientTest)
{
	TEST(LocalServerClientTest)
	{
		wxPrintf(wxT("Testing log\n"));
	}
}