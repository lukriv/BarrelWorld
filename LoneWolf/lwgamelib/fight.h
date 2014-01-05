#ifndef __LONE_WOLF_FIGHT_H__
#define __LONE_WOLF_FIGHT_H__


#include <wx/defs.h>

struct FightTable {
	wxInt32 m_enemyWound;
	wxInt32 m_charWound;
};

static const FightTable minus11[10] = 
{
	{ 0,-1000},	{ 0,-1000}, { 0,-8}, { 0,-8}, {-1,-7}, {-2,-6},	{-3,-5}, {-4,-4}, {-5,-3}, {-6, 0}
};

static const FightTable minus9[10] = 
{
	{ 0,-1000},	{ 0,-8}, { 0,-7}, {-1,-7}, {-2,-6}, {-3,-6}, {-4,-5}, {-5,-4}, {-6,-3}, {-7, 0}
};

static const FightTable minus7[10] = 
{
	{ 0,-8}, { 0,-7}, {-1,-6}, {-2,-6}, {-3,-5}, {-4,-5}, {-5,-4}, {-6,-3}, {-7,-2}, {-8, 0}
};

static const FightTable minus5[10] = 
{
	{ 0,-6}, {-1,-6}, {-2,-5}, {-3,-5}, {-4,-4}, {-5,-4}, {-6,-3}, {-7,-2}, {-8, 0}, {-9, 0}
};

static const FightTable minus3[10] = 
{
	{ -1,-6}, {-2,-5}, {-3,-5}, {-4,-4}, {-5,-4}, {-6,-3}, {-7,-2}, {-8,-1}, {-9, 0}, {-10, 0}
};

static const FightTable minus1[10] = 
{
	{ -2,-5}, {-3,-5}, {-4,-4}, {-5,-4}, {-5,-3}, {-7,-2}, {-8,-2}, {-9,-1}, {-10, 0}, {-11, 0}
};

static const FightTable zero0[10] = 
{
	{ -3,-5}, {-4,-4}, {-5,-4}, {-6,-3}, {-7,-2}, {-8,-2}, {-9,-1}, {-10, 0}, {-11, 0}, {-12, 0}
};

static const FightTable plus2[10] = 
{
	{ -4,-5}, {-5,-4}, {-6,-3}, {-7,-3}, {-8,-2}, {-9,-2}, {-10,-1}, {-11, 0}, {-12, 0}, {-14, 0}
};

static const FightTable plus4[10] = 
{
	{ -5,-4}, {-6,-3}, {-7,-3}, {-8,-2}, {-9,-2}, {-10,-2}, {-11,-1}, {-12, 0}, {-14, 0}, {-16, 0}
};

static const FightTable plus6[10] = 
{
	{ -6,-4}, {-7,-3}, {-8,-3}, {-9,-2}, {-10,-2}, {-11,-1}, {-12, 0}, {-14, 0}, {-16, 0}, {-18, 0}
};

static const FightTable plus8[10] = 
{
	{ -7,-4}, {-8,-3}, {-9,-2}, {-10,-2}, {-11,-2}, {-12,-1}, {-14, 0}, {-16, 0}, {-18, 0}, {-1000, 0}
};

static const FightTable plus10[10] = 
{
	{ -8,-3}, {-9,-3}, {-10,-2}, {-11,-2}, {-12,-2}, {-14,-1}, {-16, 0}, {-18, 0}, {-1000, 0}, {-1000, 0}
};

static const FightTable plus11[10] = 
{
	{ -9,-3}, {-10,-2}, {-11,-2}, {-12,-2}, {-14,-1}, {-16,-1}, {-18, 0}, {-1000, 0}, {-1000, 0}, {-1000, 0}
};

static const wxInt32 TURNS_INFINITE = -1;

enum EFightResult {
	FIGHT_RESULT_NOT_STARTED	= 0,
	FIGHT_RESULT_WIN			= 1,
	FIGHT_RESULT_RETREAT		= 2,
	FIGHT_RESULT_LOST			= 3	
};

class ActionFight : public ActionBase {
	typedef std::list<Character> TEnemyList;
	typedef std::list<Event> TEventList;
private:
	wxInt32 m_turnNumber;
	wxInt32 m_winTarget;
	wxInt32 m_loseTarget;
	wxInt32 m_retreatTarget;
	wxInt32 m_turnsToWin;
	wxInt32 m_turnsToRetreat;
	FightTable m_lastTurnHits;
	EFightResult m_fightResult;
	bool m_retreat;
	TEnemyList m_enemies;
	TEventList m_fightEvents;
	
	Character* m_pLoneWolf;
	
public:
	ActionFight() : m_turnNumber(0)
					, m_winTarget(TARGET_UNKNOWN)
					, m_loseTarget(TARGET_UNKNOWN)
					, m_retreatTarget(TARGET_UNKNOWN)
					, m_turnsToWin(TURNS_INFINITE)
					, m_turnsToRetreat(TURNS_INFINITE)
					, m_fightResult(FIGHT_RESULT_NOT_STARTED)
					, m_retreat(false)
					, m_pLoneWolf(NULL)
	{
		m_lastTurnHits.m_charWound = 0;
		m_lastTurnHits.m_enemyWound = 0;
	}
	
	virtual EActionType GetType() { return ACTION_FIGHT; }
	
	bool AddEnemy(Character &enemy);
	void GetEnemyList(wxVector<Character> &enemyList);
	bool AddEvent(Event& event);
	
	/*!
	 * \brief Start fight
	 * 
	 * Between 
	 * 
	 * \param loneWolf
	 * \param callback
	 * \return Next target
	 */
	wxInt32 StartFight(Character &loneWolf, LWGameEngineCallback *callback);
	
	/*!
	 * \brief Set retreat flag
	 * 
	 * In the next turn Lone Wolf retreats from the fight
	 * 
	 * \retval true Retreat flag was successfully set
	 * \retval false Retreat is not posible in this fight
	 */
	bool Retreat();
	
	Character* GetLoneWolfCharacter() { return m_pLoneWolf; }
	Character* GetActualEnemy() 
	{
		if(m_enemies.empty()) return NULL;
		return &(m_enemies.front()); 
	}
	
	wxInt32 GetActualTurn() {return m_turnNumber; }
	wxInt32 GetLastLoneWolfHits() { return m_lastTurnHits.m_charWound; }
	wxInt32 GetLastEnemyHits() { return m_lastTurnHits.m_enemyWound; }
	
	wxInt32 GetTurnsToRetreat() { return m_turnsToRetreat; }
	wxInt32 GetTurnsToWin() { return m_turnsToWin; }
	
	EFightResult GetFightResult() { return m_fightResult; }
	wxInt32 GetNextTarget();
	
};





#endif //__LONE_WOLF_FIGHT_H__