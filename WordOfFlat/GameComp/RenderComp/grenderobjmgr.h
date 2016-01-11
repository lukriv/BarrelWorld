#ifndef __GAME_RENDER_OBJECT_MANAGER_H__
#define __GAME_RENDER_OBJECT_MANAGER_H__

#include "gmanagerbase.h"
#include "grenderobject.h"

class GameRenderSystem;

class RenderObjectManager : public GameManagerBase<RenderObject>
{
	GameRenderSystem *m_pRenderSystem;
public:
	RenderObjectManager(GameRenderSystem *pRenderSystem);
	~RenderObjectManager();
	
	template<class T>
	GameErrorCode CreateComponent(wxDword compId, T*& pRenderObj)
	{
		GameErrorCode result = FWG_NO_ERROR;
		RefObjSmPtr<T> spRenderObj;
		FWG_RETURN_FAIL( GameNewChecked(spRenderObj.OutRef(), m_pRenderSystem) );
		
		FWG_RETURN_FAIL(InsertToMap(compId, spRenderObj));
		
		pRenderObj = spRenderObj.Detach();

		return result;
	}

};

#endif // __GAME_RENDER_OBJECT_MANAGER_H__
