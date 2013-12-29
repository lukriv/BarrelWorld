#ifndef __LONE_WOLF_CHARACTER_H__
#define __LONE_WOLF_CHARACTER_H__


class FightingCharacter {
	wxInt32 m_actualAttackSkill;
	wxInt32 m_actualConditions;
public:
	wxInt32 GetActualAttackSkill();
	wxInt32 GetActualConditions();
	
	void Attack(wxInt32 attack);
};


class CharacterBag {
public:
	typedef wxVector<EBagItems>::iterator BagIterator;
private:
	wxVector<EBagItems> m_bag;
	wxInt32 m_maxBagItems;
public:
	CharacterBag() : m_maxBagItems(0) {}
	
	bool SetMaxBagItems(wxInt32 maxBagItems);

	inline wxInt32 GetMaxBagItems() {return m_maxBagItems;}
	bool AddBagItem(EBagItems item);
	bool RemoveBagItem(EBagItems item);
	
	BagIterator begin() const;
	BagIterator end() const;
	
};


class CharacterWeapons {
public:
	typedef wxVector<EWeapons>::iterator Iterator;
private:
	wxVector<EWeapons> m_weapons;
	wxInt32 m_maxWeaponCount;
public:
	CharacterWeapons() : m_maxWeaponCount(0) {}

	bool SetMaxDisciplines( wxInt32 maxDisc);
	
	inline wxInt32 GetMaxWeaponsCount() {return m_maxWeaponCount;}
	bool AddWeapon(EWeapons weapon);
	bool RemoveWeapon(EBagItems item);
	
	Iterator begin() const;
	Iterator end() const;
	
};


class CharacterDisciplines {
public:
	typedef wxVector<EDisciplines>::iterator Iterator;
private:
	wxVector<EDisciplines> m_disciplines;
	wxInt32 m_maxDiscCount;
public:
	CharacterDisciplines() : m_maxDiscCount(0) {}
	
	bool SetMaxDisciplines( wxInt32 maxDisc);
	
	inline wxInt32 GetMaxDisciplines() {return m_maxDiscCount;}
	bool AddDiscipline(EDisciplines disc);
	bool RemoveDiscipline(EBagItems item);
	
	Iterator begin() const;
	Iterator end() const;
	
};


class CharacterBody {
	ESpecialItems m_head;
	ESpecialItems m_body;
public:
	CharacterBody() : m_head(SPECIAL_ITEM_UNKNOWN), m_body(SPECIAL_ITEM_UNKNOWN) {}

	ESpecialItems GetHeadItem();
	ESpecialItems GetBodyItem();
	
	void SetHeadItem(ESpecialItems item);
	void SetBodyItem(ESpecialItems item);
}


class Character {
	// disciplines
	GlobalResourceManager m_resMgr;
	
	wxVector<ESpecialItems> m_specialItems;
	
	wxInt32 m_pouch;
	wxInt32 m_maxGoldCount;
	
	wxInt32 m_baseAttackSkill;
	wxInt32 m_baseCondition;

	CharacterBag& m_bag;
	CharacterWeapons& m_weapons;
	CharacterDisciplines& m_disciplines;
	CharacterBody m_body;
	
public:
	Character(): m_maxGoldCount(50) {}
	~Character() {}

	bool Initialize(GlobalResourceManager *resMgr);
	
	inline wxInt32 GetMaxGoldCount() {return m_maxGoldCount;}
	wxInt32 AddGold(wxInt32 goldCount);
	
	inline wxInt32 GetBaseConditions() {return m_baseCondition;}
	wxInt32 GetMaxConditions();
	
	inline wxInt32 GetBaseAttackSkill() {return m_baseAttackSkill;}

	inline CharacterBag& GetBag() {return m_bag;} 
	inline CharacterWeapons& GetWeapons() {return m_weapons;}
	inline CharacterDisciplines& GetDisciplines() {return m_disciplines;}
	inline CharacterBody& GetBody();
	
	void GenerateFightCharacter(FightingCharacter& fightChar);
	
};


#endif //__LONE_WOLF_CHARACTER_H__