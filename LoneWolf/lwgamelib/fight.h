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





class FightingCharacter {
	wxInt32 m_actualAttackSkill;
	wxInt32 m_actualConditions;
public:
	inline wxInt32 GetActualAttackSkill() { return m_actualAttackSkill; }
	inline wxInt32 GetActualConditions() { return m_actualConditions; }
	
	void Attack(wxInt32 attack);
};

#endif //__LONE_WOLF_FIGHT_H__