#include "fight.h"
#include "lwengine.h"

//----------------------------------------------------------------
//----------------------FightingCharacter-------------------------
//----------------------------------------------------------------



bool ActionFight::AddEnemy(Character& enemy)
{
	m_enemies.push_back(enemy);
	return true;
}

bool ActionFight::AddEvent(Event& event)
{
	m_fightEvents.push_back(event);
	return true;
}

void ActionFight::GetEnemyList(wxVector<Character>& enemyList)
{
	for (TEnemyList::iterator iter = m_enemies.begin(); iter != m_enemies.end(); iter++)
	{
		enemyList.push_back(*iter);
	}
}

wxInt32 ActionFight::GetNextTarget()
{
	switch(m_fightResult)
	{
		case FIGHT_RESULT_WIN:
			return m_winTarget;
		case FIGHT_RESULT_LOST:
			return m_loseTarget;
		case FIGHT_RESULT_RETREAT:
			return m_retreatTarget;
		default:
			return TARGET_UNKNOWN;
	}
}

bool ActionFight::Retreat()
{
	if((m_retreatTarget > TARGET_UNKNOWN)&&(m_turnsToRetreat != TURNS_INFINITE)&&(m_turnsToRetreat <= m_turnNumber))
	{
		m_retreat = true;
		return true;
	} 
	
	return false;
}

wxInt32 ActionFight::StartFight(Character& loneWolf, LWGameEngineCallback* callback, GlobalResourceManager& resMgr)
{
	wxInt32 resultAttackSkill = 0;
	const FightTable *actualFightTable = NULL;
	m_pLoneWolf = &loneWolf;
	Event barehands;
	barehands.m_property.m_actualAttack = -4;
	barehands.m_property.m_cancelItem = resMgr.GetItemAndDiscMgr().GetRandomWeaponType();
	while((loneWolf.GetActualConditions() > 0 )
			&&(!m_enemies.empty())
			&&(!m_retreat)
			&&((m_turnsToWin >= m_turnNumber)||(m_turnsToWin == TURNS_INFINITE)))
	{
		Character *pEnemy = &(m_enemies.front());
		while((loneWolf.GetActualConditions() > 0)
			&&(pEnemy->GetActualConditions() > 0)
			&&(!m_retreat)
			&&((m_turnsToWin >= m_turnNumber)||(m_turnsToWin == TURNS_INFINITE)))
		{
			// reset attack skill
			loneWolf.ResetActualAttackSkill();
			pEnemy->ResetActualAttackSkill();
			
			// apply fight events to lonewolf
			std::list<Event>::iterator iter = m_fightEvents.begin();
			while( iter != m_fightEvents.end() )
			{
				if(iter->m_property.m_duration == 0)
				{
					// duration of event is equal to 0, remove this event
					iter = m_fightEvents.erase(iter);
					continue;
				}
				if(!loneWolf.ApplyEvent(*iter))
				{
					return TARGET_UNKNOWN;
				}
				
				if(iter->m_property.m_duration != DURATION_INFINITE)
				{
					// decrease duration counter
					iter->m_property.m_duration--;
				}
				
				// go to next event
				iter++;
			}
			//apply barehands event
			if(!loneWolf.ApplyEvent(barehands))
			{
				return TARGET_UNKNOWN;
			}
			
			// apply skills to lonewolf
			loneWolf.ApplySkills(*pEnemy);
			// apply skills to enemy
			pEnemy->ApplySkills(loneWolf);
			
			resultAttackSkill = loneWolf.GetActualAttackSkill() - pEnemy->GetActualAttackSkill();
			if(resultAttackSkill < -10)
			{
				actualFightTable = minus11;
			} else if(resultAttackSkill < -8) {
				actualFightTable = minus9;
			} else if(resultAttackSkill < -6) {
				actualFightTable = minus7;
			} else if(resultAttackSkill < -4) {
				actualFightTable = minus5;
			} else if(resultAttackSkill < -2) {
				actualFightTable = minus3;
			} else if(resultAttackSkill < 0) {
				actualFightTable = minus1;
			} else if(resultAttackSkill < 1) {
				actualFightTable = zero0;
			} else if(resultAttackSkill < 3) {
				actualFightTable = plus2;
			} else if(resultAttackSkill < 5) {
				actualFightTable = plus4;
			} else if(resultAttackSkill < 7) {
				actualFightTable = plus6;
			} else if(resultAttackSkill < 9) {
				actualFightTable = plus8;
			} else if(resultAttackSkill < 11) {
				actualFightTable = plus10;
			} else {
				actualFightTable = plus11;
			}
			
			// call user callback
			callback->FightTurn(*this);
			
			m_lastTurnHits = *(actualFightTable + RandomSpin());
			
			// add condition differences
			loneWolf.AddActualCondition(m_lastTurnHits.m_charWound);
			if(!m_retreat)
			{
				pEnemy->AddActualCondition(m_lastTurnHits.m_enemyWound);
			} else {
				m_lastTurnHits.m_enemyWound = 0;
			}
			
			// increase turn number
			m_turnNumber++;
		}
		
		if(pEnemy->GetActualConditions() <= 0)
		{
			pEnemy = NULL;
			m_enemies.pop_front(); // remove dead enemy
		}
	}
	
	if(m_enemies.empty())
	{
		m_fightResult = FIGHT_RESULT_WIN;
	} else if(m_retreat) {
		m_fightResult = FIGHT_RESULT_RETREAT;
	} else if(m_turnNumber > m_turnsToWin) {
		m_fightResult = FIGHT_RESULT_LOST;
	}
	
	callback->FightFinish(*this);
	
	return GetNextTarget();
}
