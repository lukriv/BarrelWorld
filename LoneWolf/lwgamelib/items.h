#ifndef __LONE_WOLF_ITEMS_AND_WEAPONS_H__
#define __LONE_WOLF_ITEMS_AND_WEAPONS_H__


#include <map>
#include <wx/defs.h>
#include <wx/string.h>
#include "lwdefs.h"
#include "eventprop.h"



struct Item {
	EItem m_id;
	wxString m_title;
	wxString m_desc;
	EItemPlacement m_placement;
	ItemProperties m_properties;
	
public:
	Item() : m_id(ITEM_UNKNOWN) {}
	
	Item& operator=(const Item& item)
	{
		if(this == &item)
		{
			return *this;
		}
		m_id = item.m_id;
		m_title = item.m_title;
		m_desc = item.m_desc;
		m_placement = item.m_placement;
		m_properties = item.m_properties;
		
		return *this;
	}
};



class ItemAndWeaponManager {
	typedef std::map<EItem,Item> TItemMap;
	typedef std::pair<EItem,Item> TItemMapPair;
	
	typedef std::map<wxString,EItem> TItemConvertTable;
	typedef std::pair<wxString,EItem> TItemConvertTablePair;
	
private:
	TItemMap m_itemsMap;
	TItemConvertTable m_itemConvertTable;
	
	EItem m_allWeaponsType;
	EItem m_randomWeaponType;
	EItem m_randomBagItemType;
	EItem m_backpackType;
	
public:
	ItemAndWeaponManager() : m_allWeaponsType(ITEM_UNKNOWN), m_randomWeaponType(ITEM_UNKNOWN), m_randomBagItemType(ITEM_UNKNOWN){}
	~ItemAndWeaponManager() {}
	
	bool InitializeSpecialTypes();
	
	bool AddItem(const wxString& keyName, const Item& item );
	bool ItemExists(EItem type);
	Item& GetItem(EItem type);
	
	EItem GetItemType(const wxString& keyName);
	
	inline EItem GetAllWeaponsType() const { return m_allWeaponsType; }
	inline EItem GetRandomWeaponType() const { return m_randomWeaponType; }
	inline EItem GetRandomBagItemType() const { return m_randomBagItemType; }
	inline EItem GetBackpackType() const { return m_backpackType; }

};






#endif // __LONE_WOLF_ITEMS_AND_WEAPONS_H__