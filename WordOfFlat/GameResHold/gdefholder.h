#ifndef __GAME_DEFINITION_HOLDER_H__
#define __GAME_DEFINITION_HOLDER_H__

#include <GameSystem/gdefs.h>
#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>
#include <GameSystem/gmap.h>
#include "gdeftables.h"


typedef GameBasMap<wxString, RefObjSmPtr<RenderDef> > TRenderDefMap;
typedef GameBasMap<wxString, RefObjSmPtr<InputDef> > TInputDefMap;
typedef GameBasMap<wxString, RefObjSmPtr<LogicDef> > TLogicDefMap;
typedef GameBasMap<wxString, RefObjSmPtr<EntityDef> > TEntityDefMap;

class GameDefinitionHolder : public RefObjectImpl<IRefObject> {
public:
	GameDefinitionHolder() {}
	
	template <typename T>
	GameErrorCode InsertDef(const wxString& name, RefObjSmPtr<T> &spDef, GameBasMap<wxString, RefObjSmPtr<T> > &basMap)
	{
		typename GameBasMap<wxString, RefObjSmPtr<T> >::Iterator iter;
		FWG_RETURN_FAIL(basMap.Insert( name, spDef, iter ));
		spDef->SetName(name);
		
		return FWG_NO_ERROR;
	}
	
	TNameDefMap m_meshDefs;
	TNameDefMap m_materialDefs;
	TRenderEntityDefMap m_renderEntityDefs;
	TCameraDefMap m_cameraDefs;
	TRenderDefMap m_renderDefs;
	TInputDefMap m_inputDefs;
	TLogicDefMap m_logicDefs;
	TEntityDefMap m_entityDefs;
	
	RefObjSmPtr< TerrainDef > m_spTerrain;
	
};






#endif //__GAME_DEFINITION_HOLDER_H__
