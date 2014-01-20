#ifndef __USER_CALLBACK_IMPLEMENTATION_H__
#define __USER_CALLBACK_IMPLEMENTATION_H__

#include "../lwgamelib/lwengine.h"

class ConsoleGameCallback : public LWGameEngineCallback {
	void WriteDisciplineList(const std::set<EDisciplines> &chosenDisc); 

public:
	virtual void SelectDisciplines(wxDword discToSelect, std::set<EDisciplines> &chosenDisc);
	
	virtual void FightTurn(ActionFight &fight);
	virtual void FightFinish(ActionFight &fight);
};

#endif //__USER_CALLBACK_IMPLEMENTATION_H__