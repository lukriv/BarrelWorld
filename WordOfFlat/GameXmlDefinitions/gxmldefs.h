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
static const wxChar* FWG_UNUSED(GAME_TAG_COMP_RENDER_RIGID_BODY)	= wxT("renderRigidBody");
static const wxChar* FWG_UNUSED(GAME_TAG_COMP_LOGIC_FREE_CAMERA)	= wxT("logicFreeCamera");

static const wxChar* FWG_UNUSED(GAME_TAG_COMP_PHYSICS_RIGID_BODY)	= wxT("physicsRigidBody");
                     
                     
static const wxChar* FWG_UNUSED(GAME_TAG_COMP_TERRAIN) 			= wxT("terrain");
static const wxChar* FWG_UNUSED(GAME_TAG_COMP_TERRAIN_PAGES)	= wxT("terrainpages");
static const wxChar* FWG_UNUSED(GAME_TAG_COMP_TERRAIN_PAGE)		= wxT("terrainpage");
                     
                     
//                   
static const wxChar* FWG_UNUSED(GAME_TAG_PARAM_POSITION)			= wxT("position");
static const wxChar* FWG_UNUSED(GAME_TAG_PARAM_SCALE)				= wxT("scale");
static const wxChar* FWG_UNUSED(GAME_TAG_PARAM_ROTATION)			= wxT("rotation");
static const wxChar* FWG_UNUSED(GAME_TAG_PARAM_MESH)				= wxT("mesh");
static const wxChar* FWG_UNUSED(GAME_TAG_PARAM_MATERIAL)			= wxT("material");
                     
                     
                     
                     
// component item tags references
static const wxChar* FWG_UNUSED(GAME_TAG_ITEM_RENDER_REF)			= wxT("renderref");
static const wxChar* FWG_UNUSED(GAME_TAG_ITEM_INPUT_REF)			= wxT("inputref");
static const wxChar* FWG_UNUSED(GAME_TAG_ITEM_LOGIC_REF)			= wxT("logicref");
                     
                     
                     
//input component tags

//attribute names
static const wxChar* FWG_UNUSED(GAME_ATTR_NAME_STR)				= wxT("name");
static const wxChar* FWG_UNUSED(GAME_ATTR_VALUE_STR)			= wxT("value");
static const wxChar* FWG_UNUSED(GAME_ATTR_ACTION_STR)			= wxT("action");
static const wxChar* FWG_UNUSED(GAME_ATTR_TYPE_STR)				= wxT("type");



//////////////////////////////////////
/////      Attribute values      /////
//////////////////////////////////////





#endif //__LONE_WOLF_XML_TAGS_AND_ATTRIBUTES_H__