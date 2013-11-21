#include "reader.h"

#include <wx/xml/xml.h>

bool LoneWolfXmlReader::CreateGameFromXmlFile(const wxChar* xmlfilepath, SceneManager& sceneMgr)
{
	wxXmlDocument xmlDoc;
	if(!xmlDoc.Load(wxString(xmlfilepath)))
	{
		return false;
	}
	
	if(xmlDoc.GetRoot()->GetName() != GENERAL_TAG_ROOT_STR)
		return false;
	
	//get root
	wxXmlNode* child = xmlDoc.GetRoot()->GetChildren();
	while(child)
	{
		if(child->GetName() == GENERAL_TAG_DEFINITIONS_STR)
		{
			if(!ParseDefinitions(child, sceneMgr))
				return false;
		} else if(child->GetName() == GENERAL_TAG_TITLE_STR) {
			if(!ParseTitle(child, sceneMgr))
				return false;
		} else if(child->GetName() == GENERAL_TAG_SCENE_STR) {
			if(!ParseScene(child, sceneMgr))
				return false;
		} else {
			return false;
		}
		
		child = child->GetNext();
	}
}

bool LoneWolfXmlReader::ParseDefinitions(wxXmlNode* defNode, SceneManager& sceneMgr)
{
	wxXmlNode* child = defNode->GetChildren();
	while(child)
	{
		if(child->GetName() == GENERAL_TAG_ACTIONS_STR)
		{
			if(!ParseDefActions(child, sceneMgr))
				return false;
		} else if(child->GetName() == GENERAL_TAG_DISCIPLINES_STR) {
			if(!ParseDefDisciplines(child, sceneMgr))
				return false;
		} else if(child->GetName() == GENERAL_TAG_WEAPONS_STR) {
			if(!ParseDefWeapons(child, sceneMgr))
				return false;
		} else if(child->GetName() == GENERAL_TAG_BAG_ITEMS_STR) {
			if(!ParseDefBagItems(child, sceneMgr))
				return false;
		} else if(child->GetName() == GENERAL_TAG_SPECIAL_ITEMS_STR) {
			if(!ParseDefSpecialItems(child, sceneMgr))
				return false;
		} else {
			return false;
		}
		
		child = child->GetNext();
	}
	
	return true;
}

bool LoneWolfXmlReader::ParseScene(wxXmlNode* sceneNode, SceneManager& sceneMgr)
{
}

bool LoneWolfXmlReader::ParseTitle(wxXmlNode* titleNode, SceneManager& sceneMgr)
{
}

bool LoneWolfXmlReader::ParseDefActions(wxXmlNode* defNode, SceneManager& sceneMgr)
{
	wxString nameValue;
	wxString descValue;
	wxXmlNode *child = defNode->GetChildren();
	while(child)
	{
		if(child->GetName() == GENERAL_TAG_ACTION_STR)
		{
			// read attributes
			nameValue.Clear();
			if(!child->GetAttribute(wxString(GENERAL_ATTR_NAME_STR), &nameValue))
			{
				return false;
			}
			descValue = child->GetAttribute(wxString(GENERAL_ATTR_DESC_STR));
			
			// create new action definition
			
			
		} else {
			return false;
		}
		
		child = child->GetNext();
	}
	
	return true;
}

bool LoneWolfXmlReader::ParseDefBagItems(wxXmlNode* defNode, SceneManager& sceneMgr)
{
	wxXmlNode *child = defNode->GetChildren();
	while(child)
	{
		if(child->GetName() == GENERAL_TAG_ACTIONS_STR)
		{
			if(!ParseDefActions(child, sceneMgr))
				return false;
		} else if(child->GetName() == GENERAL_TAG_DISCIPLINES_STR) {
			if(!ParseDefDisciplines(child, sceneMgr))
				return false;
		} else if(child->GetName() == GENERAL_TAG_WEAPONS_STR) {
			if(!ParseDefWeapons(child, sceneMgr))
				return false;
		} else if(child->GetName() == GENERAL_TAG_BAG_ITEMS_STR) {
			if(!ParseDefBagItems(child, sceneMgr))
				return false;
		} else if(child->GetName() == GENERAL_TAG_SPECIAL_ITEMS_STR) {
			if(!ParseDefSpecialItems(child, sceneMgr))
				return false;
		} else {
			return false;
		}
		
		child = child->GetNext();
	}
}

bool LoneWolfXmlReader::ParseDefDisciplines(wxXmlNode* defNode, SceneManager& sceneMgr)
{
	wxXmlNode *child = defNode->GetChildren();
	while(child)
	{
		if(child->GetName() == GENERAL_TAG_ACTIONS_STR)
		{
			if(!ParseDefActions(child, sceneMgr))
				return false;
		} else if(child->GetName() == GENERAL_TAG_DISCIPLINES_STR) {
			if(!ParseDefDisciplines(child, sceneMgr))
				return false;
		} else if(child->GetName() == GENERAL_TAG_WEAPONS_STR) {
			if(!ParseDefWeapons(child, sceneMgr))
				return false;
		} else if(child->GetName() == GENERAL_TAG_BAG_ITEMS_STR) {
			if(!ParseDefBagItems(child, sceneMgr))
				return false;
		} else if(child->GetName() == GENERAL_TAG_SPECIAL_ITEMS_STR) {
			if(!ParseDefSpecialItems(child, sceneMgr))
				return false;
		} else {
			return false;
		}
		
		child = child->GetNext();
	}
}

bool LoneWolfXmlReader::ParseDefSpecialItems(wxXmlNode* defNode, SceneManager& sceneMgr)
{
	wxXmlNode *child = defNode->GetChildren();
	while(child)
	{
		if(child->GetName() == GENERAL_TAG_ACTIONS_STR)
		{
			if(!ParseDefActions(child, sceneMgr))
				return false;
		} else if(child->GetName() == GENERAL_TAG_DISCIPLINES_STR) {
			if(!ParseDefDisciplines(child, sceneMgr))
				return false;
		} else if(child->GetName() == GENERAL_TAG_WEAPONS_STR) {
			if(!ParseDefWeapons(child, sceneMgr))
				return false;
		} else if(child->GetName() == GENERAL_TAG_BAG_ITEMS_STR) {
			if(!ParseDefBagItems(child, sceneMgr))
				return false;
		} else if(child->GetName() == GENERAL_TAG_SPECIAL_ITEMS_STR) {
			if(!ParseDefSpecialItems(child, sceneMgr))
				return false;
		} else {
			return false;
		}
		
		child = child->GetNext();
	}
}

bool LoneWolfXmlReader::ParseDefWeapons(wxXmlNode* defNode, SceneManager& sceneMgr)
{
	wxXmlNode *child = defNode->GetChildren();
	while(child)
	{
		if(child->GetName() == GENERAL_TAG_ACTIONS_STR)
		{
			if(!ParseDefActions(child, sceneMgr))
				return false;
		} else if(child->GetName() == GENERAL_TAG_DISCIPLINES_STR) {
			if(!ParseDefDisciplines(child, sceneMgr))
				return false;
		} else if(child->GetName() == GENERAL_TAG_WEAPONS_STR) {
			if(!ParseDefWeapons(child, sceneMgr))
				return false;
		} else if(child->GetName() == GENERAL_TAG_BAG_ITEMS_STR) {
			if(!ParseDefBagItems(child, sceneMgr))
				return false;
		} else if(child->GetName() == GENERAL_TAG_SPECIAL_ITEMS_STR) {
			if(!ParseDefSpecialItems(child, sceneMgr))
				return false;
		} else {
			return false;
		}
		
		child = child->GetNext();
	}
}
