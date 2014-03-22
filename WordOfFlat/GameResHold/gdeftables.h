#ifndef __GAME_DEFINITIONS_TABLE_H__
#define __GAME_DEFINITIONS_TABLE_H__

#include <wx/vector.h>
#include <wx/atomic.h>
#include "../GameSystem/refobject.h"

typedef wxDword GameDefId;
static const GameDefId GAME_DEF_ID_UNDEFINED = 0;

/*!
 * \class CompDefBase
 * \author Lukas
 * \date 22.3.2014
 * \file gdeftables.h
 * \brief Base class for components and entity definitions
 */
class CompDefBase {
protected:
	wxDword m_compDefId;
	wxAtomicInt m_refCount;
public:
	CompDefBase() : m_compId(GAME_DEF_ID_UNDEFINED), m_refCount(1) {}
	virtual ~CompDefBase() {}
	
	inline void SetDefId(GameDefId id) { m_compDefId = id; }
	inline GameDefId GetDefId() { return m_compDefId; }

};



/*!
 * \class TranformCompDef
 * \author Lukas
 * \date 22.3.2014
 * \file gdeftables.h
 * \brief 
 */
struct TranformCompDef {
	Ogre::Vector3 m_position;
	Ogre::Vector3 m_scale;
	Ogre::Quaternion m_rotation;
};

/*!
 * \class RenderCompDef
 * \author Lukas
 * \date 22.3.2014
 * \file gdeftables.h
 * \brief Include render component definitions
 */
struct RenderCompDef : public CompDefBase {
	wxString m_meshName; //!< Name of the mesh
	wxString m_meshGroup; //!< Name of mesh group to which mesh belongs

	RenderCompDef() : CompDefBase() {}
};

/*!
 * \class AnimationDef
 * \author Lukas
 * \date 22.3.2014
 * \file gdeftables.h
 * \brief Animation definitions
 */
struct AnimationDef : public CompDefBase {
	wxString m_animationName; //!< Name of animation
	bool m_repeat; //!< Animation repeating

	AnimationDef() : CompDefBase(), m_repeat(false) {}
};

/*!
 * \class AnimatorDef
 * \author Lukas
 * \date 22.3.2014
 * \file gdeftables.h
 * \brief Class for managing animations
 */
struct AnimatorDef : public CompDefBase {
	wxVector<wxString> m_animList; //!< List of animation belong
	
	AnimatorDef() {}
};

/*!
 * \class EntityDef
 * \author Lukas
 * \date 22.3.2014
 * \file gdeftables.h
 * \brief Entity definitios
 */
struct EntityDef : public CompDefBase {
	TranformCompDef m_tranformation;
	wxString m_renderDef;
	wxString m_animatorDef;
	
	EntityDef() : CompDefBase() {}
};


#endif //__GAME_DEFINITIONS_TABLE_H__
