#include <stdio.h>
#include <iostream>
#include <string>

#include <wx/init.h>
#include "../lwgamelib/scenemgr.h"
#include "../lwgamelib/reader.h"
#include "../lwgamelib/lwengine.h"
#include "textConversion.h"
#include "gres.h"
#include "callbackimpl.h"

//#include <windows.h>


using namespace std;



void WriteScene(Scene& scene)
{
	wxString desc;
	wxString act;
	wxVector<wxDword> posibleActions;
	Scene::TItemList listOfItems;
	Scene::TArticleVector listOfGoods;
	
	scene.GetPosibleActions(g_gameEngine.GetMainCharacter(), posibleActions);
	scene.GetItemList(listOfItems);
	
	desc = scene.m_desc;
	ConvertToNonDiacriticsCsText(desc);
	ConvertToNonBreakText(desc);
	ConvertToBasicFormatedText(desc);
	cout << "\n *** " << endl;
	cout << desc.c_str() << endl;
	if(!listOfItems.empty()||(scene.GetGoldCount() > 0))
	{
		cout << "Predmety: ";
		for(Scene::TItemList::iterator iter = listOfItems.begin(); iter != listOfItems.end(); iter++)
		{
			desc = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(*iter).m_title;
			ConvertToNonDiacriticsCsText(desc);
			cout << desc.c_str() << "(" << *iter << ")" << ", ";
		}
		
		if(scene.GetGoldCount() > 0)
		{
			cout << scene.GetGoldCount() << " Zlatych, ";
		}
		
		cout << endl;
	}
	
	scene.GetItemToBuyList(listOfGoods);
	if(!listOfGoods.empty())
	{
		cout << "Prodej: " << endl;
		for(Scene::TArticleVector::iterator iter = listOfGoods.begin(); iter != listOfGoods.end(); iter++)
		{
			desc = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(iter->m_item).m_title;
			ConvertToNonDiacriticsCsText(desc);
			cout << desc.c_str() << "(" << iter->m_item << ")" << " - " << iter->m_goldCount << " Zlatych " << endl;
		}
		cout << endl;
	}
	
	listOfGoods.clear();
	scene.GetItemToSellList(listOfGoods);
	if(!listOfGoods.empty())
	{
		cout << "Nakup: " << endl;
		for(Scene::TArticleVector::iterator iter = listOfGoods.begin(); iter != listOfGoods.end(); iter++)
		{
			desc = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(iter->m_item).m_title;
			ConvertToNonDiacriticsCsText(desc);
			cout << desc.c_str() << "(" << iter->m_item << ")" << " - " << iter->m_goldCount << " Zlatych " << endl;
		}
		cout << endl;
	}
	
	if((posibleActions.empty() || g_gameEngine.GetMainCharacter().GetActualConditions() <= 0))
	{
		cout << "Tva cesta zde skoncila, jsi mrtev! (restart)" << endl;
	} else {
		for (wxVector<wxDword>::iterator iter = posibleActions.begin(); iter != posibleActions.end(); iter++)
		{
			switch(scene.m_actions[*iter]->GetType())
			{
				case ACTION_CREATE_CHAR:
					cout << (*iter)+1 << ") vytvorit postavu" << endl;
					break;
				case ACTION_MOVE:
				{
					Action* pAction = static_cast<Action*>(scene.m_actions[*iter]);
					act.Printf(wxT("%d) %s"), (*iter)+1, pAction->GetDesc().c_str());
					ConvertToNonDiacriticsCsText(act);
					cout << act.c_str() << endl;
					break;
				}
				case ACTION_LOTERY:
					cout << (*iter)+1 << ") lotery" << endl;
					break;
				case ACTION_FIGHT:
				{
					wxString enemyName;
					wxVector<Character> enemies;
					ActionFight *pActionFight = static_cast<ActionFight*>(scene.m_actions[*iter]);
					pActionFight->GetEnemyList(enemies);
					cout << (*iter)+1 << ") Zautoc na nepritele ";
					for(wxVector<Character>::iterator iter = enemies.begin(); iter != enemies.end(); iter++)
					{
						enemyName = iter->GetCharacterName();
						ConvertToNonDiacriticsCsText(enemyName);
						cout << enemyName << "(ut=" << iter->GetActualAttackSkill() << "|kb=" <<  iter->GetActualConditions() << "); ";
					}
					cout << endl;
					break;
				}
				default:
					cout << "Unknown action - ERROR!!!" << endl;
					break;
			}
		}
	}
}

