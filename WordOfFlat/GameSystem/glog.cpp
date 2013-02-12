#include "glog.h"
#include <cstdio>
#include "glogconst.h"


GameLoggerCreator* g_pLoggerCreator = NULL;

// ***************************************************************************
// **************************** GameLogger ***********************************
// ***************************************************************************

wxInt32 GameLogger::release()
{
	wxInt32 refCount = wxAtomicDec(m_refCount);
	if (refCount == 0)
	{
		GameLoggerCreator::DestroyLogger(this);
	}
	
	return refCount;
}

void GameLogger::addRef()
{
	wxAtomicInc(m_refCount);
}

GameErrorCode GameLogger::Destroy()
{
}

const wxChar* GameLogger::GetLoggerName() const
{
}

GameErrorCode GameLogger::Initialize(const wxChar* loggerName, const wxChar* loggerFileName)
{
	FILE * pLoggerFile = NULL;
	GameErrorCode result = FWG_NO_ERROR;
	wxCharBuffer charBuff = wxString(loggerFileName).ToAscii();
	
	if(m_isInitialized)
	{
		return result;
	}
	
	if (!(pLoggerFile = fopen(charBuff.data(), "w")))
	{
		printf("Failed to open logger file, aborting.");
        return -1;
	}
		
	wxLogStderr *stdLogger = new (std::nothrow) wxLogStderr(pLoggerFile);
	if(!stdLogger)
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	delete wxLog::SetActiveTarget(stdLogger);
	
	m_isInitialized = true;
	
	return result;
}

void GameLogger::LogDebug(const wxString& msg, const wxLogRecordInfo& info)
{
	wxLogMessage(wxString::Format("[%s] %s(%d) : %s", info.threadId, info.filename, info.line, msg));
}

void GameLogger::LogDebugFormat(const wxString& formatStr, const wxLogRecordInfo& info, ...)
{

}

void GameLogger::LogDetail(const wxString& msg, const wxLogRecordInfo& info)
{
}

void GameLogger::LogDetailFormat(const wxString& formatStr, const wxLogRecordInfo& info, ...)
{
}

void GameLogger::LogError(const wxString& msg, const wxLogRecordInfo& info)
{
}

void GameLogger::LogErrorFormat(const wxString& formatStr, const wxLogRecordInfo& info, ...)
{
}

void GameLogger::LogInfo(const wxString& msg, const wxLogRecordInfo& info)
{
}

void GameLogger::LogInfoFormat(const wxString& formatStr, const wxLogRecordInfo& info, ...)
{
}

void GameLogger::LogWarning(const wxString& msg, const wxLogRecordInfo& info)
{
}

void GameLogger::LogWarningFormat(const wxString& formatStr, const wxLogRecordInfo& info, ...)
{
}

GameLogger::~GameLogger()
{
}



// ***************************************************************************
// ************************* GameLoggerCreator *******************************
// ***************************************************************************


GameErrorCode GameLoggerCreator::CreateLogger(GameLogger*& pLogger, const wxChar* loggerName)
{
	FILE* pLoggerFile = NULL;
	GameErrorCode result = FWG_NO_ERROR;
	wxChar* loggerFileName = NULL;
	GameLogger* pLog = NULL;
	
	if (loggerName)
	{
		for(wxDword i = 0; i < GameLoggerTableSize; ++i)
		{
			if(wxString(GameLoggerTable[i].m_loggerName).compare(loggerName))
			{
				loggerFileName = GameLoggerTable[i].m_logFileName;
				break;
			}
		}
	} else {
		loggerFileName = DefaultLogger.m_logFileName;
	}
	
	pLog = new (std::nothrow) GameLogger();
	if(!pLog)
	{
		if(FWG_FAILED(result = pLog->Initialize(loggerName, loggerFileName)))
		{
			delete pLog;
			return result;
		}
	} else {
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	pLogger = pLog;
	
	return result;
}

void GameLoggerCreator::DestroyLogger(GameLogger* pLogger)
{
	LoggerSetType::iterator iter;
	wxCriticalSectionLocker locker(g_pLoggerCreator->m_creatorLock);
	
	iter = g_pLoggerCreator->m_loggerSet.find(pLogger);
	if(g_pLoggerCreator->m_loggerSet.end() != iter)
	{
		g_pLoggerCreator->m_loggerSet.erase(iter);
		delete pLogger;
	}
}
