#ifndef __GAME_SCENE_GENERATOR_H__
#define __GAME_SCENE_GENERATOR_H__


#include <wx/vector.h>
#include <Box2D/Box2D.h>
#include "../GameSystem/refobject.h"
#include "../GameSystem/gerror.h"
#include "../GameObjects/gameobjdef.h"



struct EntityDef {
	GameObjectType m_entType;
	wxVector<GameTextureId> m_textureRefs; //!< List of textures - can be empty (dep. on type)
	wxVector<GameShapeId> m_geometryRefs; //!< List of geometry objects - can be empty (dep. on type)
	wxVector<GamePhysObjId> m_physRefs; //!< List of Joints or Bodys - depend on type
	b2Transform m_tranformation;
	bool m_textureRepeat; //!< texture repetition
	
	EntityDef() : m_textureRepeat(false) {}
};

typedef wxDword TGameWorldSegmentID;

class IGameSceneGenerator : public IRefObject {
public:
	virtual GameErrorCode GenLandscape(const TGameWorldSegmentID &wrldID, wxVector<EntityDef> &landscape) = 0;
	virtual GameErrorCode GenStaticObj(const TGameWorldSegmentID &wrldID, wxVector<EntityDef> &staticObjs) = 0;
	virtual GameErrorCode GenMoveableObj(const TGameWorldSegmentID &wrldID, wxVector<EntityDef> &moveableObjs) = 0;
	virtual GameErrorCode GenAnimals(const TGameWorldSegmentID &wrldID, wxVector<EntityDef> &animals) = 0;
	virtual GameErrorCode GenCharacters(const TGameWorldSegmentID &wrldID, wxVector<EntityDef> &characters) = 0;
	
};


#endif //__GAME_SCENE_GENERATOR_H__