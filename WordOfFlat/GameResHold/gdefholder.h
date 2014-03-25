#ifndef __GAME_DEFINITION_HOLDER_H__
#define __GAME_DEFINITION_HOLDER_H__

#include "../GameSystem/gdefs.h"
#include "../GameSystem/refobject.h"
#include "../GameSystem/simpleMap.h"
#include "gdeftables.h"


struct GameDefinitionHolder : public IRefObject {
	SimpleMap<wxString, RefObjSmPtr<NameDef> > m_meshDefs;
	SimpleMap<wxString, RefObjSmPtr<NameDef> > m_materialDefs;
	SimpleMap<wxString, RefObjSmPtr<AnimationDef> > m_animationDefs;
	SimpleMap<wxString, RefObjSmPtr<AnimatorDef> > m_animatorDefs;
	SimpleMap<wxString, RefObjSmPtr<EntityDef> > m_entityDefs;
};






#endif //__GAME_DEFINITION_HOLDER_H__