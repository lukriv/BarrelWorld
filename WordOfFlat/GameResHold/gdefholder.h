#ifndef __GAME_DEFINITION_HOLDER_H__
#define __GAME_DEFINITION_HOLDER_H__

#include "../GameSystem/gdefs.h"
#include "../GameSystem/refobject.h"
#include "../GameSystem/compContainer.h"
#include "gdeftables.h"


struct GameDefinitionHolder : public IRefObject {
	CompDefContainer<wxDword,RefObjSmPtr<NameDef>> m_meshDefs;
	CompDefContainer<wxDword,RefObjSmPtr<NameDef>> m_materialDefs;
	CompDefContainer<wxDword,RefObjSmPtr<AnimationDef>> m_animationDefs;
	CompDefContainer<wxDword,RefObjSmPtr<AnimatorDef>> m_animatorDefs;
	CompDefContainer<wxDword,RefObjSmPtr<EntityDef>> m_entityDefs;
};






#endif //__GAME_DEFINITION_HOLDER_H__