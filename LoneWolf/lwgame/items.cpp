#include "items.h"

const Item ItemAndWeaponManager::UNKNOWN_ITEM = Item();

bool ItemAndWeaponManager::AddBagItem(EBagItems type, const wxString& title, const wxString& desc)
{
	return AddItem(type, title, desc, m_bagItemsMap);
}

bool ItemAndWeaponManager::AddSpecialItem(ESpecialItems type, const wxString& title, const wxString& desc)
{
	return AddItem(type, title, desc, m_specialItemsMap);
}

bool ItemAndWeaponManager::AddWeapon(EWeapons type, const wxString& title, const wxString& desc)
{
	return AddItem(type, title, desc, m_weaponsMap);
}

Item& ItemAndWeaponManager::GetBagItem(EWeapons type)
{
	return GetItem(type, m_bagItemsMap);
}

Item& ItemAndWeaponManager::GetSpecialItem(EWeapons type)
{
	return GetItem(type, m_specialItemsMap);
}

Item& ItemAndWeaponManager::GetWeapon(EWeapons type)
{	
	return GetItem(type, m_weaponsMap);
}

Item& ItemAndWeaponManager::AddItem(wxInt32 type, const wxString& title, const wxString& desc, TItemMap& itemMap)
{
	Item item;
	item.m_id = type;
	item.m_title = title;
	item.m_desc = desc;
	std::pair<TItemMap::iterator, bool> insertResult;
	insertResult = itemMap.insert(item);
	
	return insertResult.second;
}

Item& ItemAndWeaponManager::GetItem(wxInt32 type, TItemMap& itemMap)
{
	TItemMap::iterator iter;
	iter = itemMap.find(type);
	if(iter == itemMap.end())
	{
		return UNKNOWN_ITEM;
	}
	
	return item->second;
}
