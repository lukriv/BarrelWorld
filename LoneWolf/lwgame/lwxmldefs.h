#ifndef __LONE_WOLF_XML_TAGS_AND_ATTRIBUTES_H__
#define __LONE_WOLF_XML_TAGS_AND_ATTRIBUTES_H__

//tag names
static const wxChar* GENERAL_TAG_ROOT_STR			= wxT("root");
static const wxChar* GENERAL_TAG_DEFINITIONS_STR	= wxT("definitions");
static const wxChar* GENERAL_TAG_ACTIONS_STR		= wxT("actions");
static const wxChar* GENERAL_TAG_ACTION_STR			= wxT("action");
static const wxChar* GENERAL_TAG_DISCIPLINES_STR	= wxT("disciplines");
static const wxChar* GENERAL_TAG_SKILL_STR			= wxT("skill");
static const wxChar* GENERAL_TAG_WEAPONS_STR		= wxT("weapons");
static const wxChar* GENERAL_TAG_WEAPON_STR			= wxT("weapon");
static const wxChar* GENERAL_TAG_BAG_ITEMS_STR		= wxT("bag_items");
static const wxChar* GENERAL_TAG_BAG_ITEM_STR		= wxT("bag_item");
static const wxChar* GENERAL_TAG_SPECIAL_ITEMS_STR	= wxT("special_items");
static const wxChar* GENERAL_TAG_SPECIAL_ITEM_STR	= wxT("special_item");
static const wxChar* GENERAL_TAG_CHAPTER_STR		= wxT("chapter");
static const wxChar* GENERAL_TAG_SCENE_STR			= wxT("scene");
static const wxChar* GENERAL_TAG_DESC_STR			= wxT("desc");
static const wxChar* GENERAL_TAG_SELECTION_STR		= wxT("selection");


//attribute names
static const wxChar* GENERAL_ATTR_NAME_STR			= wxT("name");
static const wxChar* GENERAL_ATTR_DESC_STR			= wxT("desc");
static const wxChar* GENERAL_ATTR_TITLE_STR			= wxT("title");
static const wxChar* GENERAL_ATTR_CHOICE_STR		= wxT("choice");
static const wxChar* GENERAL_ATTR_ID_STR			= wxT("id");
static const wxChar* GENERAL_ATTR_TARGET_STR		= wxT("target");
static const wxChar* GENERAL_ATTR_SKILL_NEEDED_STR	= wxT("skill_needed");
static const wxChar* GENERAL_ATTR_NUMBERS_STR		= wxT("numbers");

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
static const wxChar* WEAPON_DAGGER_STR		= wxT("dagger");
static const wxChar* WEAPON_SPEAR_STR		= wxT("spear");
static const wxChar* WEAPON_MACE_STR		= wxT("mace");
static const wxChar* WEAPON_SHORT_SWORD_STR	= wxT("short_sword");
static const wxChar* WEAPON_WARHAMMER_STR	= wxT("warhammer");
static const wxChar* WEAPON_SWORD_STR		= wxT("sword");
static const wxChar* WEAPON_AXE_STR			= wxT("axe");
static const wxChar* WEAPON_QUATERSTAFF_STR	= wxT("quaterstaff");
static const wxChar* WEAPON_BROADSWORD_STR	= wxT("broadsword");

//bag items
static const wxChar* BAG_ITEM_MEAL_STR				= wxT("meal");		
static const wxChar* BAG_ITEM_HEALING_POTION_STR	= wxT("healing_potion");

//special items
static const wxChar* SPECIAL_ITEM_HELMET_STR				= wxT("helmet");
static const wxChar* SPECIAL_ITEM_CHAINMAIL_WAISTCOAT_STR 	= wxT("chainmail_waistcoat");

//actions
static const wxChar* ACTION_CREATE_CHAR_STR		= wxT("create_character");
static const wxChar* ACTION_MOVE_STR			= wxT("move");
static const wxChar* ACTION_LOTERY_STR			= wxT("lotery");


#endif //__LONE_WOLF_XML_TAGS_AND_ATTRIBUTES_H__