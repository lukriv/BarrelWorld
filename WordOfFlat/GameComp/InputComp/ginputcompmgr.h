#ifndef __GAME_INPUT_COMPONENT_MANAGER_H__
#define __GAME_INPUT_COMPONENT_MANAGER_H__

#include "ginputchar.h"
#include "../gmanagerbase.h"


class InputComponentManager : public GameManagerBase<InputComponent>
{
private:
	GameInputSystem *m_pInputSystem;
public:
	InputComponentManager(GameInputSystem* pInputSystem, GameEntityManager *pEntityMgr);
	~InputComponentManager();
	
	template<class T>
	GameErrorCode CreateComponent(wxDword compId, T*& pInputComp)
	{
		GameErrorCode result = FWG_NO_ERROR;
		RefObjSmPtr<T> spInputComp;
		FWG_RETURN_FAIL( GameNewChecked(spInputComp.OutRef()) );
		
		if(FWG_FAILED(result = spInputComp->Initialize(m_pInputSystem)))
		{
			FWGLOG_ERROR_FORMAT(wxT("Input component initialize failed: 0x%08x"), GetLogger(), result, FWGLOG_ENDVAL);
			return result;
		}
		
		FWG_RETURN_FAIL(InsertToMap(compId, spInputComp));
		
		pInputComp = spInputComp.Detach();

		return result;
	}
};

#endif // __GAME_INPUT_COMPONENT_MANAGER_H__
