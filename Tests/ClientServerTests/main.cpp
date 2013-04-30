#include <UnitTest++/UnitTest++.h>
#include <wx/init.h>
#include <wx/wxcrtvararg.h>
#include <wx/app.h>
#include <wx/socket.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// To add a test, simply put the following code in the a .cpp file of your choice:
//
// =================================
// Simple Test
// =================================
//
//  TEST(YourTestName)
//  {
//  }
//
// The TEST macro contains enough machinery to turn this slightly odd-looking syntax into legal C++, and automatically register the test in a global list. 
// This test list forms the basis of what is executed by RunAllTests().
//
// If you want to re-use a set of test data for more than one test, or provide setup/teardown for tests, 
// you can use the TEST_FIXTURE macro instead. The macro requires that you pass it a class name that it will instantiate, so any setup and teardown code should be in its constructor and destructor.
//
//  struct SomeFixture
//  {
//    SomeFixture() { /* some setup */ }
//    ~SomeFixture() { /* some teardown */ }
//
//    int testData;
//  };
// 
//  TEST_FIXTURE(SomeFixture, YourTestName)
//  {
//    int temp = testData;
//  }
//
// =================================
// Test Suites
// =================================
// 
// Tests can be grouped into suites, using the SUITE macro. A suite serves as a namespace for test names, so that the same test name can be used in two difference contexts.
//
//  SUITE(YourSuiteName)
//  {
//    TEST(YourTestName)
//    {
//    }
//
//    TEST(YourOtherTestName)
//    {
//    }
//  }
//
// This will place the tests into a C++ namespace called YourSuiteName, and make the suite name available to UnitTest++. 
// RunAllTests() can be called for a specific suite name, so you can use this to build named groups of tests to be run together.
// Note how members of the fixture are used as if they are a part of the test, since the macro-generated test class derives from the provided fixture class.
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// run all tests


class MyApp : public wxApp {
	wxSocketServer *m_pSocketServer;
public:
	virtual bool OnInit();
	virtual int OnRun();
	virtual int FilterEvent (wxEvent &event);
	void SocketReceiver(wxSocketEvent& event);
};


wxDECLARE_APP(MyApp);


int MyApp::OnRun() {
	return UnitTest::RunAllTests();
}


int MyApp::FilterEvent(wxEvent& event)
{
	wxPrintf(wxT("Event was called: %u\n"),event.GetId());
	return -1;
}

bool MyApp::OnInit()
{
	if(!wxApp::OnInit()) return false;
	
	// initialize socket server
	wxIPV4address addr;
	addr.Service(9567);
	
	m_pSocketServer = new wxSocketServer(addr, wxSOCKET_BLOCK);
	if (!m_pSocketServer) return false;
	
	if (m_pSocketServer->Error()) {
		
		wxPrintf(wxT("GameMsgSrv::Initialize() : Socket server initialization failed: 0x%08x"), m_pSocketServer->LastError());
		return false;	
	}
	
	Bind(wxEVT_SOCKET, &MyApp::SocketReceiver, this, wxID_ANY);
	
	// set socket server event handling
	m_pSocketServer->SetEventHandler(*this);
	m_pSocketServer->SetNotify(wxSOCKET_CONNECTION_FLAG);
	m_pSocketServer->Notify(true);
	
	return true;
	
}


void MyApp::SocketReceiver(wxSocketEvent& event)
{
	wxPrintf(wxT("Recieved event\n"));	
	switch(event.GetSocketEvent())
	{
		case wxSOCKET_CONNECTION:
   
			// Check if the server socket
			if (m_pSocketServer == (wxSocketServer*) event.GetSocket())
			{
				wxDword freeIndex = 0;
				wxSocketBase *  pSocket = m_pSocketServer->Accept(true);
				wxPrintf(wxT("Recieved connection\n"));	
				
			}
			break;
		default:
			wxPrintf(wxT("GameMsgSrv::SocketReceiver() : Unknown event: %d"), event.GetEventType());
			break;
	}
	
}


wxIMPLEMENT_APP_CONSOLE(MyApp);
