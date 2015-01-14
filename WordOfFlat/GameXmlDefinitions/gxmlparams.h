#ifndef __GAME_XML_PARAMETERS_DEFINITIONS_H__
#define __GAME_XML_PARAMETERS_DEFINITIONS_H__

//////////////////////////////////////
/////      Parameter values      /////
//////////////////////////////////////

#include <GameResHold/gdeftables.h>

static const wxChar* GAME_PARAM_MESHNAME		= wxT("meshname");
static const wxChar* GAME_PARAM_MATERIALNAME	= wxT("materialname");
static const wxChar* GAME_PARAM_GROUPNAME		= wxT("groupname");
static const wxChar* GAME_PARAM_LOGICTYPE		= wxT("logictype");
static const wxChar* GAME_PARAM_POSITION		= wxT("position");
static const wxChar* GAME_PARAM_DIRECTION		= wxT("direction");
static const wxChar* GAME_PARAM_NEAR			= wxT("near");
static const wxChar* GAME_PARAM_SCALE			= wxT("scale");
static const wxChar* GAME_PARAM_ROTATION		= wxT("rotation");
static const wxChar* GAME_PARAM_MASS			= wxT("mass");
static const wxChar* GAME_PARAM_INERTIA			= wxT("inertia");
static const wxChar* GAME_PARAM_BOX_HALF_SIZE	= wxT("boxHalfSize");


enum GameDefType {
	GAMEDEF_TYPE_UNKNOWN 	= 0,
	GAMEDEF_TYPE_TEXT		= 1,
	GAMEDEF_TYPE_INT		= 2,
	GAMEDEF_TYPE_FLOAT		= 3,
	GAMEDEF_TYPE_VEC3		= 4,
	GAMEDEF_TYPE_QUAT		= 5,
};


struct ParamDefinition {
	const wxChar* m_paramName;
	GameDefType m_type;
	int m_structOffset;
	bool m_mandatory;	
};


struct PhysicsShapeTypeConvertRow {
	const wxChar* m_typeName;
	PhysShapeDef::ShapeType m_typeEnum;
};

static const PhysicsShapeTypeConvertRow PhysicsShapeTypeConvertTable[] = 
{
	{ wxT("box"), PhysShapeDef::SHAPE_TYPE_BOX }
};

// mesh parameters
static const ParamDefinition MeshXmlParamTable[] = {
	{ GAME_PARAM_MESHNAME, GAMEDEF_TYPE_TEXT, offsetof(struct NameDef, m_name), true },
	{ GAME_PARAM_GROUPNAME, GAMEDEF_TYPE_TEXT, offsetof(struct NameDef, m_group), false }
};

static const ParamDefinition MaterialXmlParamTable[] = {
	{ GAME_PARAM_MATERIALNAME, GAMEDEF_TYPE_TEXT, offsetof(struct NameDef, m_name), true },
	{ GAME_PARAM_GROUPNAME, GAMEDEF_TYPE_TEXT, offsetof(struct NameDef, m_group), false }
};

static const ParamDefinition RenderEntityXmlParamTable[] = {
	{ GAME_PARAM_POSITION, GAMEDEF_TYPE_VEC3, offsetof(struct RenderEntityDef, m_position), false }
};

static const ParamDefinition CameraXmlParamTable[] = {
	{ GAME_PARAM_POSITION, GAMEDEF_TYPE_VEC3, offsetof(struct CameraDef, m_position), true },
	{ GAME_PARAM_DIRECTION, GAMEDEF_TYPE_VEC3, offsetof(struct CameraDef, m_direction), true },
	{ GAME_PARAM_NEAR, GAMEDEF_TYPE_FLOAT, offsetof(struct CameraDef, m_near), false },
};

static const ParamDefinition LogicXmlParamTable[] = {
	{ GAME_PARAM_LOGICTYPE, GAMEDEF_TYPE_TEXT, offsetof(struct LogicDef, m_logicType), true }
};

static const ParamDefinition TransformXmlParamTable[] = {
	{ GAME_PARAM_POSITION, GAMEDEF_TYPE_VEC3, offsetof(struct TransformDef, m_position), false },
	{ GAME_PARAM_SCALE, GAMEDEF_TYPE_VEC3, offsetof(struct TransformDef, m_scale), false },
	{ GAME_PARAM_ROTATION, GAMEDEF_TYPE_QUAT, offsetof(struct TransformDef, m_rotation), false },
};

static const ParamDefinition PhysicsXmlParamTable[] = {
	{ GAME_PARAM_MASS, GAMEDEF_TYPE_FLOAT, offsetof(struct PhysCompDef, m_mass), true },
	{ GAME_PARAM_INERTIA, GAMEDEF_TYPE_VEC3, offsetof(struct PhysCompDef, m_inertiaVector), false },
};

static const ParamDefinition PhysicsShapeXmlParamTable[] = {
	{ GAME_PARAM_BOX_HALF_SIZE, GAMEDEF_TYPE_VEC3, offsetof(struct PhysShapeDef, m_boxHalfSize), false },
};


#endif //__GAME_XML_PARAMETERS_DEFINITIONS_H__