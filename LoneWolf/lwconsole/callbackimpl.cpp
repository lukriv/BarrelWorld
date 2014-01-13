#include "callbackimpl.h"

#include "textConversion.h"
#include "gres.h"
#include "../lwgamelib/fight.h"

using namespace std;


void ConsoleGameCallback::WriteDisciplineList(const std::set<EDisciplines> &chosenDisc)
{
	wxString title, desc;
	cout << "Discipliny k vyberu: " << endl;
	for (DisciplinesManager::Iterator iter = g_gameEngine.GetResMgr().GetDisciplineMgr().begin()
							; iter != g_gameEngine.GetResMgr().GetDisciplineMgr().end()
							; iter++)
	{
		if(chosenDisc.find((*iter).m_id) == chosenDisc.end())
		{
			// if discipline is not chosen yet write this discipline
			title = (*iter).m_title;
			desc = (*iter).m_desc;
			ConvertToNonDiacriticsCsText(title);
			ConvertToNonDiacriticsCsText(desc);
			cout << "  " << static_cast<wxInt32>((*iter).m_id) << ") " << title.c_str() << " [" << desc.c_str() << "]" << endl;
		}
	}
}

void ConsoleGameCallback::SelectDisciplines(wxDword discToSelect, std::set<EDisciplines> &chosenDisc)
{
	wxString disc;
	string input;
	long discId = 0;
	
	while (chosenDisc.size() < discToSelect)
	{
		WriteDisciplineList(chosenDisc);
		while((discId == 0)
				||(!g_gameEngine.GetResMgr().GetDisciplineMgr().DisciplineExists(static_cast<EDisciplines>(discId)))
				||(chosenDisc.find(static_cast<EDisciplines>(discId)) != chosenDisc.end()))
		{
			cout << "Zadej cislo discipliny z vyberu (zbyva vybrat " << (discToSelect - chosenDisc.size()) << " disciplin): " << endl;
			cin >> input;
			cout << endl;
			disc = input;
			disc.ToLong(&discId);
		}
		
		chosenDisc.insert(static_cast<EDisciplines>(discId));
	}
	
}

void ConsoleGameCallback::FightTurn(ActionFight &fight)
{
	std::string input;
	wxString wxInput;
	Character *pChar = NULL;
	if(fight.GetActualTurn() > 0)
	{
		cout << "LW(" << fight.GetLastLoneWolfHits() << "), enemy(" << fight.GetLastEnemyHits() << ")" << endl;
	}	
	cout << " ------ Boj kolo " << fight.GetActualTurn();
	if(fight.GetTurnsToWin() == TURNS_INFINITE)
	{
		cout << ", kol do konce boje(INF)";
	} else {
		wxInt32 toWin = fight.GetTurnsToWin() - fight.GetActualTurn();
		toWin = (toWin < 0)? 0 : toWin;
		cout << ", kol do konce boje(" << toWin << ")";
	}
	
	if(fight.GetTurnsToRetreat() == TURNS_INFINITE)
	{
		cout << ", kol do ustupu(INF) ";
	} else {
		wxInt32 toWin = fight.GetTurnsToRetreat() - fight.GetActualTurn();
		toWin = (toWin < 0)? 0 : toWin;
		cout << ", kol do ustupu(" << toWin << ") ";
	}
	
	cout << "------" << endl;
	pChar = fight.GetActualEnemy();
	if(pChar != NULL)
	{
		cout << "Nepritel (" << pChar->GetCharacterName() << "): "
				<< " utok( " << pChar->GetActualAttackSkill() << "), "
				<< " kondice( " << pChar->GetActualConditions() << ") " << endl;
	}
	
	pChar = fight.GetLoneWolfCharacter();
	if(pChar != NULL)
	{
		cout << "LoneWolf: " 
				<< " utok( " << pChar->GetActualAttackSkill() << "), "
				<< " kondice( " << pChar->GetActualConditions() << ") " << endl;
	}
	cout << "Libovolne pismeno nebo retreat pro ustup > ";
	cin >> input;
	wxInput = input;
	if(wxInput.Cmp(wxT("retreat")) == 0)
	{
		if(fight.Retreat())
		{
			cout << "Ustup!" << endl;
		} else {
			cout << "Ustup neni mozny" << endl;
		}
	}
	
}

void ConsoleGameCallback::FightFinish(ActionFight &fight)
{
	cout << "LW(" << fight.GetLastLoneWolfHits() << "), enemy(" << fight.GetLastEnemyHits() << ")" << endl;
	cout << " ------ Konec boje ------ " << endl;
	if((fight.GetLoneWolfCharacter() != NULL)&&(fight.GetLoneWolfCharacter()->GetActualConditions() > 0))
	{
		switch(fight.GetFightResult())
		{
			case FIGHT_RESULT_WIN:
				cout << "Vitezstvi! Porazil jsi vsechny nepratele!" << endl;
				break;
			case FIGHT_RESULT_LOST:
				cout << "Boj byl ztracen. Nestihl jsi porazit nepratele vcas." << endl;
				break;
			case FIGHT_RESULT_RETREAT:
				cout << "Zvolil jsi ustup pred nepritelem." << endl;
				break;
			default:
				cout << "Chyba ve vysledku boje" << endl;
				break;
		}
	} else {
		cout << "Zemrel jsi. Tvuj protivnik byl nad Tve sili." << endl;
	}
	
	if(fight.GetLoneWolfCharacter() != NULL)
	{
		cout << "LoneWolf: " 
				<< " utok( " << fight.GetLoneWolfCharacter()->GetActualAttackSkill() << "), "
				<< " kondice( " << fight.GetLoneWolfCharacter()->GetActualConditions() << ") " << endl;
	}
	cout << " --------------------------- " << endl;
}