void WriteCharacterState(Character& character)
{
	wxString title;
	
	cout << " *** Stav postavy *** " << endl;
	cout << " Discipliny a umeni (" << character.GetDisciplines().Size() << "): ";
	for (CharacterDisciplines::Iterator iter = character.GetDisciplines().Begin()
			; iter != character.GetDisciplines().End()
			; iter++)
	{
		title = g_gameEngine.GetResMgr().GetDisciplineMgr().GetDiscipline(iter->first).m_title;
		ConvertToNonDiacriticsCsText(title);
		if( character.GetDisciplines().FindValue(iter->first) != NULL)
		{
			if( character.GetDisciplines().FindValue(iter->first)->m_neededItem != ITEM_UNKNOWN)
			{
				wxString weapon = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(character.GetDisciplines().FindValue(iter->first)->m_neededItem).m_title;
				ConvertToNonDiacriticsCsText(weapon);
				cout << title.c_str() << " (" << weapon.c_str() << "), ";
			} else {
				cout << title.c_str() << ", ";
			}
		} else {
			cout << "Chyba v disciplinach postavy" << endl;
		}
		
	}
	cout << endl;
	
	cout << " Kondice (zakladni kondice " << character.GetBaseConditions() << "): " 
		<< character.GetActualConditions() << "/" << character.GetMaxConditions() << endl;
	cout << " Zakladni umeni boje: " << character.GetBaseAttackSkill() << endl;
	
	cout << " Batoh (max. " <<  character.GetBackPack().GetMaxCount() << "): " << endl;
	cout << "   ";
	for (Character::CharacterBackpack::Iterator iter = character.GetBackPack().Begin()
			; iter != character.GetBackPack().End()
			; iter++)
	{
		title = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(*iter).m_title;
		ConvertToNonDiacriticsCsText(title);
		cout << title.c_str() << "(" << g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(*iter).m_id << ")" << ", ";
	}
	cout << endl;
	
	cout << " Zbrane: ";
	for (Character::CharacterWeapons::Iterator iter = character.GetWeapons().Begin()
			; iter != character.GetWeapons().End()
			; iter++)
	{
		title = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(*iter).m_title;
		ConvertToNonDiacriticsCsText(title);
		cout << title.c_str() << "(" << g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(*iter).m_id << ")" << ", ";
	}
	cout << endl;
	
	title = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(character.GetBody().GetHeadItem()).m_title;
	ConvertToNonDiacriticsCsText(title);
	cout << " Hlava: ";
	if(character.GetBody().GetHeadItem() != ITEM_UNKNOWN)
	{
		cout << title.c_str() << "(" << g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(character.GetBody().GetHeadItem()).m_id << ")" << endl;	
	} else {
		cout << " -- " << endl;
	}
	
	title = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(character.GetBody().GetTorsoItem()).m_title;
	ConvertToNonDiacriticsCsText(title);
	cout << " Torso: " ;
	if(character.GetBody().GetTorsoItem() != ITEM_UNKNOWN)
	{
		cout << title.c_str() << "(" << g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(character.GetBody().GetTorsoItem()).m_id << ")" << endl;
	} else {
		cout << " -- " << endl;
	}
	
	cout << " Specialni predmety: ";
	for (Character::CharacterSpecialItems::Iterator iter = character.GetSpecialItems().Begin()
			; iter != character.GetSpecialItems().End()
			; iter++)
	{
		title = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(*iter).m_title;
		ConvertToNonDiacriticsCsText(title);
		cout << title.c_str() << "(" << g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(*iter).m_id << ")" << ", ";
	}
	cout << endl;
	cout << " Mesec (max. " << character.GetMaxGoldCount() << " Zlatych): " << character.GetGoldCount() << " Zlatych" << endl;
	cout << " **********" << endl;
	
						
}

