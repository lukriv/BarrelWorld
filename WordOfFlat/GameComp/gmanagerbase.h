#ifndef __GAME_MANAGER_BASE_H__
#define __GAME_MANAGER_BASE_H__

#include <GameSystem/glog.h>
#include <GameSystem/gmap.h>


template <typename Type>
class GameManagerBase
{
protected:
	
	using TTypeMapIterator = typename GameBasMap<wxDword, RefObjSmPtr<Type> >::Iterator;
	typedef GameBasMap<wxDword, RefObjSmPtr<Type> > TTypeMap;
	
protected:
	GameLoggerPtr m_spLogger;
	TTypeMap m_compMap;
	
protected:
	inline GameErrorCode InsertToMap(wxDword id, Type* pComp)
	{
		
		TTypeMapIterator iter = m_compMap.Find(id);
	
		if(iter == m_compMap.End())
		{
			FWG_RETURN_FAIL(m_compMap.Insert(id, pComp));
		} else {
			iter->second = pComp;
		}
		
		return FWG_NO_ERROR;
	}
	
public:
	GameManagerBase(GameLogger *pLogger) : m_spLogger(pLogger) {}
	~GameManagerBase() 
	{ 
		m_compMap.Clear();
	}
	
	inline GameLogger * GetLogger() { return m_spLogger; }
	
	GameErrorCode AddComponent(wxDword compId, Type* pNewComponent)
	{
		FWG_RETURN_FAIL(InsertToMap(compId, pNewComponent));
		return FWG_NO_ERROR;
	}
	void DestroyComponent(wxDword compId) { m_compMap.Remove(compId); }
	void DestroyAllComponents() { m_compMap.Clear(); }
	
	Type* GetEntity(wxDword compId) { m_compMap.FindValue(compId); }


};

#endif // __GAME_MANAGER_BASE_H__
