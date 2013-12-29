#include "reader.h"

#include <iostream>
#include <wx/xml/xml.h>
#include "lwxmldefs.h"

bool LoneWolfXmlReader::LoadChapter(const wxChar* xmlfilepath, SceneManager& sceneMgr)
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
		if(child->GetName() == GENERAL_TAG_CHAPTER_STR) 
		{
			if(!ParseChapter(child, sceneMgr))
			{
				ProcessError(child);
				return false;
			}
		} else {
			return false;
		}
		
		child = child->GetNext();
	}
	
	return true;
}



bool LoneWolfXmlReader::ParseScene(wxXmlNode* sceneNode, SceneManager& sceneMgr)
{
	long int readedNumber = 0;
	wxString tempValue;
	Scene scene;
	wxXmlNode* child = sceneNode->GetChildren();

	// read attributes
	tempValue.Clear();
	if(!sceneNode->GetAttribute(wxString(GENERAL_ATTR_ID_STR), &tempValue))
	{
		ProcessError(child);
		return false;
	}
	
	tempValue.ToLong(&readedNumber);
	scene.m_sceneId = readedNumber;
			
	while(child)
	{
		if(child->GetName() == GENERAL_TAG_DESC_STR)
		{
			wxXmlNode *cdata = child->GetChildren();
			if(cdata->GetType() == wxXML_CDATA_SECTION_NODE)
			{
				scene.m_desc = cdata->GetContent();
			}
		} else if (child->GetName() == GENERAL_TAG_ACTIONS_STR) {
			if(!ParseSceneActions(child, scene))
			{
				ProcessError(child);
				return false;
			}
			
		}
		
		child = child->GetNext();
	}
	
	if(!sceneMgr.AddScene(scene))
	{
		ProcessError(child);
		return false;
	}
	
	return true;
}

