#ifndef __LONE_WOLF_CONSOLE_COMMANDS_H__
#define __LONE_WOLF_CONSOLE_COMMANDS_H__

#include "../lwgamelib/scenemgr.h"
#include "../lwgamelib/lwengine.h"


enum EItemListType {
	ITEM_LIST_SCENE_ITEMS,
	ITEM_LIST_SCENE_SELECTION,
	ITEM_LIST_SCENE_BUY,
	ITEM_LIST_SCENE_SELL
};



void WriteListSceneItems(Scene& scene, EItemListType type);
void WriteListCharacterItems(Character& character, bool forUseOnly = false);

void WriteScene(Scene& scene);
void WriteCharacterState(Character& character);
bool PickUpItem(wxString &command, Scene& scene, Character& character);
bool DropItem(wxString &command, Scene& scene, Character& character);
bool UseItem(wxString &command, Character& character);
bool BuyItem(wxString &command, Scene& scene, Character& character );
bool SellItem(wxString &command, Scene& scene, Character& character );

#endif //__LONE_WOLF_CONSOLE_COMMANDS_H__