bool PickUpItem(wxString &command, Scene& scene, Character& character)
{
	long int itemValue = 0;
	EItem item = ITEM_UNKNOWN;
	wxString title;
	if(command.Cmp(wxT("gold")) == 0)
	{
		// pick up all gold you can
		if(scene.GetGoldCount() > 0)
		{
			cout << "Sebral jsi " << character.PickUpGold(scene) << " Zlatych" << endl;
		}
		
		return true;
	}
	command.ToLong(&itemValue);
	
	item = static_cast<EItem>(itemValue);
	
	if(!g_gameEngine.GetResMgr().GetItemAndDiscMgr().ItemExists(item))
	{
		cout << "Predmet pod cislem (" << item << ") neexistuje" << endl;
		return false;	
	}
	
	title = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(item).m_title;
	ConvertToNonDiacriticsCsText(title);
	
	if((item != 0)&&(scene.ContainsItem(item)))
	{
		if(!character.PickUpItem(item, scene))
		{
			cout << "Predmet (" << title.c_str() << ") se nepodarilo zvednout" << endl;
			return false;
		} else {
			cout << "Sebral jsi predmet (" << title.c_str() << ")" << endl;
		}
	} else {
		cout << "Predmet (" << title.c_str() << ") neni ve scene dostupny" << endl;
		return false;	
	}
	
	return true;
}

bool DropItem(wxString &command, Scene& scene, Character& character)
{
	long int itemValue = 0;
	EItem item = ITEM_UNKNOWN;
	wxString title;
	command.ToLong(&itemValue);
	
	item = static_cast<EItem>(itemValue);
	
	if(!g_gameEngine.GetResMgr().GetItemAndDiscMgr().ItemExists(item))
	{
		cout << "Predmet pod cislem (" << item << ") neexistuje" << endl;
		return false;	
	}
	
	title = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(item).m_title;
	ConvertToNonDiacriticsCsText(title);
	
	if((item != 0)&&(character.ContainsItem(item)))
	{
		if(!character.DropItem(item, scene))
		{
			cout << "Predmet (" << title.c_str() << ") se nepodarilo polozit" << endl;
			return false;
		} else {
			cout << "Polozil jsi predmet (" << title.c_str() << ")" << endl;
		}
	} else {
		cout << "Predmet (" << title.c_str() << ") nemas u sebe" << endl;
		return false;	
	}
	
	return true;
}

bool UseItem(wxString &command, Character& character)
{
	long int itemValue = 0;
	EItem item = ITEM_UNKNOWN;
	wxString title;
	command.ToLong(&itemValue);
	
	item = static_cast<EItem>(itemValue);
	
	if(!g_gameEngine.GetResMgr().GetItemAndDiscMgr().ItemExists(item))
	{
		cout << "Predmet pod cislem (" << item << ") neexistuje" << endl;
		return false;	
	}
	
	title = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(item).m_title;
	ConvertToNonDiacriticsCsText(title);
	
	if((item != 0)&&(character.ContainsItem(item)))
	{
		if(!character.UseItem(item))
		{
			cout << "Predmet (" << title.c_str() << ") nelze pouzit" << endl;
			return false;
		} else {
			cout << "Pouzil jsi predmet (" << title.c_str() << ")" << endl;
		}
	} else {
		cout << "Predmet (" << title.c_str() << ") nemas u sebe" << endl;
		return false;	
	}
	
	return true;
}

bool BuyItem(wxString &command, Scene& scene, Character& character )
{
	long int itemValue = 0;
	EItem item = ITEM_UNKNOWN;
	wxString title;
	command.ToLong(&itemValue);
	
	item = static_cast<EItem>(itemValue);
	
	if(!g_gameEngine.GetResMgr().GetItemAndDiscMgr().ItemExists(item))
	{
		cout << "Predmet pod cislem (" << item << ") neexistuje" << endl;
		return false;	
	}
	
	title = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(item).m_title;
	ConvertToNonDiacriticsCsText(title);
	
	if((item != 0)&&(scene.ContainsItemToBuy(item)))
	{
		if(!character.BuyItem(item, scene))
		{
			cout << "Predmet (" << title.c_str() << ") nelze zakoupit (nemas dostatek penez nebo ho uz neuneses)" << endl;
			return false;
		} else {
			cout << "Zakoupil jsi predmet (" << title.c_str() << ")" << endl;
		}
	} else {
		cout << "Predmet (" << title.c_str() << ") neni ve scene na prodej" << endl;
		return false;	
	}
	
	return true;
}