bool LoneWolfXmlReader::ParseSceneActions(wxXmlNode* sceneActionNode, Scene& scene)
{
	long unsigned int loteryFrom = 0, loteryTo = 0;
	long unsigned int target = 0;
	wxString tempValue;
	wxString nameValue;
	wxXmlNode* child = sceneActionNode->GetChildren();
	wxXmlNode *loteryNode = NULL;
	
	while (child)
	{
		if(child->GetName() == GENERAL_TAG_ACTION_STR)
		{
			Action action;
			
			// read attributes
			nameValue.Clear();
			if(!child->GetAttribute(wxString(GENERAL_ATTR_NAME_STR), &nameValue))
			{
				ProcessError(child);
				return false;
			}
			
			EActionType actionType = Convertor::ConvertActionNameToType(nameValue);
			tempValue.Clear();
			switch(actionType)
			{
				case ACTION_CREATE_CHAR:
					action.SetType(ACTION_CREATE_CHAR);
					
					
					if(!child->GetAttribute(GENERAL_ATTR_TARGET_STR, &tempValue))
					{
						ProcessError(child);
						return false;
					}
					tempValue.ToULong(&target);
					
					action.SetMoveTarget(target);
					
					break;
				case ACTION_MOVE:
					action.SetType(ACTION_MOVE);
					if(!child->GetAttribute(GENERAL_ATTR_DESC_STR, &tempValue))
					{
						ProcessError(child);
						return false;
					}
					
					action.SetDesc(tempValue);
					
					if(!child->GetAttribute(GENERAL_ATTR_TARGET_STR, &tempValue))
					{
						ProcessError(child);
						return false;
					}
					tempValue.ToULong(&target);
					
					action.SetMoveTarget(target);
					break;
				case ACTION_LOTERY:
					action.SetType(ACTION_LOTERY);
					loteryNode = child->GetChildren();
					while(loteryNode)
					{
						if(loteryNode->GetName() == GENERAL_TAG_SELECTION_STR)
						{
							tempValue.Clear();
							if(!loteryNode->GetAttribute(GENERAL_ATTR_NUMBERS_FROM_STR, &tempValue))
							{
								ProcessError(loteryNode);
								return false;
							}
							tempValue.ToULong(&loteryFrom);
							
							tempValue.Clear();
							if(!loteryNode->GetAttribute(GENERAL_ATTR_NUMBERS_TO_STR, &tempValue))
							{
								ProcessError(loteryNode);
								return false;
							}
							tempValue.ToULong(&loteryTo);
							tempValue.Clear();
							if(!loteryNode->GetAttribute(GENERAL_ATTR_TARGET_STR, &tempValue))
							{
								ProcessError(loteryNode);
								return false;
							}
							tempValue.ToULong(&target);
							
							action.SetLoteryTarget(loteryFrom, loteryTo, target);
							
						}
						loteryNode = loteryNode->GetNext();
					}
					break;
				
				default:
					ProcessError(child);
					return false;
			}
			
			scene.m_actions.push_back(action);
			
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
	long int id = 0;
	wxXmlNode *child = chapterNode->GetChildren();
	if(!chapterNode->GetAttribute(wxString(GENERAL_ATTR_TITLE_STR), &nameValue))
	{
		ProcessError(child);
		return false;
	}
	sceneMgr.SetTitle(nameValue);
	if(!chapterNode->GetAttribute(wxString(GENERAL_ATTR_ID_STR), &tempValue))
	{
		ProcessError(child);
		return false;
	}
	tempValue.ToLong(&id);
	sceneMgr.SetId(id);
	while(child)
	{
		if(child->GetName() == GENERAL_TAG_SCENE_STR)
		{
			if(!ParseScene(child, sceneMgr))
			{
				ProcessError(child);
				return false;
			}			
		} else {
			ProcessError(child);
			return false;
		}
		
		child = child->GetNext();
	}
	
	return true;
}

void LoneWolfXmlReader::ProcessError(wxXmlNode* errNode)
{
	wxXmlAttribute *attributes = errNode->GetAttributes();
	std::cout << "***** ERROR *****" << std::endl;
	std::cout << "Error on line: " << errNode->GetLineNumber() << std::endl;
	std::cout << "Name of node: " << errNode->GetName().c_str() << std::endl;
	std::cout << "Node attributes: " << std::endl;
	while (attributes)
	{
		std::cout << "  " << attributes->GetName().c_str() << ":\"" << attributes->GetValue().c_str() << "\"" << std::endl;
		attributes = attributes->GetNext();
	}
	
}

//----------------------------- Definitions ----------------------------------

bool LoneWolfXmlReader::LoadTextDefinitions(const wxChar* xmlfilepath, GlobalResourceManager& resMgr)
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
			if(!ParseDefinitions(child, resMgr))
			{
				ProcessError(child);
				return false;
			}
				
		} else {
			return false;
		}
		
		child = child->GetNext();
	}
	
	return true;
}

bool LoneWolfXmlReader::ParseDefinitions(wxXmlNode* defNode, GlobalResourceManager& resMgr)
{
	wxXmlNode* child = defNode->GetChildren();
	while(child)
	{
		if(child->GetName() == GENERAL_TAG_ACTIONS_STR)
		{
			if(!ParseDefActions(child, resMgr))
			{
				ProcessError(child);
				return false;
			}
		} else if(child->GetName() == GENERAL_TAG_DISCIPLINES_STR) {
			if(!ParseDefDisciplines(child, resMgr))
			{
				ProcessError(child);
				return false;
			}
		} else if(child->GetName() == GENERAL_TAG_WEAPONS_STR) {
			if(!ParseDefWeapons(child, resMgr))
			{
				ProcessError(child);
				return false;
			}
		} else if(child->GetName() == GENERAL_TAG_BAG_ITEMS_STR) {
			if(!ParseDefBagItems(child, resMgr))
			{
				ProcessError(child);
				return false;
			}
		} else if(child->GetName() == GENERAL_TAG_SPECIAL_ITEMS_STR) {
			if(!ParseDefSpecialItems(child, resMgr))
			{
				ProcessError(child);
				return false;
			}
		} else {
			ProcessError(child);
			return false;
		}
		
		child = child->GetNext();
	}
	
	return true;
}

