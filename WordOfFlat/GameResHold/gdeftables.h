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
	wxString m_defName;
	
public:
	DefBase() : m_refCount(1) {}
	virtual ~DefBase() {}
	
	void SetName(const wxString pName)
	{
		m_defName = pName;
	}
	
	const wxString& GetName() const 
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

struct RenderEntityDef : public DefBase {
	RefObjSmPtr<NameDef> m_mesh;
	RefObjSmPtr<NameDef> m_material;
	
	Ogre::Vector3 m_position;
	
	RenderEntityDef() : DefBase(),
						m_position(Ogre::Vector3::ZERO){}
};

/*!
 * \class CameraDef
 * \author Lukas
 * \date 06/01/14
 * \file gdeftables.h
 * \brief Camera definition
 */
struct CameraDef : public DefBase {
public:
	Ogre::Vector3 m_position;
	Ogre::Vector3 m_direction;
	
	CameraDef() : DefBase(),
				m_position(Ogre::Vector3::ZERO),
				m_direction(Ogre::Vector3::ZERO){}
};

/*!
 * \class LightDef
 * \author Lukas
 * \date 06/01/14
 * \file gdeftables.h
 * \brief Light definition for render component
 */
struct LightDef : public DefBase {
	
	Ogre::Vector3 m_position;
	
	LightDef() : DefBase(),
				m_position(Ogre::Vector3::ZERO){}
};

/*!
 * \class RenderDef
 * \author Lukas
 * \date 06/01/14
 * \file gdeftables.h
 * \brief Render component definition
 */
struct RenderDef : public DefBase {
public:
	wxVector< RefObjSmPtr<RenderEntityDef> > m_entities;
	wxVector< RefObjSmPtr<CameraDef> > m_cameras;
	wxVector< RefObjSmPtr<LightDef> > m_lights;
public:
	RenderDef() : DefBase() {}
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




struct InputDef : public DefBase {
	typedef GameBasMap<wxString, wxInt32 > TInputMap;
public:
	TInputMap m_inputMap;
	
public:
	InputDef() {}
	
};


struct LogicDef : public DefBase {
	wxString m_logicType;
public:
	LogicDef() {}
};

/*!
 * \class EntityDef
 * \author Lukas
 * \date 22.3.2014
 * \file gdeftables.h
 * \brief Entity definitios
 */
struct EntityDef : public DefBase {
	RefObjSmPtr<TransformDef> m_transformation;
	RefObjSmPtr<RenderDef> m_renderDef;
	RefObjSmPtr<AnimatorDef> m_animatorDef;
	RefObjSmPtr<InputDef> m_inputDef;
	RefObjSmPtr<LogicDef> m_logicDef;

	EntityDef() : DefBase() {}
};


#endif //__GAME_DEFINITIONS_TABLE_H__
