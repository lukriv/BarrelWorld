#ifndef __GAME_DEFINITION_HOLDER_H__
#define __GAME_DEFINITION_HOLDER_H__

#include "../GameSystem/gdefs.h"
#include "../GameSystem/refobject.h"
#include "../GameSystem/gmap.h"
#include "gdeftables.h"


typedef GameBasMap<wxString, RefObjSmPtr<NameDef> > TNameDefMap;
typedef GameBasMap<wxString, RefObjSmPtr<AnimationDef> > TAnimationDefMap;
typedef GameBasMap<wxString, RefObjSmPtr<AnimatorDef> > TAnimatorDefMap;
typedef GameBasMap<wxString, RefObjSmPtr<EntityDef> > TEntityDefMap;

class GameDefinitionHolder : public IRefObject {
	wxAtomicInt m_refCount;
public:
	GameDefinitionHolder() : m_refCount(1) {}
	
	TNameDefMap m_meshDefs;
	TNameDefMap m_materialDefs;
	TAnimationDefMap m_animationDefs;
	TAnimatorDefMap	m_animatorDefs;
	TEntityDefMap m_entityDefs;
	

	virtual void addRef();
	virtual wxInt32 release();
};






#endif //__GAME_DEFINITION_HOLDER_H__
