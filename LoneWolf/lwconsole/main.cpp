#include <stdio.h>
#include <iostream>
#include <string>

#include <wx/init.h>
#include "../lwgamelib/scenemgr.h"
#include "../lwgamelib/reader.h"
#include "../lwgamelib/lwengine.h"
#include "textConversion.h"

#include <windows.h>


using namespace std;

LWGameEngine g_gameEngine;

class ConsoleGameCallback : public LWGameEngineCallback {
	void WriteDisciplineList(const std::set<EDisciplines> &chosenDisc)
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
public:
	virtual void SelectDisciplines(wxDword discToSelect, std::set<EDisciplines> &chosenDisc)
	{
		
		wxInt32 input = 0;
		
		while (chosenDisc.size() < discToSelect)
		{
			WriteDisciplineList(chosenDisc);
			while((input == 0)
					||(!g_gameEngine.GetResMgr().GetDisciplineMgr().DisciplineExists(static_cast<EDisciplines>(input)))
					||(chosenDisc.find(static_cast<EDisciplines>(input)) != chosenDisc.end()))
			{
				cout << "Zadej cislo discipliny z vyberu (zbyva vybrat " << (discToSelect - chosenDisc.size()) << " disciplin): " << endl;
				cin >> input;
				cout << endl;
			}
			
			chosenDisc.insert(static_cast<EDisciplines>(input));
		}
		
	}
};

void WriteScene(Scene& scene)
{
	wxString desc;
	wxString act;
	wxVector<wxDword> posibleActions;
	Scene::TItemList listOfItems;
	
	scene.GetPosibleActions(g_gameEngine.GetMainCharacter(), posibleActions);
	scene.GetItemList(listOfItems);
	
	desc = scene.m_desc;
	ConvertToNonDiacriticsCsText(desc);
	ConvertToNonBreakText(desc);
	ConvertToBasicFormatedText(desc);
	cout << "\n *** " << endl;
	cout << desc.c_str() << endl;
	cout << "Veci: ";
	for(Scene::TItemList::iterator iter = listOfItems.begin(); iter != listOfItems.end(); iter++)
	{
		desc = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(*iter).m_title;
		ConvertToNonDiacriticsCsText(desc);
		cout << desc.c_str() << "(" << *iter << ")" << ", ";
	}
	cout << endl;
	for (wxVector<wxDword>::iterator iter = posibleActions.begin(); iter != posibleActions.end(); iter++)
	{
		switch(scene.m_actions[*iter].GetType())
		{
			case ACTION_CREATE_CHAR:
				cout << (*iter)+1 << ") vytvorit postavu" << endl;
				break;
			case ACTION_MOVE:
				act.Printf(wxT("%d) %s"), (*iter)+1, scene.m_actions[*iter].GetDesc().c_str());
				ConvertToNonDiacriticsCsText(act);
				cout << act.c_str() << endl;
				break;
			case ACTION_LOTERY:
				cout << (*iter)+1 << ") lotery" << endl;
				break;
			default:
				cout << "Unknown action - ERROR!!!" << endl;
				break;
		}
	}
}

void WriteCharacterState()
{
	wxString title;
	
	cout << " *** Stav postavy *** " << endl;
	cout << " Discipliny a umeni(" << g_gameEngine.GetMainCharacter().GetDisciplines().Size() << "): ";
	for (CharacterDisciplines::Iterator iter = g_gameEngine.GetMainCharacter().GetDisciplines().Begin()
			; iter != g_gameEngine.GetMainCharacter().GetDisciplines().End()
			; iter++)
	{
		title = g_gameEngine.GetResMgr().GetDisciplineMgr().GetDiscipline(iter->first).m_title;
		ConvertToNonDiacriticsCsText(title);
		if( g_gameEngine.GetMainCharacter().GetDisciplines().FindValue(iter->first) != NULL)
		{
			if( g_gameEngine.GetMainCharacter().GetDisciplines().FindValue(iter->first)->m_neededItem != ITEM_UNKNOWN)
			{
				wxString weapon = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(g_gameEngine.GetMainCharacter().GetDisciplines().FindValue(iter->first)->m_neededItem).m_title;
				ConvertToNonDiacriticsCsText(weapon);
				cout << title.c_str() << " (" << weapon.c_str() << "), ";
			}
		} else {
			cout << title.c_str() << ", ";
		}
		
	}
	cout << endl;
	
	cout << " Kondice: " << g_gameEngine.GetMainCharacter().GetActualConditions() << "/" 
						<< g_gameEngine.GetMainCharacter().GetMaxConditions() 
						<< " (" << g_gameEngine.GetMainCharacter().GetBaseConditions() << ")" << endl;
	cout << " Umeni boje: " << " (" << g_gameEngine.GetMainCharacter().GetBaseAttackSkill() << ")" << endl;
	
	cout << " Batoh (max. " <<  g_gameEngine.GetMainCharacter().GetBackPack().GetMaxCount() << "): " << endl;
	cout << "   ";
	for (Character::CharacterBackpack::Iterator iter = g_gameEngine.GetMainCharacter().GetBackPack().Begin()
			; iter != g_gameEngine.GetMainCharacter().GetBackPack().End()
			; iter++)
	{
		title = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(*iter).m_title;
		ConvertToNonDiacriticsCsText(title);
		cout << title.c_str() << "(" << g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(*iter).m_id << ")" << ", ";
	}
	cout << endl;
	
	cout << " Zbrane: ";
	for (Character::CharacterWeapons::Iterator iter = g_gameEngine.GetMainCharacter().GetWeapons().Begin()
			; iter != g_gameEngine.GetMainCharacter().GetWeapons().End()
			; iter++)
	{
		title = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(*iter).m_title;
		ConvertToNonDiacriticsCsText(title);
		cout << title.c_str() << "(" << g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(*iter).m_id << ")" << ", ";
	}
	cout << endl;
	
	title = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(g_gameEngine.GetMainCharacter().GetBody().GetHeadItem()).m_title;
	ConvertToNonDiacriticsCsText(title);
	cout << " Hlava: " << title.c_str() << endl;
	title = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(g_gameEngine.GetMainCharacter().GetBody().GetTorsoItem()).m_title;
	ConvertToNonDiacriticsCsText(title);
	cout << " Torso: " << title.c_str() << endl;
	
	cout << " Specialni predmety: ";
	for (Character::CharacterSpecialItems::Iterator iter = g_gameEngine.GetMainCharacter().GetSpecialItems().Begin()
			; iter != g_gameEngine.GetMainCharacter().GetSpecialItems().End()
			; iter++)
	{
		title = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(*iter).m_title;
		ConvertToNonDiacriticsCsText(title);
		cout << title.c_str() << "(" << g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(*iter).m_id << ")" << ", ";
	}
	cout << endl;
	cout << " **********" << endl;
	
						
}

