#include "lwdefs.h"	
#include "lwxmldefs.h"


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
}

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
}

static const ConvertTable BagItemConvert[] = 
{
	{ BAG_ITEM_MEAL, 			BAG_ITEM_MEAL_STR},
	{ BAG_ITEM_HEALING_POTION, 	BAG_ITEM_HEALING_POTION_STR},
}

static const ConvertTable SpecialItemConvert[] = 
{
	{ SPECIAL_ITEM_HELMET, 				SPECIAL_ITEM_HELMET_STR},
	{ SPECIAL_ITEM_CHAINMAIL_WAISTCOAT, SPECIAL_ITEM_CHAINMAIL_WAISTCOAT_STR},
}

static const ConvertTable ActionConvert[] = 
{
	{ ACTION_CREATE_CHAR, 	ACTION_CREATE_CHAR_STR},
	{ ACTION_MOVE, 			ACTION_MOVE_STR},
	{ ACTION_LOTERY, 		ACTION_LOTERY_STR},
}
	
EActionType Convertor::ConvertActionNameToType(const wxString& actionName)
{
}

EBagItems Convertor::ConvertBagItemNameToType(const wxString& bagItemName)
{
}

EDisciplines Convertor::ConvertDisciplineNameToType(const wxString& disciplineName)
{
}

ESpecialItems Convertor::ConvertSpecialItemNameToType(const wxString& specialItemName)
{
}

EWeapons Convertor::ConvertWeaponNameToType(const wxString& weaponName)
{
}

const wxChar* Convertor::GetActionName(EActionType type)
{
}

const wxChar* Convertor::GetBagItemName(EBagItems type)
{

}

const wxChar* Convertor::GetDisciplineName(EDisciplines type)
{

}

const wxChar* Convertor::GetSpecialItemName(ESpecialItems type)
{
}

const wxChar* Convertor::GetWeaponName(EWeapons type)
{

}

const wxDword Convertor::GetConvertTableSize(const ConvertTable[] table)
{
	return (sizeof(table) / sizeof(ConvertTable));
}
