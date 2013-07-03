#ifndef __GAME_SCENE_GENERATOR_H__
#define __GAME_SCENE_GENERATOR_H__

struct EntityDef {
	GameObjectType m_entType;
	wxVector<GameTextureId> m_textureRefs; //!< List of textures - can be empty (dep. on type)
	wxVector<GameShapeId> m_geometryRefs; //!< List of geometry objects - can be empty (dep. on type)
	wxVector<GamePhysObjId> m_physRefs; //!< List of Joints or Bodys - depend on type
	b2Transform m_tranformation;
};

typedef wxDword TGameWorldSegmentID;

class IGameSceneGenerator : public IRefObject {
public:
	GameErrorCode GenLandscape(const TGameWorldSegmentID &wrldID, wxVector<EntityDef> &landscape) = 0;
	GameErrorCode GenStaticObj(const TGameWorldSegmentID &wrldID, wxVector<EntityDef> &staticObjs) = 0;
	GameErrorCode GenMoveableObj(const TGameWorldSegmentID &wrldID, wxVector<EntityDef> &moveableObjs) = 0;
	GameErrorCode GenAnimals(const TGameWorldSegmentID &wrldID, wxVector<EntityDef> &animals) = 0;
	GameErrorCode GenCharacters(const TGameWorldSegmentID &wrldID, wxVector<EntityDef> &characters) = 0;
	
};


#endif //__GAME_SCENE_GENERATOR_H__