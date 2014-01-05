#include "fight.h"


//----------------------------------------------------------------
//----------------------FightingCharacter-------------------------
//----------------------------------------------------------------



bool ActionFight::AddEnemy(Character& enemy)
{
	m_enemies.push_back(enemy);
}

bool ActionFight::AddEvent(Event& event)
{
	m_fightEvents.push_back(event);
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
	if((m_retreatTarget > TARGET_UNKNOWN)&&(m_turnsToRetreat == 0))
	{
		m_retreat = true;
		return true;
	} 
	
	return false;
}

wxInt32 ActionFight::StartFight(Character& loneWolf, LWGameEngineCallback* callback)
{
}
