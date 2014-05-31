#ifndef __GAME_LOG_H__
#define __GAME_LOG_H__

#include "gdefs.h"
#include <map>
#include <wx/log.h>
#include <wx/thread.h>
#include <wx/vector.h>
#include <wx/atomic.h>
#include <wx/ffile.h>

#include "gerror.h"
#include "refobject.h"
#include "refobjectsmptr.h"

#define FWGLOG_LEVEL_TRACE		5
#define FWGLOG_LEVEL_DEBUG		4
#define FWGLOG_LEVEL_INFO		3
#define FWGLOG_LEVEL_WARNING 	2
#define FWGLOG_LEVEL_ERROR 		1
#define FWGLOG_LEVEL_NOLOG 		0

#define FWGLOG_ENDVAL 			0

#ifndef FWGLOG_ACTUAL_LEVEL
	#define FWGLOG_ACTUAL_LEVEL FWGLOG_LEVEL_TRACE
#endif

#define FWGLOG_WRITE_LOG(message,logger,severity) \
	if(logger) (logger)->LogWrite(wxString(message),wxLogRecordInfo(__FILE__,__LINE__,__FUNCTION__,NULL),severity)

#define FWGLOG_WRITE_LOG_FORMAT(messageFormat,logger,severity,...) \
	if(logger) (logger)->LogWriteFormat(wxString(messageFormat),wxLogRecordInfo(__FILE__,__LINE__,__FUNCTION__,NULL),severity, __VA_ARGS__)

#if FWGLOG_ACTUAL_LEVEL >= FWGLOG_LEVEL_TRACE
	#define FWGLOG_TRACE(message,logger) FWGLOG_WRITE_LOG(message,logger,FWGLOG_LEVEL_TRACE)
	#define FWGLOG_TRACE_FORMAT(messageFormat,logger,...) FWGLOG_WRITE_LOG_FORMAT(messageFormat,logger,FWGLOG_LEVEL_TRACE,__VA_ARGS__)
#else 
	#define FWGLOG_TRACE(message,logger)
	#define FWGLOG_TRACE_FORMAT(messageFormat,logger,...)
#endif

#if FWGLOG_ACTUAL_LEVEL >= FWGLOG_LEVEL_DEBUG
	#define FWGLOG_DEBUG(message,logger) FWGLOG_WRITE_LOG(message,logger,FWGLOG_LEVEL_DEBUG)
	#define FWGLOG_DEBUG_FORMAT(messageFormat,logger,...) FWGLOG_WRITE_LOG_FORMAT(messageFormat,logger,FWGLOG_LEVEL_DEBUG,__VA_ARGS__)
#else 
	#define FWGLOG_DEBUG(message,logger)
	#define FWGLOG_DEBUG_FORMAT(messageFormat,logger,...)
#endif

#if FWGLOG_ACTUAL_LEVEL >= FWGLOG_LEVEL_INFO	
	#define FWGLOG_INFO(message,logger) FWGLOG_WRITE_LOG(message,logger,FWGLOG_LEVEL_INFO)
	#define FWGLOG_INFO_FORMAT(messageFormat,logger,...) FWGLOG_WRITE_LOG_FORMAT(messageFormat,logger,FWGLOG_LEVEL_INFO,__VA_ARGS__)
#else 
	#define FWGLOG_INFO(message,logger)
	#define FWGLOG_INFO_FORMAT(messageFormat,logger,...)
#endif

#if FWGLOG_ACTUAL_LEVEL >= FWGLOG_LEVEL_WARNING		
	#define FWGLOG_WARNING(message,logger) FWGLOG_WRITE_LOG(message,logger,FWGLOG_LEVEL_WARNING)
	#define FWGLOG_WARNING_FORMAT(messageFormat,logger,...) FWGLOG_WRITE_LOG_FORMAT(messageFormat,logger,FWGLOG_LEVEL_WARNING,__VA_ARGS__)
#else 
	#define FWGLOG_WARNING(message,logger)
	#define FWGLOG_WARNING_FORMAT(messageFormat,logger,...)
#endif

#if FWGLOG_ACTUAL_LEVEL >= FWGLOG_LEVEL_ERROR		
	#define FWGLOG_ERROR(message,logger) FWGLOG_WRITE_LOG(message,logger,FWGLOG_LEVEL_ERROR)
	#define FWGLOG_ERROR_FORMAT(messageFormat,logger,...) FWGLOG_WRITE_LOG_FORMAT(messageFormat,logger,FWGLOG_LEVEL_ERROR,__VA_ARGS__)
#else 
	#define FWGLOG_ERROR(message,logger)
	#define FWGLOG_ERROR_FORMAT(messageFormat,logger,...)
#endif


FWG_UNUSED(static const wxChar* FWGLOG_SEVERITY_STR_TRACE) = wxT("TRACE");
FWG_UNUSED(static const wxChar* FWGLOG_SEVERITY_STR_DEBUG) = wxT("DEBUG");
FWG_UNUSED(static const wxChar* FWGLOG_SEVERITY_STR_INFO) = wxT("INFO");
FWG_UNUSED(static const wxChar* FWGLOG_SEVERITY_STR_WARNING) = wxT("WARNING");
FWG_UNUSED(static const wxChar* FWGLOG_SEVERITY_STR_ERROR) = wxT("ERROR");


class GameLoggerCreator;


class GameLogger : public IRefObject {
private:
	bool m_isInitialized;
	wxAtomicInt m_refCount;
	wxFFile m_loggerFile;
	wxString m_loggerName;
	wxCriticalSection m_writeLock;
	
private:
	GameLogger() : m_isInitialized(false),
	    m_refCount(1),
		m_loggerFile(NULL){}
	virtual ~GameLogger();
	
	GameErrorCode Initialize(const wxChar* loggerName, const wxChar* loggerFileName);
	
	GameErrorCode Destroy();
	
	wxString LogTimeFormatter(const time_t time);
	wxString LogFormatter(const wxChar* Severity, const wxString& msg, const wxLogRecordInfo& info);
	wxString LogFormatterV(const wxChar* Severity, const wxString& msg, const wxLogRecordInfo& info, va_list args);
	
	void LogFileWrite(const wxString& msg);
	
	const wxChar* LogSeverity2String(wxDword logSeverity);
	
	friend class GameLoggerCreator;
public:
	const wxChar* GetLoggerName() const;
	
	void LogWrite(const wxString& msg, const wxLogRecordInfo& info, wxDword logSeverity);
	void LogWriteFormat(const wxString& msg, const wxLogRecordInfo& info, wxDword logSeverity, ...);
	
	void addRef();
	wxInt32 release();
};



class StrCmp {
public:
	bool operator() (const wxChar* a, const wxChar* b) {
		wxString str(a);
		return (str.Cmp(b) > 0);
	}
};

class GameLoggerCreator {
	typedef std::map<const wxChar*, GameLogger*, StrCmp> TLoggerMap;
	typedef std::pair<const wxChar*,GameLogger*> TLoggerMapItem;
private:
	static GameLoggerCreator* m_pLoggerCreator;
	TLoggerMap m_loggerMap;
	wxCriticalSection m_creatorLock;
private:
	GameLoggerCreator(){}
	
	static void DestroyLogger(GameLogger* pLogger);
	friend wxInt32 GameLogger::release();	
public:
	static GameErrorCode CreateLogger(GameLogger *&pLogger, const wxChar* loggerName = NULL, wxDword logSeverity = FWGLOG_ACTUAL_LEVEL);
	
	~GameLoggerCreator();
};

typedef RefObjSmPtr<GameLogger> GameLoggerPtr;





#endif //__GAME_LOG_H__