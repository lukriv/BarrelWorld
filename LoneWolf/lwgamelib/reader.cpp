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
		} else if(child->GetName() == GENERAL_TAG_CHAPTER_STR) {
			if(!ParseChapter(child, sceneMgr))
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
	wxXmlNode* child = defNode->GetChildren();
	while(child)
	{
		if(child->GetName() == GENERAL_TAG_SCENE_STR)
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

bool LoneWolfXmlReader::ParseChapter(wxXmlNode* chapterNode, SceneManager& sceneMgr)
{
	wxString nameValue;
	wxString descValue;
	wxString tempValue;
	wxInt32 id;
	wxXmlNode *child = chapterNode->GetChildren();
	if(!chapterNode->GetAttribute(wxString(GENERAL_ATTR_TITLE_STR), &nameValue))
	{
		return false;
	}
	sceneMgr.SetTitle(nameValue);
	if(!chapterNode->GetAttribute(wxString(GENERAL_ATTR_ID_STR), &tempValue))
	{
		return false;
	}
	tempValue.ToLong(&id);
	sceneMgr.SetId(id);
	while(child)
	{
		if(child->GetName() == GENERAL_TAG_SCENE_STR)
		{
			// read attributes
			tempValue.Clear();
			if(!child->GetAttribute(wxString(GENERAL_ATTR_ID_STR), &tempValue))
			{
				return false;
			}
			descValue = child->GetAttribute(wxString(GENERAL_ATTR_DESC_STR));
			
			
			// create new action definition
			if(ConvertActionNameToType(nameValue) != ACTION_UNKNOWN)
			{
				sceneMgr.GetActionMgr().SetDefaultActionDesc(ConvertActionNameToType(nameValue), desc);
			}
			
		} else {
			return false;
		}
		
		child = child->GetNext();
	}
	
	return true;
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
			if(ConvertActionNameToType(nameValue) != ACTION_UNKNOWN)
			{
				sceneMgr.GetActionMgr().SetDefaultActionDesc(ConvertActionNameToType(nameValue), desc);
			}
			
		} else {
			return false;
		}
		
		child = child->GetNext();
	}
	
	return true;
}

bool LoneWolfXmlReader::ParseDefBagItems(wxXmlNode* defNode, SceneManager& sceneMgr)
{
	wxString nameValue;
	wxString descValue;
	wxString titleValue;
	wxXmlNode *child = defNode->GetChildren();
	while(child)
	{
		if(child->GetName() == GENERAL_TAG_BAG_ITEM_STR)
		{
			nameValue.Clear();
			if(!child->GetAttribute(wxString(GENERAL_ATTR_NAME_STR), &nameValue))
			{
				return false;
			}
			titleValue = child->GetAttribute(wxString(GENERAL_ATTR_TITLE_STR));
			descValue = child->GetAttribute(wxString(GENERAL_ATTR_DESC_STR));
			
			// create new action definition
			if(Convertor::ConvertBagItemNameToType(nameValue) != BAG_ITEM_UNKNOWN)
			{
				if(!sceneMgr->GetItemAndDiscMgr().AddBagItem(Convertor::ConvertBagItemNameToType(nameValue), titleValue, descValue)
				{
					return false;
				}
			}
		} else {
			return false;
		}
		
		child = child->GetNext();
	}
	
	return true;
}

bool LoneWolfXmlReader::ParseDefDisciplines(wxXmlNode* defNode, SceneManager& sceneMgr)
{
	wxString nameValue;
	wxString descValue;
	wxString titleValue;
	wxXmlNode *child = defNode->GetChildren();
	while(child)
	{
		if(child->GetName() == GENERAL_TAG_SKILL_STR)
		{		
			// read attributes
			nameValue.Clear();
			if(!child->GetAttribute(wxString(GENERAL_ATTR_NAME_STR), &nameValue))
			{
				return false;
			}
			titleValue = child->GetAttribute(wxString(GENERAL_ATTR_TITLE_STR));
			descValue = child->GetAttribute(wxString(GENERAL_ATTR_DESC_STR));
			
			// create new action definition
			if(Convertor::ConvertDisciplineNameToType(nameValue) != DISCIPLINE_UNKNOWN)
			{
				sceneMgr->GetDisciplineMgr().AddDisciplineDesc(Convertor::ConvertDisciplineNameToType(nameValue), titleValue, descValue);
			}

		} else {
			return false;
		}
		
		child = child->GetNext();
	}
}

bool LoneWolfXmlReader::ParseDefSpecialItems(wxXmlNode* defNode, SceneManager& sceneMgr)
{
	wxString nameValue;
	wxString descValue;
	wxString titleValue;
	wxXmlNode *child = defNode->GetChildren();
	while(child)
	{
		if(child->GetName() == GENERAL_TAG_SPECIAL_ITEM_STR)
		{
			nameValue.Clear();
			if(!child->GetAttribute(wxString(GENERAL_ATTR_NAME_STR), &nameValue))
			{
				return false;
			}
			titleValue = child->GetAttribute(wxString(GENERAL_ATTR_TITLE_STR));
			descValue = child->GetAttribute(wxString(GENERAL_ATTR_DESC_STR));
			
			// create new action definition
			if(Convertor::ConvertSpecialItemNameToType(nameValue) != BAG_ITEM_UNKNOWN)
			{
				if(!sceneMgr->GetItemAndDiscMgr().AddSpecialItem(Convertor::ConvertSpecialItemNameToType(nameValue), titleValue, descValue))
				{
					return false;
				}
			}
		} else {
			return false;
		}
		
		child = child->GetNext();
	}
	
	return true;
}

bool LoneWolfXmlReader::ParseDefWeapons(wxXmlNode* defNode, SceneManager& sceneMgr)
{
	wxString nameValue;
	wxString descValue;
	wxString titleValue;
	wxXmlNode *child = defNode->GetChildren();
	while(child)
	{
		if(child->GetName() == GENERAL_TAG_WEAPON_STR)
		{
			nameValue.Clear();
			if(!child->GetAttribute(wxString(GENERAL_ATTR_NAME_STR), &nameValue))
			{
				return false;
			}
			titleValue = child->GetAttribute(wxString(GENERAL_ATTR_TITLE_STR));
			descValue = child->GetAttribute(wxString(GENERAL_ATTR_DESC_STR));
			
			// create new action definition
			if(Convertor::ConvertWeaponNameToType(nameValue) != BAG_ITEM_UNKNOWN)
			{
				if(!sceneMgr->GetItemAndDiscMgr().AddWeapon(Convertor::ConvertWeaponNameToType(nameValue), titleValue, descValue)
				{
					return false;
				}
			}
		} else {
			return false;
		}
		
		child = child->GetNext();
	}
	
	return true;
}
