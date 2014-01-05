#include "items.h"
#include "lwxmldefs.h"

static Item UNKNOWN_ITEM = Item();

Item& ItemAndWeaponManager::GetItem(EItem type)
{
	TItemMap::iterator iter;
	iter = m_itemsMap.find(type);
	if(iter == m_itemsMap.end())
	{
		return UNKNOWN_ITEM;
	}
	
	return iter->second;
}

bool ItemAndWeaponManager::ItemExists(EItem type)
{
	return (m_itemsMap.find(type) != m_itemsMap.end());
}

bool ItemAndWeaponManager::AddItem(const wxString& keyName, const Item& item)
{
	std::pair<TItemConvertTable::iterator, bool> retvalConvert;
	std::pair<TItemMap::iterator, bool> retvalMap;
	
	if((item.m_id == ITEM_UNKNOWN)||(item.m_placement == ITEM_PLACEMENT_UNKNOWN)||(keyName.IsEmpty()))
	{
		// non-defined types are restricted
		return false;
	}
	
	retvalConvert = m_itemConvertTable.insert(TItemConvertTablePair(keyName, item.m_id));
	if(!retvalConvert.second)
	{
		return false;
	}
	
	retvalMap = m_itemsMap.insert(TItemMapPair(item.m_id, item));
	return retvalMap.second;
	
}

EItem ItemAndWeaponManager::GetItemType(const wxString& keyName)
{
	TItemConvertTable::iterator iter = m_itemConvertTable.find(keyName);
	if(iter == m_itemConvertTable.end())
	{
		return ITEM_UNKNOWN;
	}
	
	return iter->second;
}

bool ItemAndWeaponManager::InitializeSpecialTypes()
{
	m_allWeaponsType = GetItemType(wxString(WEAPON_ALL_STR));
	m_randomWeaponType = GetItemType(wxString(WEAPON_RANDOM_STR));
	m_randomBagItemType = GetItemType(wxString(BAG_ITEM_RANDOM_STR));
	m_backpackType = GetItemType(wxString(SPECIAL_ITEM_BACKPACK_STR));
	
	return ((m_allWeaponsType != ITEM_UNKNOWN)
				&&(m_randomWeaponType != ITEM_UNKNOWN)
				&&(m_randomBagItemType != ITEM_UNKNOWN)
				&&(m_backpackType != ITEM_UNKNOWN));
}
