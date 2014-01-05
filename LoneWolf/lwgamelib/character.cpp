#include "character.h"
#include <iostream>




//----------------------------------------------------------------
//---------------------  Character  -----------------------
//----------------------------------------------------------------
wxInt32 Character::AddGold(wxInt32 goldCount)
{
	if(goldCount > 0)
	{
		// you add money
		wxInt32 rest = 0;
		m_pouch += goldCount;
		rest = m_pouch - m_maxGoldCount;
		m_pouch = (m_pouch > m_maxGoldCount) ? m_maxGoldCount : m_pouch;
		return (rest > 0) ? rest : 0;
	} else {
		// you lose money
		wxInt32 rest = 0;
		m_pouch += goldCount;
		rest = m_pouch;
		m_pouch = (m_pouch < 0) ? 0 : m_pouch;
		return (rest < 0) ? rest : 0;
	}
}

wxInt32 Character::SetGold(wxInt32 goldCount)
{
	if(goldCount >= 0)
	{
		// you add money
		wxInt32 rest = 0;
		m_pouch = goldCount;
		rest = m_pouch - m_maxGoldCount;
		m_pouch = (m_pouch > m_maxGoldCount) ? m_maxGoldCount : m_pouch;
		return (rest > 0) ? rest : 0;
	} else {
		m_pouch = 0;
		return goldCount;
	}
}

void Character::GenerateFightCharacter(FightingCharacter& fightChar)
{
}

bool Character::Initialize(GlobalResourceManager* pResMgr)
{
	if(pResMgr == NULL)
	{
		return false;
	}
	
	m_backpack.SetMaxCount(5);
	m_maxGoldCount = 50;
	m_pResMgr = pResMgr;
	
	return true;	
}

bool Character::GenerateNewCharacter()
{
	m_baseAttackSkill = RandomSpin() + 10;
	m_actualCondition = m_baseCondition = RandomSpin() + 20;
	RecomputeState();
	
	return true;
}

void Character::RecomputeState()
{
	// update conditions
	wxInt32 oldMax = m_maxCondition;
	m_maxCondition = m_baseCondition;
	
	if(m_body.GetHeadItem() != ITEM_UNKNOWN)
	{
		m_maxCondition += m_pResMgr->GetItemAndDiscMgr().GetItem(m_body.GetHeadItem()).m_properties.m_maxCond;
	}
	
	if(m_body.GetTorsoItem() != ITEM_UNKNOWN)
	{
		m_maxCondition += m_pResMgr->GetItemAndDiscMgr().GetItem(m_body.GetTorsoItem()).m_properties.m_maxCond;
	}
	
	if((m_maxCondition < m_actualCondition)||(oldMax == m_actualCondition))
	{
		m_actualCondition = m_maxCondition;
	}
	
	if((m_body.GetBackItem() == m_pResMgr->GetItemAndDiscMgr().GetBackpackType())&&(m_backpack.GetMaxCount() == 0))
	{
		m_backpack.SetMaxCount(8);
	}
	
	if((m_body.GetBackItem() != m_pResMgr->GetItemAndDiscMgr().GetBackpackType())&&(m_backpack.GetMaxCount() > 0))
	{
		m_backpack.Clear();
		m_backpack.SetMaxCount(0);
	}
}

bool Character::ContainsItem(EItem item)
{
	if(m_backpack.Contains(item)) return true;
	if(m_specialItems.Contains(item)) return true;
	if(m_body.GetHeadItem() == item) return true;
	if(m_body.GetTorsoItem() == item) return true;
	if(m_weapons.Contains(item)) return true;
	return false;
}

bool Character::DropItem(EItem item, Scene& scene)
{
	switch(m_pResMgr->GetItemAndDiscMgr().GetItem(item).m_placement)
	{
		case ITEM_PLACEMENT_BACKPACK:
			if(!m_backpack.Contains(item)) return false;
			if(!m_backpack.Remove(item)) return false;
			scene.AddItem(item);
			break;			
		case ITEM_PLACEMENT_WEAPON:
			if(!m_weapons.Contains(item)) return false;
			if(!m_weapons.Remove(item)) return false;
			scene.AddItem(item);
			break;
		case ITEM_PLACEMENT_BODY_HEAD:
			if(m_body.GetHeadItem() != item) return false;
			m_body.SetHeadItem(ITEM_UNKNOWN);
			scene.AddItem(item);
			break;
		case ITEM_PLACEMENT_BODY_TORSO:
			if(m_body.GetTorsoItem() != item) return false;
			m_body.SetTorsoItem(ITEM_UNKNOWN);
			scene.AddItem(item);
			break;
		case ITEM_PLACEMENT_BODY_BACK:
			if(m_body.GetBackItem() != item) return false;
			if(item == m_pResMgr->GetItemAndDiscMgr().GetBackpackType())
			{
				if(!m_backpack.IsEmpty()) return false;
			}
			m_body.SetBackItem(ITEM_UNKNOWN);
			scene.AddItem(item);
			break;
		case ITEM_PLACEMENT_SPECIAL:
			if(!m_specialItems.Contains(item)) return false;
			if(!m_specialItems.Remove(item)) return false;
			scene.AddItem(item);
			break;
		case ITEM_PLACEMENT_POUCH:
		default:
			return false;
	}
	RecomputeState();
	return true;
}

