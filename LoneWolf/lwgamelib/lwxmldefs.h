#ifndef __LONE_WOLF_XML_TAGS_AND_ATTRIBUTES_H__
#define __LONE_WOLF_XML_TAGS_AND_ATTRIBUTES_H__

//tag names
static const wxChar* GENERAL_TAG_ROOT_STR			= wxT("root");
static const wxChar* GENERAL_TAG_DEFINITIONS_STR	= wxT("definitions");
static const wxChar* GENERAL_TAG_ACTIONS_STR		= wxT("actions");
static const wxChar* GENERAL_TAG_ACTION_STR			= wxT("action");
static const wxChar* GENERAL_TAG_DISCIPLINES_STR	= wxT("disciplines");
static const wxChar* GENERAL_TAG_SKILL_STR			= wxT("skill");
static const wxChar* GENERAL_TAG_ITEMS_STR			= wxT("items");
static const wxChar* GENERAL_TAG_WEAPON_STR			= wxT("weapon");
static const wxChar* GENERAL_TAG_ITEM_STR			= wxT("item");
static const wxChar* GENERAL_TAG_CHAPTER_STR		= wxT("chapter");
static const wxChar* GENERAL_TAG_SCENE_STR			= wxT("scene");
static const wxChar* GENERAL_TAG_DESC_STR			= wxT("desc");
static const wxChar* GENERAL_TAG_SELECTION_STR		= wxT("selection");
static const wxChar* GENERAL_TAG_EVENTS_STR			= wxT("events");
static const wxChar* GENERAL_TAG_EVENT_STR			= wxT("event");
static const wxChar* GENERAL_TAG_ENEMY_STR			= wxT("enemy");
static const wxChar* GENERAL_TAG_FIGHT_EVENT_STR	= wxT("fight_event");


//attribute names
static const wxChar* GENERAL_ATTR_NAME_STR				= wxT("name");
static const wxChar* GENERAL_ATTR_DESC_STR				= wxT("desc");
static const wxChar* GENERAL_ATTR_TITLE_STR				= wxT("title");
static const wxChar* GENERAL_ATTR_ADD_INFO_STR			= wxT("addinfo");
static const wxChar* GENERAL_ATTR_ID_STR				= wxT("id");
static const wxChar* GENERAL_ATTR_TARGET_STR			= wxT("target");
static const wxChar* GENERAL_ATTR_FIGTH_SKILL_STR		= wxT("fight_skill"); // skill can be used during fight
static const wxChar* GENERAL_ATTR_ONE_USE_STR			= wxT("one_use"); // one use item - item can be used only once
static const wxChar* GENERAL_ATTR_SKILL_NEEDED_STR		= wxT("skill_needed");
static const wxChar* GENERAL_ATTR_ITEM_NEEDED_STR		= wxT("item_needed");
static const wxChar* GENERAL_ATTR_CONDITIONED_STR		= wxT("conditioned");
static const wxChar* GENERAL_ATTR_NUMBERS_FROM_STR		= wxT("numbers_from");
static const wxChar* GENERAL_ATTR_NUMBERS_TO_STR		= wxT("numbers_to");
static const wxChar* GENERAL_ATTR_PLACEMENT_STR			= wxT("placement");
static const wxChar* GENERAL_ATTR_COND_ACT_STR			= wxT("cond_actual"); 	// events
static const wxChar* GENERAL_ATTR_COND_MAX_STR			= wxT("cond_max"); 		// events
static const wxChar* GENERAL_ATTR_ATTACK_ACT_STR		= wxT("attack_actual"); // events
static const wxChar* GENERAL_ATTR_ATTACK_BASE_STR		= wxT("attack_max"); 	// events
static const wxChar* GENERAL_ATTR_DURATION_STR			= wxT("duration"); 		// events - duration in fight turns
static const wxChar* GENERAL_ATTR_GOLD_COUNT_STR		= wxT("gold_count"); 	// events 
static const wxChar* GENERAL_ATTR_CANCELED_BY_ITEM_STR 	= wxT("canceled_by_item"); // events - if character has item, event has no effect
static const wxChar* GENERAL_ATTR_CANCELED_BY_SKILL_STR = wxT("canceled_by_skill"); // events - if character has skill, event has no effect
static const wxChar* GENERAL_ATTR_WIN_STR				= wxT("win"); // action fight attr - win target
static const wxChar* GENERAL_ATTR_RETREAT_STR			= wxT("retreat"); // action fight attr - retreat target
static const wxChar* GENERAL_ATTR_LOST_STR				= wxT("lost"); // action fight attr - lost target
static const wxChar* GENERAL_ATTR_TURN_TO_RETREAT_STR	= wxT("turn_to_retreat"); // action fight attr - turns until retreat will be posible
static const wxChar* GENERAL_ATTR_TURN_TO_WIN_STR		= wxT("turn_to_win"); // action fight attr - turns until you can win the fight (otherwise you lose the fight)


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

static const wxChar* WEAPON_RANDOM_STR			= wxT("random_weapon");
static const wxChar* WEAPON_ALL_STR			= wxT("all_weapons");

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
static const wxChar* EVENT_ADD_ITEM_TO_CHARACTER_STR = wxT("add_item_to_character"); // add item to character
static const wxChar* EVENT_ADD_ITEM_TO_SCENE_STR  	 = wxT("add_item_to_scene"); // add item to scene
static const wxChar* EVENT_ADD_GOLD_TO_CHARACTER_STR = wxT("add_gold_to_character"); // add gold to character
static const wxChar* EVENT_ADD_GOLD_TO_SCENE_STR  	 = wxT("add_gold_to_scene"); // add gold to scene
static const wxChar* EVENT_LOTERY_STR			  	 = wxT("lotery"); // add gold to scene


//items placement
static const wxChar* ITEM_PLACEMENT_BACKPACK_STR    = wxT("backpack");
static const wxChar* ITEM_PLACEMENT_WEAPON_STR  	= wxT("weapon");
static const wxChar* ITEM_PLACEMENT_BODY_HEAD_STR   = wxT("body_head");
static const wxChar* ITEM_PLACEMENT_BODY_TORSO_STR	= wxT("body_torso");
static const wxChar* ITEM_PLACEMENT_BODY_BACK_STR	= wxT("body_back");
static const wxChar* ITEM_PLACEMENT_POUCH_STR   	= wxT("pouch");
static const wxChar* ITEM_PLACEMENT_SPECIAL_STR 	= wxT("special");

//actions
static const wxChar* ACTION_CREATE_CHAR_STR		= wxT("create_character");
static const wxChar* ACTION_MOVE_STR			= wxT("move");
static const wxChar* ACTION_LOTERY_STR			= wxT("lotery");
static const wxChar* ACTION_FIGHT_STR			= wxT("fight");


#endif //__LONE_WOLF_XML_TAGS_AND_ATTRIBUTES_H__