


// GameCliClbkWorkerPool inline methods

GameErrorCode GameCliClbkWorkerPool::RegisterCallbackInternal(GameMessageType msgType, IGameMsgCallback* pClbk)
{ 
	GameErrorCode result = FWG_NO_ERROR;

	ClbkMapType::iterator mapIter;
	ClbkVecType::iterator vecIter;
	
	if(!m_isInitialized) {
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}

	wxCriticalSectionLocker locker(m_clbkLock);
	
	mapIter = m_callbackMap.find(msgType);
	if (mapIter != m_callbackMap.end())
	{
		mapIter->second.insert(pClbk);
	} else {
		ClbkVecType newVec;
		newVec.insert(pClbk);
		m_callbackMap.insert(ClbkMapPairType(msgType,newVec));
	}
	
	return result;
}


GameErrorCode GameCliClbkWorkerPool::UnregisterCallbackInternal(GameMessageType msgType, IGameMsgCallback* pClbk)
{
	GameErrorCode result = FWG_NO_ERROR;

	ClbkMapType::iterator mapIter;
	ClbkVecType::iterator vecIter;
	
	if(!m_isInitialized) {
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}

	wxCriticalSectionLocker locker(m_clbkLock);
	
	mapIter = m_callbackMap.find(msgType);
	if(mapIter != m_callbackMap.end())
	{
		vecIter = mapIter->second.find(pClbk);
		if (vecIter != mapIter->second.end())
		{
			mapIter->second.erase(vecIter);
			if (mapIter->second.empty())
			{
				m_callbackMap.erase(mapIter);
			}
		}
	}
	
	return result;
}