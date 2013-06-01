#include "glog.h"
#include <wx/log.h>
#include <wx/datetime.h>
#include <cstdio>
#include "glogconst.h"
#include <stdarg.h>

// initilize global variable
GameLoggerCreator* GameLoggerCreator::m_pLoggerCreator = NULL;


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

const wxChar* GameLogger::LogSeverity2String(wxDword logSeverity)
{
	switch(logSeverity)
	{
		case FWGLOG_LEVEL_TRACE:
			return FWGLOG_SEVERITY_STR_TRACE;
		case FWGLOG_LEVEL_DEBUG:
			return FWGLOG_SEVERITY_STR_DEBUG;
		case FWGLOG_LEVEL_INFO:
			return FWGLOG_SEVERITY_STR_INFO;
		case FWGLOG_LEVEL_WARNING:
			return FWGLOG_SEVERITY_STR_WARNING;
		case FWGLOG_LEVEL_ERROR:
			return FWGLOG_SEVERITY_STR_ERROR;
		default:
			return NULL;
	}
} 


void GameLogger::LogWrite(const wxString& msg, const wxLogRecordInfo& info, wxDword logSeverity)
{	
	wxString str = LogFormatter(LogSeverity2String(logSeverity), msg, info);
	wxLogMessage(str.wx_str());
	fflush(m_loggerFile);
}

void GameLogger::LogWriteFormat(const wxString& formatStr, const wxLogRecordInfo& info, wxDword logSeverity, ...)
{
	va_list args;
	va_start(args, FWGLOG_ENDVAL);
	wxString str = LogFormatterV(LogSeverity2String(logSeverity), formatStr, info, args);
	va_end(args);
	wxLogMessage(str.wx_str());
	fflush(m_loggerFile);
}

GameLogger::~GameLogger()
{
}




// ***************************************************************************
// ************************* GameLoggerCreator *******************************
// ***************************************************************************


GameErrorCode GameLoggerCreator::CreateLogger(GameLogger*& pLogger, const wxChar* loggerName, wxDword logSeverity)
{
	FILE* pLoggerFile = NULL;
	GameErrorCode result = FWG_NO_ERROR;
	const wxChar* loggerFileName = NULL;
	GameLogger* pLog = NULL;
	FWG_UNREFERENCED_PARAMETER(logSeverity);
	
	
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
	if (m_pLoggerCreator)
	{
		wxCriticalSectionLocker locker(m_pLoggerCreator->m_creatorLock);
		
		iter = m_pLoggerCreator->m_loggerSet.find(pLogger);
		if(m_pLoggerCreator->m_loggerSet.end() != iter)
		{
			m_pLoggerCreator->m_loggerSet.erase(iter);
		}
	}
	
	delete pLogger;
}
GameLoggerCreator::~GameLoggerCreator()
{
	m_pLoggerCreator = NULL;
}


