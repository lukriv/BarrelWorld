#include "glog.h"
#include <wx/log.h>
#include <wx/datetime.h>
#include <cstdio>
#include "glogconst.h"
#include <stdarg.h>


GameLoggerCreator* g_pLoggerCreator = NULL;


class MyLogFormatter: public wxLogFormatter
{
	virtual wxString Format(wxLogLevel level, const wxString& msg, const wxLogRecordInfo& info) const
	{
		return wxString::Format("%s", msg);
	}
};

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
		
	wxLogStderr *pStdLogger = new (std::nothrow) wxLogStderr(pLoggerFile);
	if(!pStdLogger)
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	delete wxLog::SetActiveTarget(pStdLogger);
	
	wxLog::DisableTimestamp();
	
	m_loggerName.assign(loggerName);
	
	m_isInitialized = true;
	
	return result;
}

wxString GameLogger::LogTimeFormatter(const time_t time)
{
	wxDateTime tm(time);
	wxString normTime;
	normTime = tm.FormatISOCombined(' ');
	return wxString::Format(wxT("%s,%u"), normTime.wx_str(), tm.GetMillisecond());
	
}

wxString GameLogger::LogFormatter(const wxChar* Severity, const wxString& msg, const wxLogRecordInfo& info)
{
	return wxString::Format(wxT("[%s] %s :%s : %s(%d); thr(%d); %s(): %s"), 
					m_loggerName.wx_str(),
					Severity,
					LogTimeFormatter(info.timestamp).wx_str(),
					info.filename,
					info.line,
					info.threadId,
					info.func,
					msg.wx_str());
}
wxString GameLogger::LogFormatterV(const wxChar* Severity, const wxString& msg, const wxLogRecordInfo& info, va_list args)
{
	wxString intMsg;
	intMsg = wxString::FormatV(msg, args);
	return LogFormatter(Severity, intMsg, info);
}


void GameLogger::LogTrace(const wxString& msg, const wxLogRecordInfo& info)
{
	wxString str = LogFormatter(FWGLOG_SEVERITY_STR_TRACE, msg, info);
	wxLogMessage(str.wx_str());
}

void GameLogger::LogTraceFormat(const wxString& formatStr, const wxLogRecordInfo& info, ...)
{
	va_list args;
	va_start(args, formatStr.fn_str());
	wxString str = LogFormatterV(FWGLOG_SEVERITY_STR_TRACE, formatStr, info, args);
	va_end(args);
	wxLogMessage(str.wx_str());
}

void GameLogger::LogDebug(const wxString& msg, const wxLogRecordInfo& info)
{
	wxString str = LogFormatter(FWGLOG_SEVERITY_STR_DEBUG, msg, info);
	wxLogMessage(str.wx_str());
}

void GameLogger::LogDebugFormat(const wxString& formatStr, const wxLogRecordInfo& info, ...)
{
	va_list args;
	va_start(args, formatStr.fn_str());
	wxString str = LogFormatterV(FWGLOG_SEVERITY_STR_DEBUG, formatStr, info, args);
	va_end(args);
	wxLogMessage(str.wx_str());
}

void GameLogger::LogInfo(const wxString& msg, const wxLogRecordInfo& info)
{
	wxString str = LogFormatter(FWGLOG_SEVERITY_STR_INFO, msg, info);
	wxLogMessage(str.wx_str());
}

void GameLogger::LogInfoFormat(const wxString& formatStr, const wxLogRecordInfo& info, ...)
{
	va_list args;
	va_start(args, formatStr.fn_str());
	wxString str = LogFormatterV(FWGLOG_SEVERITY_STR_INFO, formatStr, info, args);
	va_end(args);
	wxLogMessage(str.wx_str());
}

void GameLogger::LogWarning(const wxString& msg, const wxLogRecordInfo& info)
{
	wxString str = LogFormatter(FWGLOG_SEVERITY_STR_WARNING, msg, info);
	wxLogMessage(str.wx_str());
}

void GameLogger::LogWarningFormat(const wxString& formatStr, const wxLogRecordInfo& info, ...)
{
	va_list args;
	va_start(args, formatStr.fn_str());
	wxString str = LogFormatterV(FWGLOG_SEVERITY_STR_WARNING, formatStr, info, args);
	va_end(args);
	wxLogMessage(str.wx_str());
}

void GameLogger::LogError(const wxString& msg, const wxLogRecordInfo& info)
{
	wxString str = LogFormatter(FWGLOG_SEVERITY_STR_ERROR, msg, info);
	wxLogMessage(str.wx_str());
}

void GameLogger::LogErrorFormat(const wxString& formatStr, const wxLogRecordInfo& info, ...)
{
	va_list args;
	va_start(args, formatStr.fn_str());
	wxString str = LogFormatterV(FWGLOG_SEVERITY_STR_ERROR, formatStr, info, args);
	va_end(args);
	wxLogMessage(str.wx_str());
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
	const wxChar* loggerFileName = NULL;
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
		
		loggerName = DefaultLogger.m_loggerName;
		loggerFileName = DefaultLogger.m_logFileName;
		
	} else {
		loggerName = DefaultLogger.m_loggerName;
		loggerFileName = DefaultLogger.m_logFileName;
	}
	
	pLog = new (std::nothrow) GameLogger();
	if(pLog)
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
	if (g_pLoggerCreator)
	{
		wxCriticalSectionLocker locker(g_pLoggerCreator->m_creatorLock);
		
		iter = g_pLoggerCreator->m_loggerSet.find(pLogger);
		if(g_pLoggerCreator->m_loggerSet.end() != iter)
		{
			g_pLoggerCreator->m_loggerSet.erase(iter);
		}
	}
	
	delete pLogger;
}
GameLoggerCreator::~GameLoggerCreator()
{
	g_pLoggerCreator = NULL;
}
