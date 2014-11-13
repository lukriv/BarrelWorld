#ifndef __GAME_DEFINITION_HOLDER_H__
#define __GAME_DEFINITION_HOLDER_H__

#include <GameSystem/gdefs.h>
#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>
#include <GameSystem/gmap.h>
#include "gdeftables.h"


typedef GameBasMap<wxString, RefObjSmPtr<NameDef> > TNameDefMap;
typedef GameBasMap<wxString, RefObjSmPtr<RenderEntityDef> > TRenderEntityDefMap;
typedef GameBasMap<wxString, RefObjSmPtr<RenderDef> > TRenderDefMap;
typedef GameBasMap<wxString, RefObjSmPtr<CameraDef> > TCameraDefMap;
typedef GameBasMap<wxString, RefObjSmPtr<AnimationDef> > TAnimationDefMap;
typedef GameBasMap<wxString, RefObjSmPtr<AnimatorDef> > TAnimatorDefMap;
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
	TAnimationDefMap m_animationDefs;
	TAnimatorDefMap	m_animatorDefs;
	TInputDefMap m_inputDefs;
	TLogicDefMap m_logicDefs;
	TEntityDefMap m_entityDefs;
	
};






#endif //__GAME_DEFINITION_HOLDER_H__
