#include "glog.h"
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
	return m_loggerName.GetData();
}

GameErrorCode GameLogger::Initialize(const wxChar* loggerName, const wxChar* loggerFileName)
{
	GameErrorCode result = FWG_NO_ERROR;
	
	if(m_isInitialized)
	{
		return result;
	}
	
	if (!(m_loggerFile.Open(wxString(loggerFileName), wxString("a"))))
	{
		printf("Failed to open logger file, aborting.");
        return -1;
	}
		
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
	LogFileWrite(str);
}

void GameLogger::LogWriteFormat(const wxString& formatStr, const wxLogRecordInfo& info, wxDword logSeverity, ...)
{
	va_list args;
	va_start(args, FWGLOG_ENDVAL);
	wxString str = LogFormatterV(LogSeverity2String(logSeverity), formatStr, info, args);
	va_end(args);
	LogFileWrite(str);
}

GameLogger::~GameLogger()
{
	if(m_loggerFile.IsOpened())
	{
		m_loggerFile.Flush();
		m_loggerFile.Close();
	}
}

void GameLogger::LogFileWrite(const wxString& msg)
{
	wxCriticalSectionLocker lock(m_writeLock);
	m_loggerFile.Write(msg);
	m_loggerFile.Write(wxT("\n")); // add new line
	m_loggerFile.Flush();
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
	TLoggerMap::iterator iter;
	
	if (m_pLoggerCreator == NULL)
	{
		m_pLoggerCreator = new (std::nothrow) GameLoggerCreator();
		if (m_pLoggerCreator == NULL) return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	if (loggerName)
	{
		wxDword i;
		for(i = 0; i < GameLoggerTableSize; ++i)
		{
			if(wxString(GameLoggerTable[i].m_loggerName).Cmp(loggerName) == 0)
			{
				loggerFileName = GameLoggerTable[i].m_logFileName;
				break;
			}
		}
		// if logger name was not found set name to default
		if (i == GameLoggerTableSize) {
			loggerName = DefaultLogger.m_loggerName;
			loggerFileName = DefaultLogger.m_logFileName;
		}
		
	} else {
		loggerName = DefaultLogger.m_loggerName;
		loggerFileName = DefaultLogger.m_logFileName;
	}
	
	iter = m_pLoggerCreator->m_loggerMap.find(loggerName);
	if (iter == m_pLoggerCreator->m_loggerMap.end()) {
	
		pLog = new (std::nothrow) GameLogger();
		if(pLog)
		{
			if(FWG_FAILED(result = pLog->Initialize(loggerName, loggerFileName)))
			{
				delete pLog;
				return result;
			}
			
			//insert into map
			m_pLoggerCreator->m_loggerMap.insert(TLoggerMapItem(loggerName, pLog));
			
		} else {
			return FWG_E_MEMORY_ALLOCATION_ERROR;
		}
	} else {
		pLog = iter->second;
		pLog->addRef();
	}
	
	pLogger = pLog;
	
	return result;
}

void GameLoggerCreator::DestroyLogger(GameLogger* pLogger)
{
	TLoggerMap::iterator iter;
	if (m_pLoggerCreator)
	{
		wxCriticalSectionLocker locker(m_pLoggerCreator->m_creatorLock);
		
		iter = m_pLoggerCreator->m_loggerMap.find(pLogger->GetLoggerName());
		if(m_pLoggerCreator->m_loggerMap.end() != iter)
		{
			m_pLoggerCreator->m_loggerMap.erase(iter);
		}
	}
	
	delete pLogger;
}
GameLoggerCreator::~GameLoggerCreator()
{
	m_pLoggerCreator = NULL;
	
	TLoggerMap::iterator iter;
	for (iter = m_loggerMap.begin(); iter != m_loggerMap.end(); iter++)
	{
		delete iter->second;
	}
	
	m_loggerMap.clear();
}


