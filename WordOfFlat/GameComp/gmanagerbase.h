#ifndef __GAME_MANAGER_BASE_H__
#define __GAME_MANAGER_BASE_H__

#include <GameSystem/glog.h>
#include <GameSystem/gmap.h>

class GameEntityManager;

template <typename Type>
class GameManagerBase
{
protected:
	
	using TTypeMapIterator = typename GameBasMap<wxDword, RefObjSmPtr<Type> >::Iterator;
	typedef GameBasMap<wxDword, RefObjSmPtr<Type> > TTypeMap;
	
protected:
	GameLoggerPtr m_spLogger;
	TTypeMap m_compMap;
	GameEntityManager *m_pEntityMgr;
	
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
		
		pComp->SetEntityManager(m_pEntityMgr);
		pComp->SetParentEntity(id);
		
		return FWG_NO_ERROR;
	}
	
public:
	GameManagerBase(GameLogger *pLogger, GameEntityManager *pEntityMgr) : m_spLogger(pLogger), m_pEntityMgr(pEntityMgr) {}
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
	
	Type* GetComponent(wxDword compId) 
	{	
		TTypeMapIterator iter = m_compMap.Find(compId);
		if(iter != m_compMap.End())
		{
			return iter->second;
		} else {
			return nullptr;
		}
	}


};

#endif // __GAME_MANAGER_BASE_H__
