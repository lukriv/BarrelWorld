#ifndef __GAME_ENTITY_H__
#define __GAME_ENTITY_H__

struct GameEntity : public CompBase {
	wxString m_entityName;
	RefObjSmPtr<TranformCompDef> m_tranformation;
	RefObjSmPtr<RenderCompDef> m_renderDef;
	RefObjSmPtr<AnimatorDef> m_animatorDef;

	EntityDef() : CompDefBase() {}
};

#endif // __GAME_ENTITY_H__
