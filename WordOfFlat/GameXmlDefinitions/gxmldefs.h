#ifndef __GAME_XML_TAGS_AND_ATTRIBUTES_DEFINITIONS_H__
#define __GAME_XML_TAGS_AND_ATTRIBUTES_DEFINITIONS_H__

//tag names
static const wxChar* FWG_UNUSED(GAME_TAG_ROOT_STR)			= wxT("world");
static const wxChar* FWG_UNUSED(GAME_TAG_DEFINITIONS_STR)	= wxT("definitions");
static const wxChar* FWG_UNUSED(GAME_TAG_SCENE_STR)			= wxT("scene");
static const wxChar* FWG_UNUSED(GAME_TAG_ENTITY_STR)		= wxT("entity");

//definitions tags   
static const wxChar* FWG_UNUSED(GAME_TAG_DEFS_INPUTS_STR)		= wxT("inputdefs");
static const wxChar* FWG_UNUSED(GAME_TAG_DEFS_LOGICS_STR)		= wxT("logicdefs");
static const wxChar* FWG_UNUSED(GAME_TAG_DEFS_RENDER_STR)		= wxT("renderdefs");

// component tags    
static const wxChar* FWG_UNUSED(GAME_TAG_COMP_TRANSFORM) 			= wxT("transform");
static const wxChar* FWG_UNUSED(GAME_TAG_COMP_INPUT)	 			= wxT("input");
static const wxChar* FWG_UNUSED(GAME_TAG_COMP_RENDER_OBJECT)		= wxT("robject");
static const wxChar* FWG_UNUSED(GAME_TAG_COMP_RENDER_POSITION)		= wxT("rposition");
static const wxChar* FWG_UNUSED(GAME_TAG_COMP_PHYSICS)				= wxT("physics");
static const wxChar* FWG_UNUSED(GAME_TAG_COMP_MOVEABLE)				= wxT("moveable");

// render object types
static const wxChar* FWG_UNUSED(GAME_TAG_TYPE_RENDER_RIGID_BODY)	= wxT("renderRigidBody");
static const wxChar* FWG_UNUSED(GAME_TAG_TYPE_RENDER_CAMERA)		= wxT("renderCamera");


// physics types
static const wxChar* FWG_UNUSED(GAME_TAG_TYPE_PHYSICS_RIGID_BODY)		= wxT("physicsRigidBody");
static const wxChar* FWG_UNUSED(GAME_TAG_TYPE_PHYSICS_STATIC_OBJECT)	= wxT("physicsStaticObject");


// input types
static const wxChar* FWG_UNUSED(GAME_TAG_TYPE_INPUT_CHARACTER)		= wxT("characterInput");
static const wxChar* FWG_UNUSED(GAME_TAG_TYPE_INPUT_FREE_CAMERA)	= wxT("freeCameraInput");

// logic types
static const wxChar* FWG_UNUSED(GAME_TAG_TYPE_MOVEABLE_FREE_CAMERA)	= wxT("logicFreeCamera");
static const wxChar* FWG_UNUSED(GAME_TAG_TYPE_MOVEABLE_MANUAL_TEST)	= wxT("logicManualTest");




static const wxChar* FWG_UNUSED(GAME_TAG_PARAM_PHYSICS_SHAPE)		= wxT("physicsShape");
static const wxChar* FWG_UNUSED(GAME_TAG_PARAM_PHYSICS_BOX_SHAPE)	= wxT("boxShape");
                     
static const wxChar* FWG_UNUSED(GAME_TAG_COMP_TERRAIN) 			= wxT("terrain");
static const wxChar* FWG_UNUSED(GAME_TAG_COMP_TERRAIN_PAGES)	= wxT("terrainpages");
static const wxChar* FWG_UNUSED(GAME_TAG_COMP_TERRAIN_PAGE)		= wxT("terrainpage");

//
static const wxChar* FWG_UNUSED(GAME_TAG_PARAM_POSITION)			= wxT("position");
static const wxChar* FWG_UNUSED(GAME_TAG_PARAM_SCALE)				= wxT("scale");
static const wxChar* FWG_UNUSED(GAME_TAG_PARAM_ROTATION)			= wxT("rotation");

static const wxChar* FWG_UNUSED(GAME_TAG_PARAM_TERRAIN_MAPSIZE)		= wxT("mapSize");
static const wxChar* FWG_UNUSED(GAME_TAG_PARAM_TERRAIN_WORLDSIZE)	= wxT("worldSize");
static const wxChar* FWG_UNUSED(GAME_TAG_PARAM_TERRAIN_PAGEX)		= wxT("pageX");
static const wxChar* FWG_UNUSED(GAME_TAG_PARAM_TERRAIN_PAGEY)		= wxT("pageY");

static const wxChar* FWG_UNUSED(GAME_TAG_PARAM_NAME)				= wxT("name");
static const wxChar* FWG_UNUSED(GAME_TAG_PARAM_MESH)				= wxT("mesh");
static const wxChar* FWG_UNUSED(GAME_TAG_PARAM_MATERIAL)			= wxT("material");
static const wxChar* FWG_UNUSED(GAME_TAG_PARAM_SHADOWS)				= wxT("shadows");
static const wxChar* FWG_UNUSED(GAME_TAG_PARAM_FOV)					= wxT("fov");
static const wxChar* FWG_UNUSED(GAME_TAG_PARAM_PROJECTION)			= wxT("projection");
static const wxChar* FWG_UNUSED(GAME_TAG_PARAM_NEAR)				= wxT("near");
static const wxChar* FWG_UNUSED(GAME_TAG_PARAM_FAR)					= wxT("far");
static const wxChar* FWG_UNUSED(GAME_TAG_PARAM_BOX_HALF_SIZE)		= wxT("halfSize");                     
static const wxChar* FWG_UNUSED(GAME_TAG_PARAM_MASS)				= wxT("mass"); 
static const wxChar* FWG_UNUSED(GAME_TAG_PARAM_INPUT_KEY)	 		= wxT("key");
static const wxChar* FWG_UNUSED(GAME_TAG_PARAM_FILENAME)			= wxT("filename");                  
                     
                     
// component item tags references
static const wxChar* FWG_UNUSED(GAME_TAG_ITEM_RENDER_REF)			= wxT("renderref");
static const wxChar* FWG_UNUSED(GAME_TAG_ITEM_INPUT_REF)			= wxT("inputref");
static const wxChar* FWG_UNUSED(GAME_TAG_ITEM_LOGIC_REF)			= wxT("logicref");
                     
                     
                     
//input component tags

//attribute names
static const wxChar* FWG_UNUSED(GAME_TAG_ATTR_NAME)				= wxT("name");
static const wxChar* FWG_UNUSED(GAME_TAG_ATTR_VALUE)			= wxT("value");
static const wxChar* FWG_UNUSED(GAME_TAG_ATTR_ACTION)			= wxT("action");
static const wxChar* FWG_UNUSED(GAME_TAG_ATTR_TYPE)				= wxT("type");



//////////////////////////////////////
/////      Attribute values      /////
//////////////////////////////////////
static const wxChar* FWG_UNUSED(GAME_TAG_VALUE_ORTHOGRAPHIC)		= wxT("orthographic");
static const wxChar* FWG_UNUSED(GAME_TAG_VALUE_PERSPECTIVE)			= wxT("perspective");




#endif //__LONE_WOLF_XML_TAGS_AND_ATTRIBUTES_H__