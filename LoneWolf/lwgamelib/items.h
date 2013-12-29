#ifndef __LONE_WOLF_ITEMS_AND_WEAPONS_H__
#define __LONE_WOLF_ITEMS_AND_WEAPONS_H__


#include <map>
#include <wx/defs.h>
#include <wx/string.h>
#include "lwdefs.h"


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
	static const Item UNKNOWN_ITEM;
	
private:
	TItemMap m_weaponsMap;
	TItemMap m_bagItemsMap;
	TItemMap m_specialItemsMap;
public:
	ItemAndWeaponManager() {}
	~ItemAndWeaponManager() {}
	
	bool AddWeapon(EWeapons type, const wxString& title, const wxString& desc);
	const Item& GetWeapon(EWeapons type);
	
	bool AddBagItem(EBagItems type, const wxString& title, const wxString& desc);
	const Item& GetBagItem(EWeapons type);
	
	bool AddSpecialItem(ESpecialItems type, const wxString& title, const wxString& desc);
	const Item& GetSpecialItem(EWeapons type);

private:
	bool AddItem(wxInt32 type, const wxString& title, const wxString& desc, TItemMap& itemMap);
	const Item& GetItem(wxInt32 type, TItemMap& itemMap);
};






#endif // __LONE_WOLF_ITEMS_AND_WEAPONS_H__