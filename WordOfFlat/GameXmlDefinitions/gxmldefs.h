#ifndef __GAME_XML_TAGS_AND_ATTRIBUTES_DEFINITIONS_H__
#define __GAME_XML_TAGS_AND_ATTRIBUTES_DEFINITIONS_H__

//tag names
static const wxChar* GAME_TAG_ROOT_STR			= wxT("world");
static const wxChar* GAME_TAG_DEFINITIONS_STR	= wxT("definitions");
static const wxChar* GAME_TAG_SCENE_STR			= wxT("scene");
static const wxChar* GAME_TAG_ENTITY_STR		= wxT("entity");

//definitions tags
static const wxChar* GAME_TAG_DEFS_MESHES_STR		= wxT("meshdefs");
static const wxChar* GAME_TAG_DEFS_MATERIALS_STR	= wxT("materialdefs");
static const wxChar* GAME_TAG_DEFS_INPUTS_STR		= wxT("inputdefs");
static const wxChar* GAME_TAG_DEFS_LOGICS_STR		= wxT("logicdefs");
static const wxChar* GAME_TAG_DEFS_CAMERAS_STR		= wxT("cameradefs");
static const wxChar* GAME_TAG_DEFS_RENDER_ENTS_STR	= wxT("renderentdefs");
static const wxChar* GAME_TAG_DEFS_RENDER_STR		= wxT("renderdefs");

// component tags
static const wxChar* GAME_TAG_COMP_MESH 			= wxT("mesh");
static const wxChar* GAME_TAG_COMP_MATERIAL 		= wxT("material");
static const wxChar* GAME_TAG_COMP_RENDER_ENT 		= wxT("renderent");
static const wxChar* GAME_TAG_COMP_CAMERA	 		= wxT("camera");
static const wxChar* GAME_TAG_COMP_RENDER	 		= wxT("render");
static const wxChar* GAME_TAG_COMP_INPUT	 		= wxT("input");
static const wxChar* GAME_TAG_COMP_LOGIC	 		= wxT("logic");
static const wxChar* GAME_TAG_COMP_TRANSFORM 		= wxT("transform");
static const wxChar* GAME_TAG_COMP_PHYSICS 			= wxT("physics");
static const wxChar* GAME_TAG_COMP_SHAPE 			= wxT("shape");


// parameter tag
static const wxChar* GAME_TAG_ITEM_PARAM		= wxT("param");

// component item tags references
static const wxChar* GAME_TAG_ITEM_MESH_REF				= wxT("meshref");
static const wxChar* GAME_TAG_ITEM_MATERIAL_REF			= wxT("materialref");
static const wxChar* GAME_TAG_ITEM_RENDER_ENTITY_REF	= wxT("renderentref");
static const wxChar* GAME_TAG_ITEM_CAMERA_REF			= wxT("cameraref");
static const wxChar* GAME_TAG_ITEM_RENDER_REF			= wxT("renderref");
static const wxChar* GAME_TAG_ITEM_INPUT_REF			= wxT("inputref");
static const wxChar* GAME_TAG_ITEM_LOGIC_REF			= wxT("logicref");



//input component tags
static const wxChar* GAME_TAG_INPUT_KEY				= wxT("key");

//attribute names
static const wxChar* GAME_ATTR_NAME_STR				= wxT("name");
static const wxChar* GAME_ATTR_VALUE_STR			= wxT("value");
static const wxChar* GAME_ATTR_ACTION_STR			= wxT("action");
static const wxChar* GAME_ATTR_TYPE_STR				= wxT("type");


//////////////////////////////////////
/////      Attribute values      /////
//////////////////////////////////////





#endif //__LONE_WOLF_XML_TAGS_AND_ATTRIBUTES_H__