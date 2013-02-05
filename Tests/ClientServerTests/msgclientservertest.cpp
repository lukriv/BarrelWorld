#include <UnitTest++.h>

#include <wx/msgout.h>

SUITE(MsgServerClientTest)
{
	TEST(LocalServerClientTest)
	{
		wxMessageOutputStderr output();
		output.DoPrintfWchar(wxT("SimpleTest: formated message"));
	}
}