bool SellItem(wxString &command, Scene& scene, Character& character )
{
	long int itemValue = 0;
	EItem item = ITEM_UNKNOWN;
	wxString title;
	command.ToLong(&itemValue);
	
	item = static_cast<EItem>(itemValue);
	
	if(!g_gameEngine.GetResMgr().GetItemAndDiscMgr().ItemExists(item))
	{
		cout << "Predmet pod cislem (" << item << ") neexistuje" << endl;
		return false;	
	}
	
	title = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(item).m_title;
	ConvertToNonDiacriticsCsText(title);
	
	if((item != 0)&&(scene.ContainsItemToSell(item)))
	{
		if(!character.SellItem(item, scene))
		{
			cout << "Predmet (" << title.c_str() << ") nelze prodat (predmet nemas v inventari)" << endl;
			return false;
		} else {
			cout << "Prodal jsi predmet (" << title.c_str() << ")" << endl;
		}
	} else {
		cout << "Predmet (" << title.c_str() << ") ve scene nelze prodat" << endl;
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
	
	if(!g_gameEngine.NewGame())
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
		
		cout << "Zadej cislo akce nebo prikaz (exit, char, scene, pickup, drop, use, buy, sell): " << endl;
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
				WriteCharacterState(g_gameEngine.GetMainCharacter());
				continue;
			}
			
			if(act.Cmp(wxT("scene")) == 0)
			{
				WriteScene(*g_gameEngine.GetActualScene());
				continue;
			}
			
			if(act.Cmp(wxT("pickup")) == 0)
			{
				cout << "Zadej cislo predmetu ze sceny pro zvednuti nebo 'gold' pro zvednuti zlata: ";
				cout.flush();
				cin >> input;
				act = input;
				if(PickUpItem(act, *g_gameEngine.GetActualScene(), g_gameEngine.GetMainCharacter()))
				{
					WriteScene(*g_gameEngine.GetActualScene());
				}
				continue;
			}	
			
			if(act.Cmp(wxT("drop")) == 0)
			{
				cout << "Zadej cislo predmetu z batohu pro polozeni: ";
				cout.flush();
				cin >> input;
				act = input;
				if(DropItem(act, *g_gameEngine.GetActualScene(), g_gameEngine.GetMainCharacter()))
				{
					WriteScene(*g_gameEngine.GetActualScene());
				}
				continue;
			}
			
			if(act.Cmp(wxT("use")) == 0)
			{
				cout << "Zadej cislo predmetu z batohu pro pouziti: ";
				cout.flush();
				cin >> input;
				act = input;
				if(UseItem(act, g_gameEngine.GetMainCharacter()))
				{
					WriteCharacterState(g_gameEngine.GetMainCharacter());
				}
				continue;
			}
			
			if(act.Cmp(wxT("buy")) == 0)
			{
				cout << "Zadej cislo kupovaneho predmetu: ";
				cout.flush();
				cin >> input;
				act = input;
				if(BuyItem(act, *g_gameEngine.GetActualScene(), g_gameEngine.GetMainCharacter()))
				{
					WriteScene(*g_gameEngine.GetActualScene());
				}
				continue;
			}	
			
			if(act.Cmp(wxT("sell")) == 0)
			{
				cout << "Zadej cislo prodavaneho predmetu z inventare: ";
				cout.flush();
				cin >> input;
				act = input;
				if(SellItem(act, *g_gameEngine.GetActualScene(), g_gameEngine.GetMainCharacter()))
				{
					WriteScene(*g_gameEngine.GetActualScene());
				}
				continue;
			}

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
