#ifndef __GAME_LOG_H__
#define __GAME_LOG_H__

#include <set>
#include <wx/log.h>
#include <wx/thread.h>
#include <wx/vector.h>
#include <wx/atomic.h>
#include "gerror.h"
#include "refobject.h"

#define FWGLOG_LEVEL_DETAIL		5
#define FWGLOG_LEVEL_DEBUG		4
#define FWGLOG_LEVEL_INFO		3
#define FWGLOG_LEVEL_WARNING 	2
#define FWGLOG_LEVEL_ERROR 		1
#define FWGLOG_LEVEL_NOLOG 		0

#define FWGLOG_ACTUAL_LEVEL FWGLOG_LEVEL_ERROR


#if FWGLOG_ACTUAL_LEVEL <= FWGLOG_LEVEL_DETAIL
	#define FWGLOG_DETAIL(message,logger) logger->LogDetail(wxString(message),wxLogRecordInfo(__FILE__,__LINE__,__FUNCTION__,NULL))
	#define FWGLOG_DETAIL_FORMAT(messageFormat,logger,...)
#else 
	#define FWGLOG_DETAIL(message,logger)
	#define FWGLOG_DETAIL_FORMAT(messageFormat,logger,...)
#endif

#if FWGLOG_ACTUAL_LEVEL <= FWGLOG_LEVEL_DEBUG
	#define FWGLOG_DEBUG(message,logger) logger->LogDebug(wxString(message),wxLogRecordInfo(__FILE__,__LINE__,__FUNCTION__,NULL))
	#define FWGLOG_DEBUG_FORMAT(messageFormat,logger,...)
#else 
	#define FWGLOG_DEBUG(message,logger)
	#define FWGLOG_DEBUG_FORMAT(messageFormat,logger,...)
#endif

#if FWGLOG_ACTUAL_LEVEL <= FWGLOG_LEVEL_INFO	
	#define FWGLOG_INFO(message,logger) logger->LogInfo(wxString(message),wxLogRecordInfo(__FILE__,__LINE__,__FUNCTION__,NULL))
	#define FWGLOG_INFO_FORMAT(messageFormat,logger,...)
#else 
	#define FWGLOG_INFO(message,logger)
	#define FWGLOG_INFO_FORMAT(messageFormat,logger,...)
#endif

#if FWGLOG_ACTUAL_LEVEL <= FWGLOG_LEVEL_WARNING		
	#define FWGLOG_WARNING(message,logger) logger->LogWarning(wxString(message),wxLogRecordInfo(__FILE__,__LINE__,__FUNCTION__,NULL))
	#define FWGLOG_WARNING_FORMAT(messageFormat,logger,...)
#else 
	#define FWGLOG_WARNING(message,logger)
	#define FWGLOG_WARNING_FORMAT(messageFormat,logger,...)
#endif

#if FWGLOG_ACTUAL_LEVEL <= FWGLOG_LEVEL_ERROR		
	#define FWGLOG_ERROR(message,logger) logger->LogError(wxString(message),wxLogRecordInfo(__FILE__,__LINE__,__FUNCTION__,NULL))
	#define FWGLOG_ERROR_FORMAT(messageFormat,logger,...)
#else 
	#define FWGLOG_ERROR(message,logger)
	#define FWGLOG_ERROR_FORMAT(messageFormat,logger,...)
#endif



class LogFormatterWithThread : public wxLogFormatter
{
	virtual wxString Format(wxLogLevel level,
	const wxString& msg,
	const wxLogRecordInfo& info) const
	{
	return wxString::Format("[%d] %s(%d) : %s",
	info.threadId, info.filename, info.line, msg);
	}
};


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

	friend class GameLoggerCreator;
public:
	const wxChar* GetLoggerName() const;
	
	void LogDetail(const wxString& msg, const wxLogRecordInfo& info);
	void LogDebug(const wxString& msg, const wxLogRecordInfo& info);
	void LogInfo(const wxString& msg, const wxLogRecordInfo& info);
	void LogWarning(const wxString& msg, const wxLogRecordInfo& info);
	void LogError(const wxString& msg, const wxLogRecordInfo& info);
	
	void LogDetailFormat(const wxString& formatStr, const wxLogRecordInfo& info, ...);
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
	
	~GameLoggerCreator(){}
};


extern GameLoggerCreator* g_pLoggerCreator;



#endif //__GAME_LOG_H__