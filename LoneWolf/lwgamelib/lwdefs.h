#ifndef __LONE_WOLF_DEFINITIONS_H__
#define __LONE_WOLF_DEFINITIONS_H__

#include <wx/string.h>
enum EDisciplines {
	DISCIPLINE_UNKNOWN		= 0,
	DISCIPLINE_CAMOUFLAGE	= 1,		//camouflage
	DISCIPLINE_HUNTING		= 2,		//hunting
	DISCIPLINE_SIXTH_SENSE	= 3,		//sixth_sense
	DISCIPLINE_TRACKING		= 4,		//tracking
	DISCIPLINE_HEALING		= 5,		//healing
	DISCIPLINE_WEAPONSKILL	= 6,		//weaponskill
	DISCIPLINE_MINDSHIELD	= 7,		//mindshield
	DISCIPLINE_MINDBLAST	= 8,		//mindblast
	DISCIPLINE_ANIMAL_KINDSHIP	= 9,	//animal_kinship
	DISCIPLINE_MIND_OVER_MATTER = 10	//mind_over_matter
};

enum EWeapons {
	WEAPON_UNKNOWN		= 0,
	WEAPON_DAGGER		= 1, //dagger
	WEAPON_SPEAR		= 2, //spear
	WEAPON_MACE			= 3, //mace
	WEAPON_SHORT_SWORD	= 4, //short_sword
	WEAPON_WARHAMMER	= 5, //warhammer
	WEAPON_SWORD		= 6, //sword
	WEAPON_AXE			= 7, //axe
	WEAPON_QUATERSTAFF	= 8, //quaterstaff
	WEAPON_BROADSWORD	= 9  //broadsword
};

enum EBagItems {
	BAG_ITEM_UNKNOWN		= 0,
	BAG_ITEM_MEAL			= 1,
	BAG_ITEM_HEALING_POTION = 2,
};

enum ESpecialItems {
	SPECIAL_ITEM_UNKNOWN			 = 0,
	SPECIAL_ITEM_HELMET				 = 1,
	SPECIAL_ITEM_CHAINMAIL_WAISTCOAT = 2
};


enum EActionType {
	ACTION_UNKNOWN 		= 0,
	ACTION_CREATE_CHAR	= 1,
	ACTION_MOVE			= 2,
	ACTION_LOTERY		= 3		
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
	static const wxChar* GetDisciplineName(EDisciplines type);
	static EDisciplines ConvertDisciplineNameToType(const wxString &disciplineName);
	
	static const wxChar* GetWeaponName(EWeapons type);
	static EWeapons ConvertWeaponNameToType(const wxString &weaponName);
	
	static const wxChar* GetBagItemName(EBagItems type);
	static EBagItems ConvertBagItemNameToType(const wxString &bagItemName);
	
	static const wxChar* GetSpecialItemName(ESpecialItems type);
	static ESpecialItems ConvertSpecialItemNameToType(const wxString &specialItemName);

	static const wxChar* GetActionName(EActionType type);
	static EActionType ConvertActionNameToType(const wxString &actionName);
};



#endif //__LONE_WOLF_DEFINITIONS_H__