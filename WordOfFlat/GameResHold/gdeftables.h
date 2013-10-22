#ifndef __GAME_DEFINITIONS_TABLE_H__
#define __GAME_DEFINITIONS_TABLE_H__

#include <wx/vector.h>
#include <box2d/common/b2Math.h>
#include "../GameObjects/ganimation.h"
#include "../GameObjects/gobjdef.h"



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

struct AnimationDef {
	GameAnimation::AnimationType m_animType;
	wxVector<GameObjectID> m_frameRefs;
	wxVector<sf::Time> m_frameDurations;
	bool m_repeat;
	bool m_endless;
	
	AnimationDef() :m_animType(GameAnimation::ANIM_TYPE_STATIC), m_repeat(false), m_endless(false){}
};


#endif //__GAME_DEFINITIONS_TABLE_H__