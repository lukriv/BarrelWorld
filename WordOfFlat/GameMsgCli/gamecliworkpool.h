#ifndef __GAME_CLIENT_WORKER_POOL_H__
#define __GAME_CLIENT_WORKER_POOL_H__


#include <map>
#include <wx/stream.h>
#include "../GameSystem/gdefs.h"
#include "../GameSystem/gerror.h"
#include "../GameSystem/gthread.h"
#include "../GameSystem/gmsgqueue.h"
#include "../GameSystem/glog.h"
#include "../GameComm/GameMessage.h"
#include "gamemsgcli.h"


class GameCliClbkWorkerPool {
protected:
	typedef std::set<IGameMsgCallback*> ClbkVecType;
	typedef std::pair<GameMessageType,ClbkVecType> ClbkMapPairType;
	typedef std::map<GameMessageType,ClbkVecType> ClbkMapType;
	typedef GameMessageQueue<IGameMessage*> MsgQueueType;

	class CallbackThread : public GameThread {
		wxAtomicInt m_isStopRequest;
		GameCliClbkWorkerPool* m_pOwner;
		bool m_isInitialized;
		
	protected:
		virtual void *Entry();
		
	public:
		CallbackThread(GameCliClbkWorkerPool* pOwner): GameThread(wxTHREAD_JOINABLE),
										m_isStopRequest(0),
										m_pOwner(pOwner),
										m_isInitialized(false) {}
		GameErrorCode Initialize();
		GameErrorCode StopRequest();

	};
	
	typedef wxVector<CallbackThread*> CallbackWorkerPool;
	
private:
	bool m_isInitialized;
protected:
	GameLoggerPtr m_pLogger;
	wxCriticalSection m_clbkLock;
	ClbkMapType m_callbackMap;
	MsgQueueType m_msgQueue;
	wxDword m_threadsNr;
	CallbackWorkerPool m_clbkThreads;
protected:
	GameErrorCode MessageProcess(IGameMessage* pMsg);

public:
	GameCliClbkWorkerPool() : m_isInitialized(false), m_threadsNr(0) {}
	virtual ~GameCliClbkWorkerPool();
	
	GameErrorCode Initialize(wxDword threadsNr, GameLogger *pLogger);
	
	/*!
	 * \brief Destroy all created members
	 */
	void DestroyThreads();
	
	inline GameLoggerPtr GetLogger() { return m_pLogger; }

};

#endif //__GAME_CLIENT_WORKER_POOL_H__