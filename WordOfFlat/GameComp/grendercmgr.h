#ifndef __GAME_RENDER_COMPONENT_MANAGER_H__
#define __GAME_RENDER_COMPONENT_MANAGER_H__

#include "../GameSystem/gdefs.h"
#include "gcompmgrimpl.h"
#include "gameobjdef.h"


struct RenderCompDef {
	GameObjectID m_textureRef; //!< List of textures - can be empty (dep. on type)
	GameObjectID m_geometryRef; //!< List of geometry objects - can be empty (dep. on type)
	GameObjectID m_physRef; //!< List of Joints or Bodys - depend on type
	b2Transform m_tranformation;
	bool m_textureRepeat; //!< texture repetition
	
	RenderCompDef() : m_textureRef(GAME_OBJECT_ID_INVALID)
			, m_geometryRef(GAME_OBJECT_ID_INVALID)
			, m_physRef(GAME_OBJECT_ID_INVALID)
			, m_textureRepeat(false) {}
};


class GameRenderComponentMgr : public GameComponentMgrBase {
	
// IGameComponent interface
public:
	virtual GameCompMgrType GetType() {return GAME_COMP_RENDER;}
	GameErrorCode CreateBackgroundObject();
	GameErrorCode CreateLandscapeObject(const RenderCompDef &entityDef, GameObjectId &compId);
	GameErrorCode CreateGenericComponent(const RenderCompDef &entityDef, GameObjectId &compId);
	
	GameErrorCode DeleteBackGroundObject();
	GameErrorCode DeleteLandscapeObject(GameObjectId compId);
	GameErrorCode DeleteGenericObject(GameObjectId compId);
	
	GameErrorCode RenderBackground();
	GameErrorCode RenderLandscape();
	GameErrorCode RenderGeneric();
	
	GameErrorCode RenderAll(){}
	
};


#endif //__GAME_RENDER_COMPONENT_H__