#ifndef __LONE_WOLF_GLOBAL_RESOURCES_H__
#define __LONE_WOLF_GLOBAL_RESOURCES_H__



#include "actions.h"
#include "disciplines.h"
#include "items.h"

/*!
 * \class GlobalResourceManager
 * \author Lukas
 * \date 12/29/13
 * \file lwglobres.h
 * \brief Holds all global managers with translations and properties
 */
class GlobalResourceManager {
	// managers
	ActionManager m_actionMgr;
	DisciplinesManager m_disciplineMgr;
	ItemAndWeaponManager m_itemAndWeaponMgr;
public:
	GlobalResourceManager() {}

	inline ActionManager& GetActionMgr() {return m_actionMgr;}
	inline DisciplinesManager& GetDisciplineMgr() {return m_disciplineMgr;}
	inline ItemAndWeaponManager& GetItemAndDiscMgr() {return m_itemAndWeaponMgr;}
};


// global random method
wxInt32 RandomSpin();


#endif //__LONE_WOLF_GLOBAL_RESOURCES_H__