#ifndef __LONE_WOLF_ITEMS_AND_WEAPONS_H__
#define __LONE_WOLF_ITEMS_AND_WEAPONS_H__


#include <map>


struct Item {
	wxInt32 m_id;
	wxString m_title;
	wxString m_desc;
	
public:
	Item() : m_id(0) {}
	
	Item& operator=(const Item& item)
	{
		if(this == &item)
		{
			return *this;
		}
		m_id = item.m_id;
		m_title = item.m_title;
		m_desc = item.m_desc;
		
		return *this;
	}
};



class ItemAndWeaponManager {
	typedef std::map<wxInt32,Item> TItemMap;
	typedef std::pair<wxInt32,Item> TItemMapPair;
	const Item UNKNOWN_ITEM;
	
private:
	TItemMap m_weaponsMap;
	TItemMap m_bagItemsMap;
	TItemMap m_specialItemsMap;
public:
	ItemAndWeaponManager() {}
	~ItemAndWeaponManager() {}
	
	bool AddWeapon(EWeapons type, const wxString& title, const wxString& desc);
	Item& GetWeapon(EWeapons type);
	
	bool AddBagItem(EBagItems type, const wxString& title, const wxString& desc);
	Item& GetBagItem(EWeapons type);
	
	bool AddSpecialItem(ESpecialItems type, const wxString& title, const wxString& desc);
	Item& GetSpecialItem(EWeapons type);

private:
	Item& AddItem(wxInt32 type, const wxString& title, const wxString& desc, TItemMap& itemMap);
	Item& GetItem(wxInt32 type, TItemMap& itemMap);
};






#endif // __LONE_WOLF_ITEMS_AND_WEAPONS_H__