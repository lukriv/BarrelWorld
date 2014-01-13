#include "lwdefs.h"	
#include "lwxmldefs.h"

/////////////////////////////////////////////////////////
// Static tables
/////////////////////////////////////////////////////////

#define STATIC_TABLE_SIZE(table) (sizeof(table) / sizeof(table[0]))

//static const ConvertTable DisciplineConvert[] = 
//{
//	{ DISCIPLINE_CAMOUFLAGE, 		DISCIPLINE_CAMOUFLAGE_STR},
//	{ DISCIPLINE_HUNTING, 			DISCIPLINE_HUNTING_STR},
//	{ DISCIPLINE_SIXTH_SENSE, 		DISCIPLINE_SIXTH_SENSE_STR},
//	{ DISCIPLINE_TRACKING, 			DISCIPLINE_TRACKING_STR},
//	{ DISCIPLINE_HEALING, 			DISCIPLINE_HEALING_STR},
//	{ DISCIPLINE_WEAPONSKILL,		DISCIPLINE_WEAPONSKILL_STR},
//	{ DISCIPLINE_MINDSHIELD, 		DISCIPLINE_MINDSHIELD_STR},
//	{ DISCIPLINE_MINDBLAST, 		DISCIPLINE_MINDBLAST_STR},
//	{ DISCIPLINE_ANIMAL_KINDSHIP, 	DISCIPLINE_ANIMAL_KINDSHIP_STR},
//	{ DISCIPLINE_MIND_OVER_MATTER, 	DISCIPLINE_MIND_OVER_MATTER_STR}
//};
//
//static const ConvertTable ItemConvert[] = 
//{
//	{ WEAPON_DAGGER, 		WEAPON_DAGGER_STR},
//	{ WEAPON_SPEAR, 		WEAPON_SPEAR_STR},
//	{ WEAPON_MACE, 			WEAPON_MACE_STR},
//	{ WEAPON_SHORT_SWORD, 	WEAPON_SHORT_SWORD_STR},
//	{ WEAPON_WARHAMMER, 	WEAPON_WARHAMMER_STR},
//	{ WEAPON_SWORD,			WEAPON_SWORD_STR},
//	{ WEAPON_AXE, 			WEAPON_AXE_STR},
//	{ WEAPON_QUATERSTAFF, 	WEAPON_QUATERSTAFF_STR},
//	{ WEAPON_BROADSWORD, 	WEAPON_BROADSWORD_STR},
//	
//	{ BAG_ITEM_MEAL, 			BAG_ITEM_MEAL_STR},
//	{ BAG_ITEM_HEALING_POTION, 	BAG_ITEM_HEALING_POTION_STR},
//	
//	{ SPECIAL_ITEM_HELMET, 				SPECIAL_ITEM_HELMET_STR},
//	{ SPECIAL_ITEM_CHAINMAIL_WAISTCOAT, SPECIAL_ITEM_CHAINMAIL_WAISTCOAT_STR},
//	{ SPECIAL_ITEM_VORDAK_GEM,			SPECIAL_ITEM_VORDAK_GEM_STR},
//	{ SPECIAL_ITEM_GOLDEN_KEY, 			SPECIAL_ITEM_GOLDEN_KEY_STR}
//
//
//};

static const ConvertTable ItemPlacementConvert[] = 
{
	{ ITEM_PLACEMENT_BACKPACK , 	ITEM_PLACEMENT_BACKPACK_STR },
	{ ITEM_PLACEMENT_WEAPON , 		ITEM_PLACEMENT_WEAPON_STR },
	{ ITEM_PLACEMENT_BODY_HEAD , 	ITEM_PLACEMENT_BODY_HEAD_STR },
	{ ITEM_PLACEMENT_BODY_TORSO , 	ITEM_PLACEMENT_BODY_TORSO_STR},
	{ ITEM_PLACEMENT_BODY_BACK , 	ITEM_PLACEMENT_BODY_BACK_STR},
	{ ITEM_PLACEMENT_POUCH , 		ITEM_PLACEMENT_POUCH_STR },
	{ ITEM_PLACEMENT_SPECIAL ,		ITEM_PLACEMENT_SPECIAL_STR },
};

static const ConvertTable ActionConvert[] = 
{
	{ ACTION_CREATE_CHAR, 	GENERAL_TAG_ACTION_CREATE_STR},
	{ ACTION_MOVE, 			GENERAL_TAG_ACTION_MOVE_STR},
	{ ACTION_LOTERY, 		GENERAL_TAG_ACTION_LOTERY_STR},
	{ ACTION_FIGHT, 		GENERAL_TAG_ACTION_FIGHT_STR}
};

static const ConvertTable EventConvert[] = 
{
	{ EVENT_ADD_ITEM_TO_CHARACTER, 	EVENT_ADD_ITEM_TO_CHARACTER_STR},
	{ EVENT_ADD_ITEM_TO_SCENE, 		EVENT_ADD_ITEM_TO_SCENE_STR},
	{ EVENT_ADD_GOLD_TO_CHARACTER,  EVENT_ADD_GOLD_TO_CHARACTER_STR },
	{ EVENT_ADD_GOLD_TO_SCENE,		EVENT_ADD_GOLD_TO_SCENE_STR},
	{ EVENT_LOTERY,					EVENT_LOTERY_STR},
	{ EVENT_DEAD,					EVENT_DEAD_STR},
	{ EVENT_CHARACTER,				EVENT_CHARACTER_STR},
};

/////////////////////////////////////////////////////////
// templates
/////////////////////////////////////////////////////////
template <typename T>
T Convertor::ConvertNameToType(const wxString& name, const ConvertTable table [], wxDword tableSize)
{
	for(wxDword i = 0; i < tableSize; i++)
	{
		if(name.Cmp(wxString(table[i].m_str)) == 0)
		{
			//strings are the same
			return static_cast<T>(table[i].m_id);
		}
	}
	
	return static_cast<T>(0);
}

template <typename T>
const wxChar* Convertor::GetName(T type, const ConvertTable table [], wxDword tableSize)
{
	for(wxDword i = 0; i < tableSize; i++)
	{
		if(table[i].m_id == type)
		{
			//strings are the same
			return table[i].m_str;
		}
	}
	
	return NULL;
}

/////////////////////////////////////////////////////////
// Static methods
/////////////////////////////////////////////////////////
	
EActionType Convertor::ConvertActionNameToType(const wxString& actionName)
{
	return ConvertNameToType<EActionType>(actionName, ActionConvert, STATIC_TABLE_SIZE(ActionConvert));
}

const wxChar* Convertor::GetActionName(EActionType type)
{
	return GetName<EActionType>(type, ActionConvert, STATIC_TABLE_SIZE(ActionConvert));
}

/////
EItemPlacement Convertor::ConvertPlacementNameToType(const wxString& placementName)
{
	return ConvertNameToType<EItemPlacement>(placementName, ItemPlacementConvert, STATIC_TABLE_SIZE(ItemPlacementConvert));
}

const wxChar* Convertor::GetPlacementName(EItemPlacement type)
{
	return GetName<EItemPlacement>(type, ItemPlacementConvert, STATIC_TABLE_SIZE(ItemPlacementConvert));
}


////
EEventType Convertor::ConvertEventNameToType(const wxString& eventName)
{
	return ConvertNameToType<EEventType>(eventName, EventConvert, STATIC_TABLE_SIZE(EventConvert));
}

const wxChar* Convertor::GetEventName(EEventType type)
{
	return GetName<EEventType>(type, EventConvert, STATIC_TABLE_SIZE(EventConvert));
}



