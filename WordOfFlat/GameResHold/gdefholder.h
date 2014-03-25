#ifndef __GAME_DEFINITION_HOLDER_H__
#define __GAME_DEFINITION_HOLDER_H__

#include "../GameSystem/gdefs.h"
#include "../GameSystem/refobject.h"
#include "../GameSystem/gmap.h"
#include "gdeftables.h"


struct GameDefinitionHolder : public IRefObject {
	GameBasMap<wxString, RefObjSmPtr<NameDef> > m_meshDefs;
	GameBasMap<wxString, RefObjSmPtr<NameDef> > m_materialDefs;
	GameBasMap<wxString, RefObjSmPtr<AnimationDef> > m_animationDefs;
	GameBasMap<wxString, RefObjSmPtr<AnimatorDef> > m_animatorDefs;
	GameBasMap<wxString, RefObjSmPtr<EntityDef> > m_entityDefs;
};






#endif //__GAME_DEFINITION_HOLDER_H__