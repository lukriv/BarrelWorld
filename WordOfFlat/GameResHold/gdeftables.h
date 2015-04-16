#ifndef __GAME_DEFINITIONS_TABLE_H__
#define __GAME_DEFINITIONS_TABLE_H__

#include <wx/vector.h>
#include <wx/atomic.h>
#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>
#include <GameSystem/refobject.h>
#include <GameSystem/refobjectsmptr.h>
#include <GameSystem/gmap.h>

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
 * \class RenderDef
 * \author Lukas
 * \date 06/01/14
 * \file gdeftables.h
 * \brief Render component definition
 */
struct RenderDef : public DefBase {
public:
	wxString m_dotSceneDef;
public:
	RenderDef() : DefBase() {}
};



struct InputDef : public DefBase {
	typedef GameBasMap<wxString, wxInt32 > TInputMap;
public:
	enum InputType {
		INPUT_TYPE_GENERIC 			= 0,
		INPUT_TYPE_CHARACTER 		= 1,
		INPUT_TYPE_FREE_CAMERA 		= 2,
		INPUT_TYPE_GENERIC_MOUSE	= 3
	};
public:
	InputType m_inputType;
	TInputMap m_inputMap;
	
public:
	InputDef() : m_inputType(INPUT_TYPE_GENERIC) {}
	
};


struct LogicDef : public DefBase {
	wxString m_logicType;
public:
	LogicDef() {}
};


struct PhysShapeDef : public DefBase {
public:
	enum ShapeType {
		SHAPE_TYPE_UNKNOWN 	= 0,
		SHAPE_TYPE_BOX		= 1,
	};
public:
	ShapeType m_shapeType;
	Ogre::Vector3 m_boxHalfSize;
	
	
	PhysShapeDef() : DefBase()
				, m_shapeType(SHAPE_TYPE_UNKNOWN)
				, m_boxHalfSize(Ogre::Vector3::UNIT_SCALE) {}
	
};

struct PhysCompDef : public DefBase {
public:
	enum PhysicsType {
		PHYS_TYPE_UNKNOWN 	= 0,
		PHYS_TYPE_COLLISION = 1,
		PHYS_TYPE_RIGID		= 2
	};
public:
	PhysicsType m_physType;
	RefObjSmPtr<PhysShapeDef> m_shape;
	float m_mass;
	Ogre::Vector3 m_inertiaVector;
	
	PhysCompDef() : DefBase()
					, m_physType(PHYS_TYPE_UNKNOWN)
					, m_mass(0.0f)
					, m_inertiaVector(Ogre::Vector3::ZERO) {}
};

struct TerrainPage : public DefBase {
	wxString m_filename;
	wxInt32 m_pageX;
	wxInt32 m_pageY;
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
	RefObjSmPtr<InputDef> m_inputDef;
	RefObjSmPtr<LogicDef> m_logicDef;
	RefObjSmPtr<PhysCompDef> m_physDef;
	EntityDef() : DefBase() {}
};

struct TerrainDef : public DefBase {
	wxDword m_mapSize;
	float m_worldSize;
	wxVector< RefObjSmPtr<TerrainPage> > m_terrainPages;
};


#endif //__GAME_DEFINITIONS_TABLE_H__
