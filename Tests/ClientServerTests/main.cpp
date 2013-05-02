#include <UnitTest++/UnitTest++.h>
#include <wx/init.h>
#include <wx/wxcrtvararg.h>
#include <wx/app.h>
#include <wx/socket.h>
#include <wx/evtloop.h>
#include <wx/apptrait.h>
#include "../../WordOfFlat/GameSystem/gthread.h"

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


class EventThread : public GameThread {
	wxConsoleAppTraits* m_pTraits;    
	wxEventLoopBase* m_pEventLoop;
	//bool m_isInitialized;
		
protected:
	virtual void *Entry();
		
public:
	EventThread(): GameThread(wxTHREAD_JOINABLE),
										m_pTraits(NULL),
										m_pEventLoop(NULL) 
										{}

	~EventThread() {
		if (m_pEventLoop)
		{
			m_pEventLoop->IsRunning();
			m_pEventLoop->Exit();
			delete m_pEventLoop;
			m_pEventLoop = NULL;
		}
		
		if (m_pTraits)
		{
			delete m_pTraits;
			m_pTraits = NULL;
		}
	}
	
	GameErrorCode Initialize(wxConsoleAppTraits* pTraits);
	GameErrorCode StopRequest();

};

GameErrorCode EventThread::Initialize(wxConsoleAppTraits* pTraits)
{
	m_pTraits = pTraits;
	m_pEventLoop = m_pTraits->CreateEventLoop();
	return FWG_NO_ERROR;
}

GameErrorCode EventThread::StopRequest()
{
	if (!m_pEventLoop) return FWG_NO_ERROR;
	
	if(m_pEventLoop->IsRunning())
	{
		m_pEventLoop->Exit();
	}
	
	return FWG_NO_ERROR;
}

void* EventThread::Entry()
{
	if(m_pEventLoop == NULL) return NULL;
	return (void*) m_pEventLoop->Run();
}




int main(int argc, char **argv)
{
	wxInitializer intializer (argc, argv);
	if (!intializer.IsOk())
	{
		wxPrintf(wxT("Initializer failed"));
		return -1;
	}
	
	wxConsoleAppTraits traits;
	EventThread eventThr;
	
	eventThr.Initialize(&traits);
	eventThr.Run();
	
	
 	return UnitTest::RunAllTests();
}



