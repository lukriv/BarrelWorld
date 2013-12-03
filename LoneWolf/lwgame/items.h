#ifndef __LONE_WOLF_ITEMS_AND_WEAPONS_H__
#define __LONE_WOLF_ITEMS_AND_WEAPONS_H__


struct Item {
	wxInt32 m_id;
	wxString m_title;
	wxString m_desc;
	
public:
	Item() : m_id(0) {}
};



class ItemAndWeaponManager {
public:
	ItemAndWeaponManager() {}
	~ItemAndWeaponManager() {}
	
	AddWeapon(EWeapons type, const wxString& title, const wxString& desc);
	AddBagItem(EBagItems type, const wxString& title, const wxString& desc);
	AddSpecialItem(ESpecialItems type, const wxString& title, const wxString& desc);
};






#endif // __LONE_WOLF_ITEMS_AND_WEAPONS_H__