bool Character::PickUpItem(EItem item, Scene& scene)
{
	if(!scene.ContainsItem(item)) return false;
	switch(m_pResMgr->GetItemAndDiscMgr().GetItem(item).m_placement)
	{
		case ITEM_PLACEMENT_BACKPACK:
			if(m_backpack.IsFull()) return false;
			if(!m_backpack.Add(item)) return false;
			scene.RemoveItem(item);
			break;			
		case ITEM_PLACEMENT_WEAPON:
			if(m_weapons.IsFull()) return false;
			if(!m_weapons.Add(item)) return false;
			scene.RemoveItem(item);
			break;
		case ITEM_PLACEMENT_BODY_HEAD:
			if(m_body.GetHeadItem() != ITEM_UNKNOWN) return false;
			m_body.SetHeadItem(item);
			scene.RemoveItem(item);
			break;
		case ITEM_PLACEMENT_BODY_TORSO:
			if(m_body.GetTorsoItem() != ITEM_UNKNOWN) return false;
			m_body.SetTorsoItem(item);
			scene.RemoveItem(item);
			break;
		case ITEM_PLACEMENT_BODY_BACK:
			if(m_body.GetBackItem() != ITEM_UNKNOWN) return false;
			m_body.SetBackItem(item);
			scene.RemoveItem(item);
			break;
		case ITEM_PLACEMENT_SPECIAL:
			if(m_specialItems.IsFull()) return false;
			if(!m_specialItems.Add(item)) return false;
			scene.RemoveItem(item);
			break;
		case ITEM_PLACEMENT_POUCH:
		default:
			return false;
	}
	RecomputeState();
	return true;
}

bool Character::PickUpGold(Scene& scene)
{
	if(scene.GetGoldCount() >= 0)
	{
		wxInt32 rest = AddGold(scene.GetGoldCount());
		return scene.SetGold(rest);
	}
	return false;
}

bool Character::LoseItem(EItem item)
{
	switch(m_pResMgr->GetItemAndDiscMgr().GetItem(item).m_placement)
	{
		case ITEM_PLACEMENT_BACKPACK:
			if(!m_backpack.Contains(item)) return false;
			if(!m_backpack.Remove(item)) return false;
			break;			
		case ITEM_PLACEMENT_WEAPON:
			if(!m_weapons.Contains(item)) return false;
			if(!m_weapons.Remove(item)) return false;
			break;
		case ITEM_PLACEMENT_BODY_HEAD:
			if(m_body.GetHeadItem() != item) return false;
			m_body.SetHeadItem(ITEM_UNKNOWN);
			break;
		case ITEM_PLACEMENT_BODY_TORSO:
			if(m_body.GetTorsoItem() != item) return false;
			m_body.SetTorsoItem(ITEM_UNKNOWN);
			break;
		case ITEM_PLACEMENT_BODY_BACK:
			if(m_body.GetBackItem() != item) return false;
			if(item == m_pResMgr->GetItemAndDiscMgr().GetBackpackType())
			{
				m_backpack.Clear();
			}
			m_body.SetBackItem(ITEM_UNKNOWN);
			break;
		case ITEM_PLACEMENT_SPECIAL:
			if(!m_specialItems.Contains(item)) return false;
			if(!m_specialItems.Remove(item)) return false;
			break;
		case ITEM_PLACEMENT_POUCH:
		default:
			return false;
	}
	RecomputeState();
	return true;
}

bool Character::AddItem(EItem item)
{	
	switch(m_pResMgr->GetItemAndDiscMgr().GetItem(item).m_placement)
	{
		case ITEM_PLACEMENT_BACKPACK:
			if(m_backpack.IsFull()) return false;
			if(!m_backpack.Add(item)) return false;
			break;			
		case ITEM_PLACEMENT_WEAPON:
			if(m_weapons.IsFull()) return false;
			if(!m_weapons.Add(item)) return false;
			break;
		case ITEM_PLACEMENT_BODY_HEAD:
			if(m_body.GetHeadItem() != ITEM_UNKNOWN) return false;
			m_body.SetHeadItem(item);
			break;
		case ITEM_PLACEMENT_BODY_TORSO:
			if(m_body.GetTorsoItem() != ITEM_UNKNOWN) return false;
			m_body.SetTorsoItem(item);
			break;
		case ITEM_PLACEMENT_BODY_BACK:
			if(m_body.GetBackItem() != ITEM_UNKNOWN) return false;
			m_body.SetBackItem(item);
			break;
		case ITEM_PLACEMENT_SPECIAL:
			if(m_specialItems.IsFull()) return false;
			if(!m_specialItems.Add(item)) return false;
			break;
		case ITEM_PLACEMENT_POUCH:
		default:
			return false;
	}
	RecomputeState();
	return true;
}





