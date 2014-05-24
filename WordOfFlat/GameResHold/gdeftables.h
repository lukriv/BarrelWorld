#ifndef __GAME_DEFINITIONS_TABLE_H__
#define __GAME_DEFINITIONS_TABLE_H__

#include <wx/vector.h>
#include <wx/atomic.h>
#include "OGRE/OgreVector3.h"
#include "OGRE/OgreQuaternion.h"
#include "../GameSystem/refobject.h"
#include "../GameSystem/refobjectsmptr.h"

// Game entity type - on this depends input behavior of entity
enum GameDefEntityType {
	GAMEDEF_NONE 					= 0,
	GAMEDEF_MAIN_CHARACTER	 		= 1,
	GAMEDEF_AI_CHARACTER			= 2
};


/*!
 * \class DefBase
 * \author Lukas
 * \date 22.3.2014
 * \file gdeftables.h
 * \brief Base class for components and entity definitions
 */
class DefBase : IRefObject
{
protected:
	wxAtomicInt m_refCount;
	const wxString* m_defName;
	
public:
	DefBase() : m_refCount(1), m_defName(NULL) {}
	virtual ~DefBase() {}
	
	void SetName(const wxString *pName)
	{
		m_defName = pName;
	}
	
	const wxString* GetName() const 
	{
		return m_defName;
	}
	
	virtual void addRef() 
	{
		wxAtomicInc(m_refCount);
	}

	virtual wxInt32 release() 
	{
		wxInt32 refCount = wxAtomicDec(m_refCount);
		if(refCount == 0)
		{
			delete this;
		}
		
		return refCount;
	}

};



/*!
 * \class TranformCompDef
 * \author Lukas
 * \date 22.3.2014
 * \file gdeftables.h
 * \brief
 */
struct TransformDef : public DefBase {
	Ogre::Vector3 m_position;
	Ogre::Vector3 m_scale;
	Ogre::Quaternion m_rotation;
	
	TransformDef() : m_position(Ogre::Vector3::ZERO)
					, m_scale(Ogre::Vector3::UNIT_SCALE)
					, m_rotation(Ogre::Quaternion::IDENTITY) {}
};

/*!
 * \class NameDef
 * \author Lukas
 * \date 22.3.2014
 * \file gdeftables.h
 * \brief Used for named resources
 */
struct NameDef : public DefBase {
	wxString m_name; //!< Name of the mesh
	wxString m_group; //!< Name of mesh group to which mesh belongs

	NameDef() : DefBase() {}
};


/*!
 * \class AnimationDef
 * \author Lukas
 * \date 22.3.2014
 * \file gdeftables.h
 * \brief Animation definitions
 */
struct AnimationDef : public DefBase {
	wxString m_animationName; //!< Name of animation
	bool m_repeat; //!< Animation repeating

	AnimationDef() : DefBase(), m_repeat(false) {}
};

/*!
 * \class AnimatorDef
 * \author Lukas
 * \date 22.3.2014
 * \file gdeftables.h
 * \brief Class for managing animations
 */
struct AnimatorDef : public DefBase {
	wxVector< RefObjSmPtr<AnimationDef> > m_animList; //!< List of animation belong

	AnimatorDef() {}
};


struct CameraDef : public DefBase {
	CameraDef() {}
};

struct LightDef : public DefBase {
	LightDef() {}
};

struct InputDef : public DefBase {
	
};

/*!
 * \class EntityDef
 * \author Lukas
 * \date 22.3.2014
 * \file gdeftables.h
 * \brief Entity definitios
 */
struct EntityDef : public DefBase {
	wxString m_entityName;
	RefObjSmPtr<TransformDef> m_transformation;
	RefObjSmPtr<NameDef> m_mesh;
	RefObjSmPtr<NameDef> m_material;
	RefObjSmPtr<AnimatorDef> m_animatorDef;
	RefObjSmPtr<CameraDef> m_camera;
	RefObjSmPtr<LightDef> m_light;

	EntityDef() : DefBase() {}
};


#endif //__GAME_DEFINITIONS_TABLE_H__
