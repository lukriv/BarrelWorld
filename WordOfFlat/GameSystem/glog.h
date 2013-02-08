#ifndef __GAME_LOG_H__
#define __GAME_LOG_H__


#include <wx/log.h>
#include <wx/atomic.h>
#include "gerror.h"
#include "refobject.h"

class GameLogger : public IRefObject {
private:
	wxAtomicInt m_refCount;
	FILE * m_loggerFile;
	wxLog* m_logger;
	bool m_isInitialized;
private:
	GameLogger() : m_refCount(1),
		m_loggerFile(NULL),
		m_logger(NULL),
		m_isInitialized(false) {}
	~GameLogger();
	
	GameErrorCode Initialize();
	
	GameErrorCode Destroy();

	static void DestroyLogger();
	
public:
	static GameErrorCode CreateLogger(GameLogger *&pLogger);
	
	wxLog* GetLogger();
	
	void addRef();
	wxInt32 release();
};


#endif //__GAME_LOG_H__