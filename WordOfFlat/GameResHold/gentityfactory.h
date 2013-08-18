#ifndef __GAME_ENTITY_FACTORY_H__
#define __GAME_ENTITY_FACTORY_H__

#include <wx/atomic.h>
#include "../GameSystem/refobjectsmptr.h"

#include "../GameObjects/gentityobj.h"
#include "../GameObjects/ganimation.h"
#include "gresholder.h"


struct BasicEntityDef {
	GameTextureId m_textureRef; //!< List of textures - can be empty (dep. on type)
	GameShapeId m_geometryRef; //!< List of geometry objects - can be empty (dep. on type)
	GamePhysObjId m_physRef; //!< List of Joints or Bodys - depend on type
	b2Transform m_tranformation;
	bool m_textureRepeat; //!< texture repetition
	
	BasicEntityDef() : m_textureRef(GAME_TEXTURE_ID_INVALID)
			, m_geometryRef(GAME_SHAPE_ID_INVALID)
			, m_physRef(GAME_PHYSICS_ID_INVALID)
			, m_textureRepeat(false) {}
};

struct AnimationDef {
	wxVector<GameTextureId> m_frameRefs;
	wxVector<sf::Time> m_frameDurations;
	bool m_repeat;
	bool m_endless;
	
	AnimationDef() : m_repeat(false), m_endless(false){}
};

class GameEntityFactory : IRefObject {
	wxAtomicInt m_refCount;
	RefObjSmPtr<GameResourceHolder> m_spResHolder;
	GameLoggerPtr m_spLogger;
public:
	GameEntityFactory() : m_refCount(1){}
	
	GameErrorCode Initialize(GameResourceHolder *pResHolder, GameLogger *pLogger);
	
	GameErrorCode CreateBasicEntity(const BasicEntityDef &entityDef, b2World &world, GameEntity &entity);
	GameErrorCode CreateAnimation(const AnimationDef& entityDef, GameAnimation &animation);
	
	
public:
 	virtual void addRef();
	virtual wxInt32 release();
};



#endif //__GAME_ENTITY_FACTORY_H__