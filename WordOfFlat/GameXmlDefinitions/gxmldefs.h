#ifndef __GAME_XML_TAGS_AND_ATTRIBUTES_DEFINITIONS_H__
#define __GAME_XML_TAGS_AND_ATTRIBUTES_DEFINITIONS_H__

//tag names
static const wxChar* GAME_TAG_ROOT_STR			= wxT("world");
static const wxChar* GAME_TAG_DEFINITIONS_STR	= wxT("definitions");
static const wxChar* GAME_TAG_SCENE_STR			= wxT("scene");

//definitions tags
static const wxChar* GAME_TAG_DEFS_MESHES_STR		= wxT("meshdefs");
static const wxChar* GAME_TAG_DEFS_MATERIALS_STR	= wxT("materialdefs");
static const wxChar* GAME_TAG_DEFS_INPUTS_STR		= wxT("inputdefs");
static const wxChar* GAME_TAG_DEFS_LOGICS_STR		= wxT("logicdefs");
static const wxChar* GAME_TAG_DEFS_CAMERAS_STR		= wxT("cameradefs");
static const wxChar* GAME_TAG_DEFS_RENDER_ENTS_STR	= wxT("renderentdefs");
static const wxChar* GAME_TAG_DEFS_RENDER_STR		= wxT("renderdefs");

//component tags

//component refs tags

//attribute names
static const wxChar* GAME_ATTR_NAME_STR				= wxT("name");
static const wxChar* GAME_ATTR_LANGUAGE_STR			= wxT("lang");
static const wxChar* GAME_ATTR_FILENAME_STR			= wxT("file");

//////////////////////////////////////
/////      Attribute values      /////
//////////////////////////////////////

// disciplines
static const wxChar* DISCIPLINE_CAMOUFLAGE_STR			= wxT("camouflage");
static const wxChar* DISCIPLINE_HUNTING_STR				= wxT("hunting");
static const wxChar* DISCIPLINE_SIXTH_SENSE_STR			= wxT("sixth_sense");
static const wxChar* DISCIPLINE_TRACKING_STR			= wxT("tracking");
static const wxChar* DISCIPLINE_HEALING_STR				= wxT("healing");
static const wxChar* DISCIPLINE_WEAPONSKILL_STR			= wxT("weaponskill");
static const wxChar* DISCIPLINE_MINDSHIELD_STR			= wxT("mindshield");
static const wxChar* DISCIPLINE_MINDBLAST_STR			= wxT("mindblast");
static const wxChar* DISCIPLINE_ANIMAL_KINDSHIP_STR		= wxT("animal_kinship");
static const wxChar* DISCIPLINE_MIND_OVER_MATTER_STR 	= wxT("mind_over_matter");

//weapons
static const wxChar* WEAPON_DAGGER_STR			= wxT("dagger");
static const wxChar* WEAPON_SPEAR_STR			= wxT("spear");
static const wxChar* WEAPON_MACE_STR			= wxT("mace");
static const wxChar* WEAPON_SHORT_SWORD_STR		= wxT("short_sword");
static const wxChar* WEAPON_WARHAMMER_STR		= wxT("warhammer");
static const wxChar* WEAPON_SWORD_STR			= wxT("sword");
static const wxChar* WEAPON_AXE_STR				= wxT("axe");
static const wxChar* WEAPON_QUATERSTAFF_STR		= wxT("quaterstaff");
static const wxChar* WEAPON_BROADSWORD_STR		= wxT("broadsword");
static const wxChar* WEAPON_BOW_STR				= wxT("bow");

static const wxChar* WEAPON_RANDOM_STR			= wxT("random_weapon");
static const wxChar* WEAPON_ALL_STR				= wxT("all_weapons");

