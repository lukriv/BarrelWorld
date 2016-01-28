#ifndef __GAME_LOGIC_MOVEABLE_MANAGER_H__
#define __GAME_LOGIC_MOVEABLE_MANAGER_H__


#include <GameComp/gmanagerbase.h>
#include "gmoveable.h"
#include "glogicfreecam.h"
#include "glogicman.h"


class MoveableManager : public GameManagerBase<Moveable>
{
private:
	GameLogicSystem* m_pLogicSystem;
public:
	MoveableManager(GameLogicSystem* pLogicSystem, GameEntityManager *pEntityMgr);
	~MoveableManager();
	
	template <class T>
	GameErrorCode CreateComponent(wxDword compId, T *&pNewComponent )
	{
		RefObjSmPtr<T> spLogicComp;
		FWG_RETURN_FAIL(GameNewChecked(spLogicComp.OutRef()));
		
		FWG_RETURN_FAIL(InsertToMap(compId, spLogicComp));
		
		// todo: insert component to logic system
		
		return FWG_NO_ERROR;
	}

};

#endif // __GAME_LOGIC_MOVEABLE_MANAGER_H__
