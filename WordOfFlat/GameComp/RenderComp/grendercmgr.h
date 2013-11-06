#ifndef __GAME_RENDER_COMPONENT_MANAGER_H__
#define __GAME_RENDER_COMPONENT_MANAGER_H__

#include "../GameSystem/gdefs.h"
#include "gcompmgrimpl.h"
#include "gobjdef.h"


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
private:
	MemoryAllocContainer<GameRenderComp> m_landObjMemory;
	MemoryAllocContainer<GameRenderComp> m_genObjMemory;
// IGameComponent interface
public:
	virtual GameCompMgrType GetType() {return GAME_COMP_RENDER;}
	GameErrorCode CreateBackgroundObject();
	GameErrorCode CreateLandscapeObject(const RenderCompDef &entityDef, GameObjectId &compId);
	GameErrorCode CreateGenericComponent(const RenderCompDef &entityDef, GameObjectId &compId);
	
	GameErrorCode DeleteBackGroundObject();
	GameErrorCode DeleteLandscapeObject(GameObjectID compId);
	GameErrorCode DeleteGenericObject(GameObjectID compId);
	
	GameErrorCode GetBackgroundObject();
	GameErrorCode GetLandscapeObject(GameObjectID compID);
	GameErrorCode GetGenericObject(GameObjectID compID);
	
	bool ExistsBackgroundObject();
	bool ExistsLandscapeObject(GameObjectID compID);
	bool ExistsGenericObject(GameObjectID compID);
	
	GameErrorCode RenderBackground();
	GameErrorCode RenderLandscape();
	GameErrorCode RenderGeneric();
	
	GameErrorCode RenderAll(){}
	
};


#endif //__GAME_RENDER_COMPONENT_H__