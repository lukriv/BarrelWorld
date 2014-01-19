#ifndef __LONE_WOLF_DEFINITIONS_H__
#define __LONE_WOLF_DEFINITIONS_H__

#include <wx/string.h>
//enum EDisciplines {
//	DISCIPLINE_UNKNOWN		= 0,
//	DISCIPLINE_CAMOUFLAGE	= 1,		//camouflage
//	DISCIPLINE_HUNTING		= 2,		//hunting
//	DISCIPLINE_SIXTH_SENSE	= 3,		//sixth_sense
//	DISCIPLINE_TRACKING		= 4,		//tracking
//	DISCIPLINE_HEALING		= 5,		//healing
//	DISCIPLINE_WEAPONSKILL	= 6,		//weaponskill
//	DISCIPLINE_MINDSHIELD	= 7,		//mindshield
//	DISCIPLINE_MINDBLAST	= 8,		//mindblast
//	DISCIPLINE_ANIMAL_KINDSHIP	= 9,	//animal_kinship
//	DISCIPLINE_MIND_OVER_MATTER = 10	//mind_over_matter
//};
//
//enum EItem {
//	ITEM_UNKNOWN						= 0,
//	//weapons
//	WEAPON_DAGGER						= 1, //dagger
//	WEAPON_SPEAR						= 2, //spear
//	WEAPON_MACE							= 3, //mace
//	WEAPON_SHORT_SWORD					= 4, //short_sword
//	WEAPON_WARHAMMER					= 5, //warhammer
//	WEAPON_SWORD						= 6, //sword
//	WEAPON_AXE							= 7, //axe
//	WEAPON_QUATERSTAFF					= 8, //quaterstaff
//	WEAPON_BROADSWORD					= 9,  //broadsword
//		
//	WEAPON_RANDOM						= 20, // random generated weapon - abstract weapon for definitions only
//	
//	//bag items
//	BAG_ITEM_MEAL						= 21,
//	BAG_ITEM_HEALING_POTION 			= 22,
//	BAG_ITEM_GEMS					    = 23,
//	BAG_ITEM_HEALING_HERB				= 24,
//
//	//special items
//	SPECIAL_ITEM_HELMET					= 100,
//	SPECIAL_ITEM_CHAINMAIL_WAISTCOAT	= 101,
//	SPECIAL_ITEM_BACKPACK				= 102, // backpack
//	SPECIAL_ITEM_VORDAK_GEM				= 103,
//	SPECIAL_ITEM_GOLDEN_KEY				= 104
//};

typedef wxInt32 EItem;
typedef wxInt32 EDisciplines;



static const EItem ITEM_UNKNOWN = 0;
static const EDisciplines DISCIPLINE_UNKNOWN = 0;

static const wxInt32 TARGET_UNKNOWN = -1;
static const wxInt32 DURATION_INFINITE = -1;

enum EEventType {
	// event types for single event type
	EVENT_UNKNOWN 						= 0,
	EVENT_ADD_ITEM_TO_CHARACTER 		= 1,
	EVENT_ADD_ITEM_TO_SCENE				= 2,
	EVENT_ADD_GOLD_TO_CHARACTER			= 3,
	EVENT_ADD_GOLD_TO_SCENE				= 4,
	EVENT_FIGHT							= 5, //fight event
	EVENT_DEAD							= 6, // instant dead
	EVENT_CHARACTER						= 7, // generic character modification
	EVENT_REMOVE_ITEM_FROM_CHARACTER	= 8, // remove item from character (included random bag or weapon)
		
	// event types for event lists type
	EVENT_LOTERY						= 20,
	EVENT_LIST							= 21
};

enum EActionType {
	ACTION_UNKNOWN 		= 0,
	ACTION_CREATE_CHAR	= 1,
	ACTION_MOVE			= 2,
	ACTION_LOTERY		= 3,
	ACTION_FIGHT		= 4,
	ACTION_NEXT_CHAPTER = 5,
	ACTION_PROMOTE_CHAR	= 6,
	ACTION_CHOOSE_ITEM	= 7
};

enum EItemPlacement {
	ITEM_PLACEMENT_UNKNOWN		= 0,
	ITEM_PLACEMENT_BACKPACK		= 1,
	ITEM_PLACEMENT_WEAPON		= 2,
	ITEM_PLACEMENT_BODY_HEAD	= 3,
	ITEM_PLACEMENT_BODY_TORSO	= 4,
	ITEM_PLACEMENT_BODY_BACK	= 5,
	ITEM_PLACEMENT_POUCH		= 6,
	ITEM_PLACEMENT_SPECIAL		= 7
};


enum EWeaponClass {
	WEAPON_CLASS_NONE			= 0,
	WEAPON_CLASS_DAGGER			= 1,
	WEAPON_CLASS_SPEAR			= 2,
	WEAPON_CLASS_MACE			= 4,
	WEAPON_CLASS_SHORT_SWORD	= 8,
	WEAPON_CLASS_WARHAMMER		= 16,
	WEAPON_CLASS_SWORD			= 32,
	WEAPON_CLASS_AXE			= 64,
	WEAPON_CLASS_QUATERSTAFF	= 128,
	WEAPON_CLASS_BROADSWORD		= 256,
	WEAPON_CLASS_BOW			= 512,
//	WEAPON_CLASS_SHIELD			= 11
};

struct ConvertTable {
	wxInt32 m_id;
	const wxChar* m_str;
};

class Convertor {
private:
	template <typename T> 
		static T ConvertNameToType(const wxString &name, const ConvertTable table[], wxDword tableSize);
	template <typename T> 
		static const wxChar* GetName(T type, const ConvertTable table[], wxDword tableSize);

public:
	static const wxChar* GetActionName(EActionType type);
	static EActionType ConvertActionNameToType(const wxString &actionName);
	
	static const wxChar* GetPlacementName(EItemPlacement type);
	static EItemPlacement ConvertPlacementNameToType(const wxString &placementName);
	
	static const wxChar* GetEventName(EEventType type);
	static EEventType ConvertEventNameToType(const wxString &eventName);
	
	static const wxChar* GetWeaponClassName(EWeaponClass type);
	static EWeaponClass ConvertWeaponClassToType(const wxString &eventName);
	
};



#endif //__LONE_WOLF_DEFINITIONS_H__