//bag items
//static const wxChar* BAG_ITEM_MEAL_STR				= wxT("meal");		
//static const wxChar* BAG_ITEM_HEALING_POTION_STR	= wxT("healing_potion");
//static const wxChar* BAG_ITEM_GEMS_STR				= wxT("gems");
//static const wxChar* BAG_ITEM_HEALING_HERB_STR		= wxT("healing_herb");
//
////special items
//
//static const wxChar* SPECIAL_ITEM_HELMET_STR				= wxT("helmet");
//static const wxChar* SPECIAL_ITEM_CHAINMAIL_WAISTCOAT_STR 	= wxT("chainmail_waistcoat");
//static const wxChar* SPECIAL_ITEM_VORDAK_GEM_STR		 	= wxT("vordak_gem");
//static const wxChar* SPECIAL_ITEM_GOLDEN_KEY_STR 			= wxT("golden_key");

static const wxChar* BAG_ITEM_RANDOM_STR		= wxT("random_backpack_item");
static const wxChar* SPECIAL_ITEM_BACKPACK_STR	= wxT("backpack");

// event types
static const wxChar* EVENT_ADD_ITEM_TO_CHARACTER_STR 		= wxT("add_item_to_character"); // add item to character
static const wxChar* EVENT_ADD_ITEM_TO_SCENE_STR  	 		= wxT("add_item_to_scene"); // add item to scene
static const wxChar* EVENT_ADD_GOLD_TO_CHARACTER_STR 		= wxT("add_gold_to_character"); // add gold to character
static const wxChar* EVENT_ADD_GOLD_TO_SCENE_STR  	 		= wxT("add_gold_to_scene"); // add gold to scene
static const wxChar* EVENT_DEAD_STR  				 		= wxT("dead"); // character dies imediately
static const wxChar* EVENT_LOTERY_STR			  	 		= wxT("lotery"); // add gold to scene
static const wxChar* EVENT_CHARACTER_STR			 		= wxT("character"); // generic event applied to character directly
static const wxChar* EVENT_REMOVE_ITEM_FROM_CHARACTER_STR	= wxT("remove_item_from_character"); // remove item from character

//items placement
static const wxChar* ITEM_PLACEMENT_BACKPACK_STR    	= wxT("backpack");
static const wxChar* ITEM_PLACEMENT_WEAPON_STR  		= wxT("weapon");
static const wxChar* ITEM_PLACEMENT_BODY_HEAD_STR   	= wxT("body_head");
static const wxChar* ITEM_PLACEMENT_BODY_TORSO_STR		= wxT("body_torso");
static const wxChar* ITEM_PLACEMENT_BODY_BACK_STR		= wxT("body_back");
static const wxChar* ITEM_PLACEMENT_BODY_SHOULDER_STR	= wxT("shoulder");
static const wxChar* ITEM_PLACEMENT_POUCH_STR   		= wxT("pouch");
static const wxChar* ITEM_PLACEMENT_SPECIAL_STR 		= wxT("special");

//weapons types                 	
static const wxChar* WEAPON_CLASS_NONE_STR			= wxT("none");
static const wxChar* WEAPON_CLASS_DAGGER_STR		= WEAPON_DAGGER_STR;
static const wxChar* WEAPON_CLASS_SPEAR_STR			= WEAPON_SPEAR_STR;
static const wxChar* WEAPON_CLASS_MACE_STR			= WEAPON_MACE_STR;
static const wxChar* WEAPON_CLASS_SHORT_SWORD_STR	= WEAPON_SHORT_SWORD_STR;
static const wxChar* WEAPON_CLASS_WARHAMMER_STR		= WEAPON_WARHAMMER_STR;
static const wxChar* WEAPON_CLASS_SWORD_STR			= WEAPON_SWORD_STR;
static const wxChar* WEAPON_CLASS_AXE_STR			= WEAPON_AXE_STR;
static const wxChar* WEAPON_CLASS_QUATERSTAFF_STR	= WEAPON_QUATERSTAFF_STR;
static const wxChar* WEAPON_CLASS_BROADSWORD_STR	= WEAPON_BROADSWORD_STR;
static const wxChar* WEAPON_CLASS_BOW_STR			= WEAPON_BOW_STR;

#endif //__LONE_WOLF_XML_TAGS_AND_ATTRIBUTES_H__