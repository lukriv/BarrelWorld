#ifndef __GAME_COMPONENT_MANAGER_BASE_IMPLEMENTATION_H__
#define __GAME_COMPONENT_MANAGER_BASE_IMPLEMENTATION_H__


#include "gcompintf.h"

class GameComponentMgrBase : public IGameComponentMgr {
	GameCompType m_compMgrType;
	wxAtomicInt m_refCount;
	
public:
	GameComponentMgrBase(GameCompType compMgrType) : m_compMgrType(compMgrType), m_refCount(1) {}
	virtual ~GameComponentMgrBase() {}

// IGameComponentMgr virtual methods
	/*!
	 * \brief Add component to component map
	 * 
	 * Component must be same type like component manager
	 * 
	 * \param pComp Inserted component (null will be ignored)
	 * \return 
	 */
	virtual GameErrorCode AddComponent(IGameComponent *pComp);
	virtual void RemoveComponent(const GameObjectID &objID);
	virtual IGameComponent* GetComponent(const GameObjectID &objID);

// IRefObject virtual methods
	virtual void addRef()
	{
		wxAtomicInc(m_refCount);
	}
	
	virtual wxInt32 release()
	{
		wxInt32 refCount = wxAtomicDec(m_refCount);
		if (refCount == 0)
		{
			delete this;
		}
		return refCount;
	}
};



#endif //__GAME_COMPONENT_MANAGER_BASE_IMPLEMENTATION_H__