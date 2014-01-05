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

bool Character::ApplyEvent(Event& event)
{
	if((event.m_property.m_neededSkill != DISCIPLINE_UNKNOWN)&&(!m_disciplines.Contains(event.m_property.m_neededSkill)))
	{
		// you dont have needed skill
		return true;
	}
	
	if(((event.m_property.m_neededItem != ITEM_UNKNOWN))&&(!ContainsItem(event.m_property.m_neededItem)))
	{
		// you dont have needed item
		return true;
	}
	
	if((event.m_property.m_cancelSkill != DISCIPLINE_UNKNOWN)&&(m_disciplines.Contains(event.m_property.m_cancelSkill)))
	{
		// you have cancel skill - break event
		return true;
	}
	
	if(((event.m_property.m_cancelItem != ITEM_UNKNOWN))&&(ContainsItem(event.m_property.m_cancelItem)))
	{
		// you have cancel item - you use it for stopping this event
		if(m_pResMgr->GetItemAndDiscMgr().GetItem(event.m_property.m_cancelItem).m_properties.m_oneUse)
		{
			LoseItem(event.m_property.m_cancelItem);
		}
		return true;
	}
	
	m_actualAttackSkill += event.m_property.m_actualAttack;	
	m_actualCondition += event.m_property.m_actualCond;
	m_baseAttackSkill += event.m_property.m_baseAttack;
	m_baseCondition += event.m_property.m_baseCond;
	AddGold(event.m_property.m_goldCount);
	
	return true;
	
}

void Character::ApplySkills(Character& enemyCharacter)
{
	CharacterDisciplines::Iterator iter;
	for(iter = m_disciplines.Begin(); iter != m_disciplines.End(); iter++)
	{
		if(!iter->second.m_fightSkill)
		{
			// skill is not useable in fight
			continue;
		}
		if(((iter->second.m_neededItem != ITEM_UNKNOWN))&&(!ContainsItem(iter->second.m_neededItem)))
		{
			// you dont have needed item
			continue;
		}
		
		if((iter->second.m_cancelSkill != DISCIPLINE_UNKNOWN)&&(enemyCharacter.m_disciplines.Contains(iter->second.m_cancelSkill)))
		{
			// enemy cancel your skill
			continue;
		}
		
		if(((iter->second.m_cancelItem != ITEM_UNKNOWN))&&(enemyCharacter.ContainsItem(iter->second.m_cancelItem)))
		{
			// enemy cancel your skill by item
			continue;
		}
		
		m_actualAttackSkill += iter->second.m_actualAttack;
		m_actualCondition += iter->second.m_actualCond;
	}
}

void Character::ApplySkills()
{
	CharacterDisciplines::Iterator iter;
	for(iter = m_disciplines.Begin(); iter != m_disciplines.End(); iter++)
	{
		if(iter->second.m_fightSkill)
		{
			// skill is useable in fight only
			continue;
		}
		if(((iter->second.m_neededItem != ITEM_UNKNOWN))&&(!ContainsItem(iter->second.m_neededItem)))
		{
			// you dont have needed item
			continue;
		}
		
		if((iter->second.m_cancelSkill != DISCIPLINE_UNKNOWN)&&(enemyCharacter.m_disciplines.Contains(iter->second.m_cancelSkill)))
		{
			// enemy cancel your skill
			continue;
		}
		
		if(((iter->second.m_cancelItem != ITEM_UNKNOWN))&&(enemyCharacter.ContainsItem(iter->second.m_cancelItem)))
		{
			// enemy cancel your skill by item
			continue;
		}
		
		m_actualAttackSkill += iter->second.m_actualAttack;
		m_actualCondition += iter->second.m_actualCond;
	}
}
