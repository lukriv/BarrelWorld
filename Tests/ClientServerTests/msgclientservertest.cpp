#include <UnitTest++/UnitTest++.h>


#include <wx/string.h>
#include <wx/log.h>


SUITE(MsgServerClientTest)
{
	TEST(LocalServerClientTest)
	{
		wxLog logger;
				
		logger.LogTextAtLevel(wxLOG_Status, wxString(wxT("Try logging")));
	}
}