bool LoneWolfXmlReader::ParseDefWeapons(wxXmlNode* defNode, GlobalResourceManager& resMgr)
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
				ProcessError(child);
				return false;
			}
			titleValue = child->GetAttribute(wxString(GENERAL_ATTR_TITLE_STR));
			descValue = child->GetAttribute(wxString(GENERAL_ATTR_DESC_STR));
			
			// create new action definition
			if(Convertor::ConvertWeaponNameToType(nameValue) != WEAPON_UNKNOWN)
			{
				if(!resMgr.GetItemAndDiscMgr().AddWeapon(Convertor::ConvertWeaponNameToType(nameValue), titleValue, descValue))
				{
					ProcessError(child);
					return false;
				}
			}
		} else {
			ProcessError(child);
			return false;
		}
		
		child = child->GetNext();
	}
	
	return true;
}


bool LoneWolfXmlReader::ParseDefSpecialItems(wxXmlNode* defNode, GlobalResourceManager& resMgr)
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
				ProcessError(child);
				return false;
			}
			titleValue = child->GetAttribute(wxString(GENERAL_ATTR_TITLE_STR));
			descValue = child->GetAttribute(wxString(GENERAL_ATTR_DESC_STR));
			
			// create new action definition
			if(Convertor::ConvertSpecialItemNameToType(nameValue) != SPECIAL_ITEM_UNKNOWN)
			{
				if(!resMgr.GetItemAndDiscMgr().AddSpecialItem(Convertor::ConvertSpecialItemNameToType(nameValue), titleValue, descValue))
				{
					ProcessError(child);
					return false;
				}
			}
		} else {
			ProcessError(child);
			return false;
		}
		
		child = child->GetNext();
	}
	
	return true;
}


bool LoneWolfXmlReader::ParseDefDisciplines(wxXmlNode* defNode, GlobalResourceManager& resMgr)
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
				ProcessError(child);
				return false;
			}
			titleValue = child->GetAttribute(wxString(GENERAL_ATTR_TITLE_STR));
			descValue = child->GetAttribute(wxString(GENERAL_ATTR_DESC_STR));
			
			// create new action definition
			if(Convertor::ConvertDisciplineNameToType(nameValue) != DISCIPLINE_UNKNOWN)
			{
				resMgr.GetDisciplineMgr().AddDisciplineDesc(Convertor::ConvertDisciplineNameToType(nameValue), titleValue, descValue);
			}

		} else {
			ProcessError(child);
			return false;
		}
		
		child = child->GetNext();
	}
	
	return true;
}

bool LoneWolfXmlReader::ParseDefBagItems(wxXmlNode* defNode, GlobalResourceManager& resMgr)
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
				ProcessError(child);
				return false;
			}
			titleValue = child->GetAttribute(wxString(GENERAL_ATTR_TITLE_STR));
			descValue = child->GetAttribute(wxString(GENERAL_ATTR_DESC_STR));
			
			// create new action definition
			if(Convertor::ConvertBagItemNameToType(nameValue) != BAG_ITEM_UNKNOWN)
			{
				if(!resMgr.GetItemAndDiscMgr().AddBagItem(Convertor::ConvertBagItemNameToType(nameValue), titleValue, descValue))
				{
					ProcessError(child);
					return false;
				}
			}
		} else {
			ProcessError(child);
			return false;
		}
		
		child = child->GetNext();
	}
	
	return true;
}


bool LoneWolfXmlReader::ParseDefActions(wxXmlNode* defNode, GlobalResourceManager& resMgr)
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
				ProcessError(child);
				return false;
			}
			descValue = child->GetAttribute(wxString(GENERAL_ATTR_DESC_STR));
			
			// create new action definition
			if(Convertor::ConvertActionNameToType(nameValue) != ACTION_UNKNOWN)
			{
				resMgr.GetActionMgr().SetDefaultActionDesc(Convertor::ConvertActionNameToType(nameValue), descValue);
			}
			
		} else {
			ProcessError(child);
			return false;
		}
		
		child = child->GetNext();
	}
	
	return true;
}