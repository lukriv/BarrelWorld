#include "lwdefs.h"	
#include "lwxmldefs.h"

/////////////////////////////////////////////////////////
// Static tables
/////////////////////////////////////////////////////////

#define STATIC_TABLE_SIZE(table) (sizeof(table) / sizeof(table[0]))

static const ConvertTable DisciplineConvert[] = 
{
	{ DISCIPLINE_CAMOUFLAGE, 		DISCIPLINE_CAMOUFLAGE_STR},
	{ DISCIPLINE_HUNTING, 			DISCIPLINE_HUNTING_STR},
	{ DISCIPLINE_SIXTH_SENSE, 		DISCIPLINE_SIXTH_SENSE_STR},
	{ DISCIPLINE_TRACKING, 			DISCIPLINE_TRACKING_STR},
	{ DISCIPLINE_HEALING, 			DISCIPLINE_HEALING_STR},
	{ DISCIPLINE_WEAPONSKILL,		DISCIPLINE_WEAPONSKILL_STR},
	{ DISCIPLINE_MINDSHIELD, 		DISCIPLINE_MINDSHIELD_STR},
	{ DISCIPLINE_MINDBLAST, 		DISCIPLINE_MINDBLAST_STR},
	{ DISCIPLINE_ANIMAL_KINDSHIP, 	DISCIPLINE_ANIMAL_KINDSHIP_STR},
	{ DISCIPLINE_MIND_OVER_MATTER, 	DISCIPLINE_MIND_OVER_MATTER_STR}
};

static const ConvertTable WeaponConvert[] = 
{
	{ WEAPON_DAGGER, 		WEAPON_DAGGER_STR},
	{ WEAPON_SPEAR, 		WEAPON_SPEAR_STR},
	{ WEAPON_MACE, 			WEAPON_MACE_STR},
	{ WEAPON_SHORT_SWORD, 	WEAPON_SHORT_SWORD_STR},
	{ WEAPON_WARHAMMER, 	WEAPON_WARHAMMER_STR},
	{ WEAPON_SWORD,			WEAPON_SWORD_STR},
	{ WEAPON_AXE, 			WEAPON_AXE_STR},
	{ WEAPON_QUATERSTAFF, 	WEAPON_QUATERSTAFF_STR},
	{ WEAPON_BROADSWORD, 	WEAPON_BROADSWORD_STR},
};

static const ConvertTable BagItemConvert[] = 
{
	{ BAG_ITEM_MEAL, 			BAG_ITEM_MEAL_STR},
	{ BAG_ITEM_HEALING_POTION, 	BAG_ITEM_HEALING_POTION_STR},
};

static const ConvertTable SpecialItemConvert[] = 
{
	{ SPECIAL_ITEM_HELMET, 				SPECIAL_ITEM_HELMET_STR},
	{ SPECIAL_ITEM_CHAINMAIL_WAISTCOAT, SPECIAL_ITEM_CHAINMAIL_WAISTCOAT_STR},
};

static const ConvertTable ActionConvert[] = 
{
	{ ACTION_CREATE_CHAR, 	ACTION_CREATE_CHAR_STR},
	{ ACTION_MOVE, 			ACTION_MOVE_STR},
	{ ACTION_LOTERY, 		ACTION_LOTERY_STR},
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

EBagItems Convertor::ConvertBagItemNameToType(const wxString& bagItemName)
{
	return ConvertNameToType<EBagItems>(bagItemName, BagItemConvert, STATIC_TABLE_SIZE(ActionConvert));
}

EDisciplines Convertor::ConvertDisciplineNameToType(const wxString& disciplineName)
{
	return ConvertNameToType<EDisciplines>(disciplineName, DisciplineConvert, STATIC_TABLE_SIZE(ActionConvert));
}

ESpecialItems Convertor::ConvertSpecialItemNameToType(const wxString& specialItemName)
{
	return ConvertNameToType<ESpecialItems>(specialItemName, SpecialItemConvert, STATIC_TABLE_SIZE(ActionConvert));
}

EWeapons Convertor::ConvertWeaponNameToType(const wxString& weaponName)
{
	return ConvertNameToType<EWeapons>(weaponName, WeaponConvert, STATIC_TABLE_SIZE(ActionConvert));
}

const wxChar* Convertor::GetActionName(EActionType type)
{
	return GetName<EActionType>(type, ActionConvert, STATIC_TABLE_SIZE(ActionConvert));
}

const wxChar* Convertor::GetBagItemName(EBagItems type)
{
	return GetName<EBagItems>(type, BagItemConvert, STATIC_TABLE_SIZE(ActionConvert));
}

const wxChar* Convertor::GetDisciplineName(EDisciplines type)
{
	return GetName<EDisciplines>(type, DisciplineConvert, STATIC_TABLE_SIZE(ActionConvert));
}

const wxChar* Convertor::GetSpecialItemName(ESpecialItems type)
{
	return GetName<ESpecialItems>(type, SpecialItemConvert, STATIC_TABLE_SIZE(ActionConvert));
}

const wxChar* Convertor::GetWeaponName(EWeapons type)
{
	return GetName<EWeapons>(type, WeaponConvert, STATIC_TABLE_SIZE(ActionConvert));
}



