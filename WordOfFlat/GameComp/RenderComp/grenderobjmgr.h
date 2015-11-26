#ifndef __GAME_RENDER_OBJECT_MANAGER_H__
#define __GAME_RENDER_OBJECT_MANAGER_H__

#include "gmanagerbase.h"

class RenderObjectManager : public GameManagerBase<RenderObject>
{
public:
	RenderObjectManager();
	~RenderObjectManager();
	
	template<class T>
	GameErrorCode CreateComponent(wxDword compId, T*& pRenderObj)
	{
		GameErrorCode result = FWG_NO_ERROR;
		RefObjSmPtr<T> spRenderObj;
		FWG_RETURN_FAIL( GameNewChecked(spRenderObj.OutRef()) );
		
		if(FWG_FAILED(result = spRenderObj->Initialize(m_pInputSystem)))
		{
			FWGLOG_ERROR_FORMAT(wxT("Input component initialize failed: 0x%08x"), GetLogger(), result, FWGLOG_ENDVAL);
			return result;
		}
		
		FWG_RETURN_FAIL(InsertToMap(compId, spRenderObj));
		
		pInputComp = spRenderObj.Detach();

		return result;
	}

};

#endif // __GAME_RENDER_OBJECT_MANAGER_H__
