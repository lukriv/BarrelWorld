#include "glog.h"
#include <cstdio>
#include <wx/scopeguard.h>

static GameLogger* g_logger = NULL;

GameErrorCode GameLogger::CreateLogger(GameLogger *&pLogger)
{
	GameErrorCode result = FWG_NO_ERROR;
	
	//logger is initialized yet
	if (g_logger)
	{
		g_logger->addRef();
		pLogger = g_logger;
		return FWG_NO_ERROR;
	}
	
	g_logger = new (std::nothrow) GameLogger();
	
	if (!g_logger)
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	
	//initialize logger
	if(FWG_FAILED(result = g_logger->Initialize()))
	{
		//initializing failed - destroy logger
		return result;
	}
	
	return result;

}

void GameLogger::DestroyLogger()
{
	GameErrorCode result = FWG_NO_ERROR;
	
	if(g_logger)
	{
		result = g_logger->Destroy();
		delete g_logger;
		g_logger = NULL;
	}
	
}


wxLog* GameLogger::GetLogger()
{
	if(!g_logger)
	{
		return NULL;
	}
	
	return g_logger->m_logger;
}

//--------------non static members-----------------

GameErrorCode GameLogger::Initialize()
{
	wxScopeGuard guard = wxMakeGuard(DestroyLogger);
	// logger settings
	if (!(m_loggerFile = fopen("error.log", "w")))
	{
		printf("Failed to open logger file, aborting.");
        return -1;
	}
	
	m_logger = new (std::nothrow) wxLog();
	if (!m_logger)
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	wxLogStderr *stdLogger = new (std::nothrow) wxLogStderr(m_loggerFile);
	if(!stdLogger)
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	delete m_logger->SetActiveTarget(stdLogger);
	
	guard.Dismiss();
	
	m_isInitialized = true;
	
	return FWG_NO_ERROR;
		
}


GameErrorCode GameLogger::Destroy()
{
	if(m_logger)
	{
		delete m_logger->SetActiveTarget(NULL);
		fclose(m_loggerFile);
		delete m_logger;
		m_logger = NULL;
	}
	
	return FWG_NO_ERROR;
}

wxInt32 GameLogger::release()
{
	wxInt32 refCount = wxAtomicDec(m_refCount);
	if (refCount == 0)
	{
		DestroyLogger();
	}
	
	return refCount;
}

void GameLogger::addRef()
{
	wxAtomicInc(m_refCount);
}

GameLogger::~GameLogger()
{
	Destroy();
}


