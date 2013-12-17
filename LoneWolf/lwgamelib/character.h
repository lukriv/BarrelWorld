#ifndef __LONE_WOLF_CHARACTER_H__
#define __LONE_WOLF_CHARACTER_H__



struct Character {
	// disciplines
	wxVector<EDisciplines> m_disciplines;
	wxDword m_maxDiscCount;
	
	wxVector<EWeapons> m_weapons;
	wxDword m_maxWeaponCount;
	
	wxVector<EBagItems> m_bag;
	wxDword m_maxBagItems;
	
	wxVector<ESpecialItems> m_specialItems;
	
	wxDword m_pouch;
	wxDword m_maxGoldCount;
	
	wxDword m_baseAttackSkill;
	wxDword m_baseCondition;	

	wxDword m_actualAttackSkill;
	wxDword m_actualCondition;
	
public:
	Character(): m_maxDiscCount(5), m_maxWeaponCount(2), m_maxBagItems(8), m_maxGoldCount(50) {}
	~Character() {}
};


#endif //__LONE_WOLF_CHARACTER_H__