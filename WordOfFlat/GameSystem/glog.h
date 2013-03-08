#ifndef __GAME_LOG_H__
#define __GAME_LOG_H__

#include <set>
#include <wx/log.h>
#include <wx/thread.h>
#include <wx/vector.h>
#include <wx/atomic.h>
#include "gerror.h"
#include "refobject.h"
#include "refobjectsmptr.h"

#define FWGLOG_LEVEL_TRACE		5
#define FWGLOG_LEVEL_DEBUG		4
#define FWGLOG_LEVEL_INFO		3
#define FWGLOG_LEVEL_WARNING 	2
#define FWGLOG_LEVEL_ERROR 		1
#define FWGLOG_LEVEL_NOLOG 		0

#ifndef FWGLOG_ACTUAL_LEVEL
	#define FWGLOG_ACTUAL_LEVEL FWGLOG_LEVEL_TRACE
#endif


#if FWGLOG_ACTUAL_LEVEL >= FWGLOG_LEVEL_TRACE
	#define FWGLOG_TRACE(message,logger) if(logger) logger->LogTrace(wxString(message),wxLogRecordInfo(__FILE__,__LINE__,__FUNCTION__,NULL))
	#define FWGLOG_TRACE_FORMAT(messageFormat,logger,...) if(logger) logger->LogTraceFormat(wxString(messageFormat),wxLogRecordInfo(__FILE__,__LINE__,__FUNCTION__,NULL), __VA_ARGS__)
#else 
	#define FWGLOG_TRACE(message,logger)
	#define FWGLOG_TRACE_FORMAT(messageFormat,logger,...)
#endif

#if FWGLOG_ACTUAL_LEVEL >= FWGLOG_LEVEL_DEBUG
	#define FWGLOG_DEBUG(message,logger) if(logger) logger->LogDebug(wxString(message),wxLogRecordInfo(__FILE__,__LINE__,__FUNCTION__,NULL))
	#define FWGLOG_DEBUG_FORMAT(messageFormat,logger,...) if(logger) logger->LogDebugFormat(wxString(messageFormat),wxLogRecordInfo(__FILE__,__LINE__,__FUNCTION__,NULL), __VA_ARGS__)
#else 
	#define FWGLOG_DEBUG(message,logger)
	#define FWGLOG_DEBUG_FORMAT(messageFormat,logger,...)
#endif

#if FWGLOG_ACTUAL_LEVEL >= FWGLOG_LEVEL_INFO	
	#define FWGLOG_INFO(message,logger) if(logger) logger->LogInfo(wxString(message),wxLogRecordInfo(__FILE__,__LINE__,__FUNCTION__,NULL))
	#define FWGLOG_INFO_FORMAT(messageFormat,logger,...) if(logger) logger->LogInfoFormat(wxString(messageFormat),wxLogRecordInfo(__FILE__,__LINE__,__FUNCTION__,NULL), __VA_ARGS__)
#else 
	#define FWGLOG_INFO(message,logger)
	#define FWGLOG_INFO_FORMAT(messageFormat,logger,...)
#endif

#if FWGLOG_ACTUAL_LEVEL >= FWGLOG_LEVEL_WARNING		
	#define FWGLOG_WARNING(message,logger) if(logger) logger->LogWarning(wxString(message),wxLogRecordInfo(__FILE__,__LINE__,__FUNCTION__,NULL))
	#define FWGLOG_WARNING_FORMAT(messageFormat,logger,...) if(logger) logger->LogWarningFormat(wxString(messageFormat),wxLogRecordInfo(__FILE__,__LINE__,__FUNCTION__,NULL), __VA_ARGS__)
#else 
	#define FWGLOG_WARNING(message,logger)
	#define FWGLOG_WARNING_FORMAT(messageFormat,logger,...)
#endif

#if FWGLOG_ACTUAL_LEVEL >= FWGLOG_LEVEL_ERROR		
	#define FWGLOG_ERROR(message,logger) if(logger) logger->LogError(wxString(message),wxLogRecordInfo(__FILE__,__LINE__,__FUNCTION__,NULL))
	#define FWGLOG_ERROR_FORMAT(messageFormat,logger,...) if(logger) logger->LogErrorFormat(wxString(messageFormat),wxLogRecordInfo(__FILE__,__LINE__,__FUNCTION__,NULL), __VA_ARGS__)
#else 
	#define FWGLOG_ERROR(message,logger)
	#define FWGLOG_ERROR_FORMAT(messageFormat,logger,...)
#endif


static const wxChar* FWGLOG_SEVERITY_STR_TRACE = wxT("TRACE");
static const wxChar* FWGLOG_SEVERITY_STR_DEBUG = wxT("DEBUG");
static const wxChar* FWGLOG_SEVERITY_STR_INFO = wxT("INFO");
static const wxChar* FWGLOG_SEVERITY_STR_WARNING = wxT("WARNING");
static const wxChar* FWGLOG_SEVERITY_STR_ERROR = wxT("ERROR");


class GameLoggerCreator;


class GameLogger : public IRefObject {
private:
	bool m_isInitialized;
	wxAtomicInt m_refCount;
	FILE * m_loggerFile;
	wxString m_loggerName;
	
private:
	GameLogger() : m_isInitialized(false),
	    m_refCount(1),
		m_loggerFile(NULL){}
	~GameLogger();
	
	GameErrorCode Initialize(const wxChar* loggerName, const wxChar* loggerFileName);
	
	GameErrorCode Destroy();
	
	wxString LogTimeFormatter(const time_t time);
	wxString LogFormatter(const wxChar* Severity, const wxString& msg, const wxLogRecordInfo& info);
	wxString LogFormatterV(const wxChar* Severity, const wxString& msg, const wxLogRecordInfo& info, va_list args);
	
	friend class GameLoggerCreator;
public:
	const wxChar* GetLoggerName() const;
	
	void LogTrace(const wxString& msg, const wxLogRecordInfo& info);
	void LogDebug(const wxString& msg, const wxLogRecordInfo& info);
	void LogInfo(const wxString& msg, const wxLogRecordInfo& info);
	void LogWarning(const wxString& msg, const wxLogRecordInfo& info);
	void LogError(const wxString& msg, const wxLogRecordInfo& info);
	
	void LogTraceFormat(const wxString& formatStr, const wxLogRecordInfo& info, ...);
	void LogDebugFormat(const wxString& formatStr, const wxLogRecordInfo& info, ...);
	void LogInfoFormat(const wxString& formatStr, const wxLogRecordInfo& info, ...);
	void LogWarningFormat(const wxString& formatStr, const wxLogRecordInfo& info, ...);
	void LogErrorFormat(const wxString& formatStr, const wxLogRecordInfo& info, ...);
	
	void addRef();
	wxInt32 release();
};

class GameLoggerCreator {
	typedef std::set<GameLogger*> LoggerSetType;
private:
	LoggerSetType m_loggerSet;
	wxCriticalSection m_creatorLock;
private:
	GameLoggerCreator(){}
	
	static void DestroyLogger(GameLogger* pLogger);
	friend wxInt32 GameLogger::release();	
public:
	static GameErrorCode CreateLogger(GameLogger *&pLogger, const wxChar* loggerName = NULL);
	
	~GameLoggerCreator();
};

typedef RefObjSmPtr<GameLogger> GameLoggerPtr;

extern GameLoggerCreator* g_pLoggerCreator;



#endif //__GAME_LOG_H__