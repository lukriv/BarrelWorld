#include "commands.h"

#include "../lwgamelib/fight.h"
#include "gres.h"
#include "consolefunc.h"
#include "textConversion.h"


using namespace std;


void WriteListSceneItems(Scene& scene, EItemListType type)
{
	wxString desc;
	Scene::TItemList listOfItems;
	Scene::TArticleVector listOfGoods;
	if(type == ITEM_LIST_SCENE_ITEMS)
	{
		scene.GetItemList(listOfItems);
		if(!listOfItems.empty()||(scene.GetGoldCount() > 0))
		{
			SetFontColor(ITEMS_HEADING); // font color
			cout << "Predmety: ";
			SetFontColor(ITEMS); // font color
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
		return;
	}
	
	if((type == ITEM_LIST_SCENE_BUY)||(type == ITEM_LIST_SCENE_SELL))
	{
		wxString itemsTitle;
		if(type == ITEM_LIST_SCENE_BUY)
		{
			itemsTitle = wxT("Prodej: ");
			scene.GetItemToBuyList(listOfGoods);
		} else {
			itemsTitle = wxT("Nakup: ");
			scene.GetItemToSellList(listOfGoods);
		}

		if(!listOfGoods.empty())
		{
			SetFontColor(ITEMS_HEADING); // font color
			cout << itemsTitle.c_str() << endl;
			SetFontColor(ITEMS); // font color
			for(Scene::TArticleVector::iterator iter = listOfGoods.begin(); iter != listOfGoods.end(); iter++)
			{
				desc = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(iter->m_item).m_title;
				ConvertToNonDiacriticsCsText(desc);
				cout << desc.c_str() << "(" << iter->m_item << ")" << " - " << iter->m_goldCount << " Zlatych " << endl;
			}
			cout << endl;
		}
		return;	
	}
}


void WriteListCharacterItems(Character& character, bool forUseOnly)
{
	wxString title;
	SetFontColor(ITEMS_HEADING); // font color
	cout << " Batoh (max. " <<  character.GetBackPack().GetMaxCount() << "): " << endl;
	SetFontColor(ITEMS); // font color
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
	SetFontColor(ITEMS_HEADING); // font color
	cout << " Zbrane: ";
	SetFontColor(ITEMS); // font color
	if(!character.GetWeapons().IsEmpty())
	{
		for (Character::CharacterWeapons::Iterator iter = character.GetWeapons().Begin()
				; iter != character.GetWeapons().End()
				; iter++)
		{
			title = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(*iter).m_title;
			ConvertToNonDiacriticsCsText(title);
			cout << title.c_str() << "(" << g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(*iter).m_id << ")" << ", ";
		}
		cout << endl;
	} else {
		cout << " -- " << endl; 
	}
	
	title = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(character.GetBody().GetHeadItem()).m_title;
	ConvertToNonDiacriticsCsText(title);
	SetFontColor(ITEMS_HEADING); // font color
	cout << " Hlava: ";
	SetFontColor(ITEMS); // font color
	if(character.GetBody().GetHeadItem() != ITEM_UNKNOWN)
	{
		cout << title.c_str() << "(" << g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(character.GetBody().GetHeadItem()).m_id << ")" << endl;	
	} else {
		cout << " -- " << endl;
	}
	
	title = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(character.GetBody().GetTorsoItem()).m_title;
	ConvertToNonDiacriticsCsText(title);
	SetFontColor(ITEMS_HEADING); // font color
	cout << " Torso: " ;
	SetFontColor(ITEMS); // font color
	if(character.GetBody().GetTorsoItem() != ITEM_UNKNOWN)
	{
		cout << title.c_str() << "(" << g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(character.GetBody().GetTorsoItem()).m_id << ")" << endl;
	} else {
		cout << " -- " << endl;
	}
	SetFontColor(ITEMS_HEADING); // font color
	cout << " Specialni predmety: ";
	SetFontColor(ITEMS); // font color
	for (Character::CharacterSpecialItems::Iterator iter = character.GetSpecialItems().Begin()
			; iter != character.GetSpecialItems().End()
			; iter++)
	{
		title = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(*iter).m_title;
		ConvertToNonDiacriticsCsText(title);
		cout << title.c_str() << "(" << g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(*iter).m_id << ")" << ", ";
	}
	cout << endl;	
}



void WriteScene(Scene& scene)
{
	wxString desc;
	wxString act;
	wxVector<wxDword> posibleActions;
	
	
	
	scene.GetPosibleActions(g_gameEngine.GetMainCharacter(), posibleActions);
	
	
	desc = scene.m_desc;
	ConvertToNonDiacriticsCsText(desc);
	ConvertToNonBreakText(desc);
	ConvertToBasicFormatedText(desc);
	SetFontColor(HEADING); // font color
	cout << "\n *** " << endl;
	SetFontColor(SCENE_DESCRIPTION); // font color
	cout << desc.c_str() << endl;
	
	WriteListSceneItems(scene, ITEM_LIST_SCENE_ITEMS);
	WriteListSceneItems(scene, ITEM_LIST_SCENE_BUY);
	WriteListSceneItems(scene, ITEM_LIST_SCENE_SELL);
	
	if((posibleActions.empty() || g_gameEngine.GetMainCharacter().GetActualConditions() <= 0))
	{
		SetFontColor(DEAD); // font color
		cout << "Tva cesta zde skoncila, jsi mrtev! (restart)" << endl;
	} else {
		SetFontColor(ACTIONS); // font color
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
	SetFontColor(HEADING); // font color
	cout << " *** Stav postavy *** " << endl;
	SetFontColor(ITEMS_HEADING); // font color
	cout << " Discipliny a umeni (" << character.GetDisciplines().Size() << "): " << endl;
	SetFontColor(ITEMS); // font color
	for (CharacterDisciplines::Iterator iter = character.GetDisciplines().Begin()
			; iter != character.GetDisciplines().End()
			; iter++)
	{
		title = g_gameEngine.GetResMgr().GetDisciplineMgr().GetDiscipline(iter->first).m_title;
		ConvertToNonDiacriticsCsText(title);
		if( character.GetDisciplines().FindValue(iter->first) != NULL)
		{
			if(!character.GetDisciplines().FindValue(iter->first)->m_weaponClass.empty())
			{
				wxString weapon = g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItem(
									g_gameEngine.GetResMgr().GetItemAndDiscMgr().GetItemType(
										wxString(Convertor::GetWeaponClassName(
											character.GetDisciplines().FindValue(iter->first)->m_weaponClass[0])
											)
										)
									).m_title;
				ConvertToNonDiacriticsCsText(weapon);
				cout << title.c_str() << " (" << weapon.c_str() << "), " << endl;
			} else {
				cout << title.c_str() << ", " << endl;
			}
		} else {
			cout << "Chyba v disciplinach postavy" << endl;
		}
		
	}
	//cout << endl;
	SetFontColor(ITEMS_HEADING); // font color
	cout << " Kondice (zakladni kondice " << character.GetBaseConditions() << "): ";
	if(character.GetActualConditions() < 10)
	{
		SetFontColor(HEALTH_BAD);
	} else if(character.GetActualConditions() < 20) {
		SetFontColor(HEALTH_GOOD);
	} else {
		SetFontColor(HEALTH_OK);
	}
	cout << character.GetActualConditions() << "/" << character.GetMaxConditions() << endl;
	SetFontColor(ITEMS_HEADING); // font color
	cout << " Zakladni umeni boje: ";
	SetFontColor(ITEMS); // font color
	cout << character.GetBaseAttackSkill() << endl;

	WriteListCharacterItems(character);

	SetFontColor(ITEMS_HEADING); // font color
	cout << " Mesec (max. " << character.GetMaxGoldCount() << " Zlatych): ";
	SetFontColor(ITEMS); // font color
	cout << character.GetGoldCount() << " Zlatych" << endl;
	SetFontColor(HEADING); // font color
	cout << " **********" << endl;
	
						
}

bool PickUpItem(wxString &command, Scene& scene, Character& character)
{
	long int itemValue = 0;
	EItem item = ITEM_UNKNOWN;
	wxString title;
	SetFontColor(ACTIONS); // font color
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
	SetFontColor(ACTIONS); // font color
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
	SetFontColor(ACTIONS); // font color
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
	SetFontColor(ACTIONS); // font color
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
	SetFontColor(ACTIONS); // font color
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