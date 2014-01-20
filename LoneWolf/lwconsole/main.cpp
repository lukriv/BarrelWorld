#include <stdio.h>
#include <iostream>
#include <string>

#include <wx/init.h>
#include "../lwgamelib/scenemgr.h"
#include "../lwgamelib/lwengine.h"
#include "textConversion.h"
#include "gres.h"
#include "callbackimpl.h"
#include "consolefunc.h"
#include "commands.h"

//#include <windows.h>


using namespace std;





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
		SetFontColor(COMMAND_LINE); // set color
		cout << "Zadej cislo akce nebo prikaz (exit, char, scene, pickup, drop, use, buy, sell): " << endl;
		while (true)
		{
			SetFontColor(COMMAND_LINE); // set font
			cout << "> ";
			cin >> input;
			act = input;
			actChoose = 0;
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
				WriteListSceneItems(*g_gameEngine.GetActualScene(), ITEM_LIST_SCENE_ITEMS);
				WriteListSceneItems(*g_gameEngine.GetActualScene(), ITEM_LIST_SCENE_SELECTION);
				SetFontColor(COMMAND_LINE); // set font
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
				WriteListCharacterItems(g_gameEngine.GetMainCharacter());
				SetFontColor(COMMAND_LINE); // set font
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
				WriteListCharacterItems(g_gameEngine.GetMainCharacter());
				SetFontColor(COMMAND_LINE); // set font
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
				WriteListSceneItems(*g_gameEngine.GetActualScene(), ITEM_LIST_SCENE_BUY);
				SetFontColor(COMMAND_LINE); // set font
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
				WriteListSceneItems(*g_gameEngine.GetActualScene(), ITEM_LIST_SCENE_SELL);
				SetFontColor(COMMAND_LINE); // set font
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
				//cout << actChoose-1 << endl;
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