bool PickUpItem(wxString &command, Scene& scene, Character& character)
{
	long int itemValue = 0;
	EItem item = ITEM_UNKNOWN;
	wxString title;
	command.Replace(wxString(wxT("pickup ")),wxString(wxT("")));
	command.ToLong(&itemValue);
	
	item = static_cast<EItem>(itemValue);
	
	if(!g_gameEngine.GetResMgr().GetItemAndDiscMgr().ItemExists(item))
	{
		cout << "Vec pod cislem (" << item << ") neexistuje" << endl;
		return false;	
	}
	
	title = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(item).m_title;
	ConvertToNonDiacriticsCsText(title);
	
	if((item != 0)&&(scene.ContainsItem(item)))
	{
		if(!character.PickUpItem(item, scene))
		{
			cout << "Vec (" << title.c_str() << ") se nepodarilo zvednout" << endl;
			return false;
		} else {
			cout << "Vec (" << title.c_str() << ") byla uspesne zvednuta" << endl;
		}
	} else {
		cout << "Vec (" << title.c_str() << ") neni ve scene dostupna" << endl;
		return false;	
	}
	
	return true;
}

int main(int argc, char **argv)
{
	bool exit = false;
	std::string input;
	wxString desc;
	wxString act;
	unsigned long int actChoose = 0;
	ConsoleGameCallback myCallback;
	
	//SetConsoleOutputCP(CP_UTF8);
	printf("Start testing lone wolf world\n");
	
	wxInitializer intializer(argc, argv);
	if (!intializer.IsOk())
	{
		wxPrintf(wxT("Failed to initialize the wxWidgets library, aborting."));
		return -1;
	}
	
	if(!g_gameEngine.Initialize(&myCallback))
	{
		cout << g_gameEngine.GetLastErrorString().c_str() << endl;
		return -1;
	}
	
	// main loop
	while(!exit)
	{
		if(g_gameEngine.GetActualScene() != NULL)
		{
			WriteScene(*g_gameEngine.GetActualScene());
		} else {
			cout << "Scene not found" << endl;
			return 0;
		}
		
		cout << "Zadej cislo akce nebo prikaz (exit, char, scene): " << endl;
		while (true)
		{
			cout << "> ";
			cin >> input;
			
			act = input;
			act.ToULong(&actChoose);
			if(act.Cmp(wxT("exit")) == 0)
			{
				exit = true;
				return 0;
			}
			
			if(act.Cmp(wxT("char")) == 0)
			{
				WriteCharacterState();
				continue;
			}
			
			if(act.Cmp(wxT("scene")) == 0)
			{
				WriteScene(*g_gameEngine.GetActualScene());
				continue;
			}
			
			if(act.Contains(wxT("pickup")))
			{
				if(PickUpItem(act, *g_gameEngine.GetActualScene(), g_gameEngine.GetMainCharacter()))
				{
					WriteScene(*g_gameEngine.GetActualScene());
				}
				continue;
			}	

			//if(act.Contains(wxT("drop")))
			//{
			//	if(DropItem(act, *g_gameEngine.GetActualScene(), g_gameEngine.GetMainCharacter()))
			//	{
			//		WriteScene(*g_gameEngine.GetActualScene());
			//	}
			//	continue;
			//}		
			
			if((actChoose != 0)&&(actChoose <= g_gameEngine.GetActualScene()->m_actions.size()))
			{
				cout << actChoose-1 << endl;
				break;
			} else {
				cout << "Chybny prikaz, zadej akci znovu (exit je pro konec): " << endl;
			}
		}
		
		if(!g_gameEngine.RunAction(static_cast<wxDword>(actChoose-1)))
		{
			cout << g_gameEngine.GetLastErrorString().c_str() << endl;
			break;
		}
		
	}
	
	return 0;
}
