#include "reader.h"

#include <iostream>
#include <wx/sstream.h>
#include <wx/stdstream.h>
#include <wx/xml/xml.h>
#include "lwxmldefs.h"
#include <wx/scopedptr.h>
#include "character.h"

wxString LoneWolfXmlReader::m_errorString = wxString();

wxString LoneWolfXmlReader::m_language = wxString(CZECH_LANG);

bool LoneWolfXmlReader::LoadChapter(const wxChar* xmlfilepath, GlobalResourceManager& resMgr, SceneManager& sceneMgr)
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
			if(!ParseChapter(child, resMgr, sceneMgr))
			{
				return false;
			}
		} else {
			return false;
		}
		
		child = child->GetNext();
	}
	
	return true;
}



bool LoneWolfXmlReader::ParseScene(wxXmlNode* sceneNode, GlobalResourceManager& resMgr, SceneManager& sceneMgr)
{
	long int readedNumber = 0;
	wxString tempValue;
	wxScopedPtr<Scene> spScene;
	wxXmlNode* child = sceneNode->GetChildren();

	spScene.reset(new(std::nothrow) Scene());
	if(spScene.get() == NULL) return false;
	// read attributes
	tempValue.Clear();
	if(!sceneNode->GetAttribute(wxString(GENERAL_ATTR_ID_STR), &tempValue))
	{
		ProcessError(child);
		return false;
	}
	
	tempValue.ToLong(&readedNumber);
	spScene->SetSceneId(readedNumber);
			
	while(child)
	{
		if(child->GetName() == GENERAL_TAG_DESC_STR)
		{
			wxString lang = child->GetAttribute(wxString(GENERAL_ATTR_LANGUAGE_STR));
			if(lang == m_language)
			{
				wxXmlNode *cdata = child->GetChildren();
				if(cdata->GetType() == wxXML_CDATA_SECTION_NODE)
				{
					spScene->m_desc = cdata->GetContent();
				} else {
					ProcessError(child);
					return false;
				}
			}
		} else if (child->GetName() == GENERAL_TAG_ACTIONS_STR) {
			if(!ParseSceneActions(child, resMgr, *(spScene.get())))
			{
				return false;
			}
		} else if (child->GetName() == GENERAL_TAG_ITEMS_STR) {
			if(!ParseSceneItems(child, resMgr, *(spScene.get())))
			{
				return false;
			}
		} else if (child->GetName() == GENERAL_TAG_EVENTS_STR) {
			if(!ParseSceneEvents(child, resMgr, *(spScene.get())))
			{
				return false;
			}
		} else if (child->GetName() == GENERAL_TAG_MERCHANDISE_STR) {
			if(!ParseSceneMerchadise(child, resMgr, *(spScene.get())))
			{
				return false;
			}
		}
		
		child = child->GetNext();
	}
	
	if(!sceneMgr.AddScene(spScene.release()))
	{
		ProcessError(sceneNode, wxT("Scene add failed!"));
		return false;
	}
	
	return true;
}

bool LoneWolfXmlReader::ParseSceneActions(wxXmlNode* sceneActionNode, GlobalResourceManager& resMgr, Scene& scene)
{
	long unsigned int loteryFrom = 0, loteryTo = 0;
	long int target = 0;
	wxString tempValue;
	wxString nameValue;
	wxXmlNode* child = sceneActionNode->GetChildren();
	wxXmlNode *loteryNode = NULL;
	wxScopedPtr<Action> spAction;
	wxScopedPtr<ActionFight> spActionFight;
	EActionType actionType = ACTION_UNKNOWN;
	
	while (child)
	{
		// read attributes
		actionType = Convertor::ConvertActionNameToType(child->GetName());
		
		switch(actionType)
		{
			case ACTION_CREATE_CHAR:
			case ACTION_MOVE:
			{
				wxXmlNode* textNode = child->GetChildren();
				spAction.reset(new(std::nothrow) Action());
				spAction->SetType(actionType);
				
				if(!child->GetAttribute(GENERAL_ATTR_TARGET_STR, &tempValue))
				{
					ProcessError(child);
					return false;
				}
				tempValue.ToLong(&target);
				
				spAction->SetMoveTarget(target);
				
				if(!ReadActionAttr(child, resMgr, *(spAction.get())))
				{
					return false;
				}
				
				if(textNode)
				{
					if(textNode->GetName() == GENERAL_TAG_DESC_STR)
					{
						wxString text;
						wxString lang;
						if(!ReadText(textNode, text, lang))
						{
							return false;
						}
						
						if(lang == m_language)
						{
							spAction->SetDesc(text);
						}
						
					}
					textNode = textNode->GetNext();
				}
				
				scene.m_actions.push_back(spAction.release());
				
				break;
			}
			case ACTION_LOTERY:
			{	
				spAction.reset(new(std::nothrow) Action());
				spAction->SetType(actionType);
				
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
						tempValue.ToLong(&target);
						
						spAction->SetLoteryTarget(loteryFrom, loteryTo, target);
						
					} else {
						ProcessError(loteryNode);
						return false;
					}
					loteryNode = loteryNode->GetNext();
				}
				
				if(!ReadActionAttr(child, resMgr, *(spAction.get())))
				{
					return false;
				}
				
				scene.m_actions.push_back(spAction.release());
				
				break;
			}
			case ACTION_FIGHT:
			{
				spActionFight.reset(new(std::nothrow) ActionFight());
			
				if(!ReadActionAttr(child, resMgr, *(spActionFight.get())))
				{
					return false;
				}
				
				scene.m_actions.push_back(spActionFight.release());
				
				break;
			}
			default:
				ProcessError(child);
				return false;
		}
		
		tempValue.Clear();
		
		child = child->GetNext();
	}
	return true;
}

bool LoneWolfXmlReader::ReadActionAttr(wxXmlNode* actionNode, GlobalResourceManager& resMgr, Action& action)
{
	wxString requiredSkillValue;
	wxString requiredItemValue;
	action.SetCondition(actionNode->HasAttribute(GENERAL_ATTR_CONDITIONED_STR));
	
	if(actionNode->HasAttribute(GENERAL_ATTR_SKILL_NEEDED_STR))
	{
		requiredSkillValue = actionNode->GetAttribute(wxString(GENERAL_ATTR_SKILL_NEEDED_STR));
		if(resMgr.GetDisciplineMgr().GetDisciplineType(requiredSkillValue) != DISCIPLINE_UNKNOWN)
		{
			action.SetRequiredSkill(resMgr.GetDisciplineMgr().GetDisciplineType(requiredSkillValue));
		} else {
			ProcessError(actionNode);
			return false;
		}
	}

	if(actionNode->HasAttribute(GENERAL_ATTR_ITEM_NEEDED_STR))
	{	
		requiredItemValue = actionNode->GetAttribute(wxString(GENERAL_ATTR_ITEM_NEEDED_STR));
		if(resMgr.GetItemAndDiscMgr().GetItemType(requiredItemValue) != ITEM_UNKNOWN)
		{
			action.SetRequiredItem(resMgr.GetItemAndDiscMgr().GetItemType(requiredItemValue));
		} else {
			ProcessError(actionNode);
			return false;
		}
	}
	
	if(actionNode->HasAttribute(GENERAL_ATTR_GOLD_COUNT_STR))
	{
		long goldCount = 0;
		requiredItemValue = actionNode->GetAttribute(wxString(GENERAL_ATTR_GOLD_COUNT_STR));
		requiredItemValue.ToLong(&goldCount);
		action.SetMinimumGold(goldCount);
	}
	
	return true;
}

bool LoneWolfXmlReader::ReadActionAttr(wxXmlNode* actionNode, GlobalResourceManager& resMgr, ActionFight& action)
{
	wxString tempValue;
	wxString skillName;
	long int intValue = 0;
	wxXmlNode* child = actionNode->GetChildren();
	
	if(!actionNode->GetAttribute(wxString(GENERAL_ATTR_WIN_STR), &tempValue))
	{
		ProcessError(actionNode);
		return false;
	}
	tempValue.ToLong(&intValue);
	action.SetWinTarget(intValue);
	
	if(actionNode->GetAttribute(wxString(GENERAL_ATTR_LOST_STR), &tempValue))
	{
		tempValue.ToLong(&intValue);
		action.SetLoseTarget(intValue);
	}
	
	if(actionNode->GetAttribute(wxString(GENERAL_ATTR_RETREAT_STR), &tempValue))
	{
		tempValue.ToLong(&intValue);
		action.SetRetreatTarget(intValue);
	}
	
	if(actionNode->GetAttribute(wxString(GENERAL_ATTR_TURN_TO_WIN_STR), &tempValue))
	{
		tempValue.ToLong(&intValue);
		action.SetTurnsToWin(intValue);
	}
	
	if(actionNode->GetAttribute(wxString(GENERAL_ATTR_TURN_TO_RETREAT_STR), &tempValue))
	{
		tempValue.ToLong(&intValue);
		action.SetTurnsToRetreat(intValue);
	}
	
	while(child)
	{
		if(child->GetName() == GENERAL_TAG_FIGHT_EVENT_STR)
		{
			Event event;
			event.m_type = EVENT_FIGHT;
			
			if(!ReadEventAttr(child, resMgr, event.m_property))
			{
				return false;
			}
			
			action.AddEvent(event);
		} else if(child->GetName() == GENERAL_TAG_ENEMY_STR) {
			Character enemy;
			if(!enemy.Initialize(&resMgr))
			{
				return false;
			}
			
			if(!child->GetAttribute(wxString(GENERAL_ATTR_ATTACK_STR), &tempValue))
			{
				ProcessError(child);
				return false;
			}
			
			tempValue.ToLong(&intValue);
			enemy.SetBaseAttackSkill(intValue);
			
			if(!child->GetAttribute(wxString(GENERAL_ATTR_CONDITION_STR), &tempValue))
			{
				ProcessError(child);
				return false;
			}
			
			tempValue.ToLong(&intValue);
			enemy.SetBaseConditions(intValue);
			
			wxXmlNode* charProp = child->GetChildren();
			while(charProp)
			{
				if(charProp->GetName() == GENERAL_TAG_SKILL_STR)
				{
					DisciplineProperties discProp;
					if(!charProp->GetAttribute(wxString(GENERAL_ATTR_NAME_STR), &skillName))
					{
						ProcessError(charProp);
						return false;
					}
					
					if(resMgr.GetDisciplineMgr().GetDisciplineType(skillName) == DISCIPLINE_UNKNOWN)
					{
						ProcessError(charProp);
						return false;
					}
					
					if(!ReadDisciplineAttr(charProp, resMgr, discProp))
					{
						return false;
					}
					
					if(!enemy.AddNewCharacterDiscipline(resMgr.GetDisciplineMgr().GetDisciplineType(skillName), &discProp))
					{
						ProcessError(charProp);
						return false;
					}
				} else if(charProp->GetName() == GENERAL_TAG_TITLE_STR) {
					wxString lang;
					wxString text;
					if(!ReadText(charProp, text, lang))
					{
						return false;
					}
					if(lang == m_language)
					{
						enemy.SetCharacterName(text);
					}
				}
					
				charProp = charProp->GetNext();
			}
			
			enemy.RecomputeState();
			
			if(!action.AddEnemy(enemy))
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


bool LoneWolfXmlReader::ParseSceneItems(wxXmlNode* sceneActionNode, GlobalResourceManager& resMgr, Scene& scene)
{
	wxString tempValue;
	wxString nameValue;
	wxXmlNode* child = sceneActionNode->GetChildren();
	while(child)
	{
		if(child->GetName() == GENERAL_TAG_ITEM_STR)
		{
			if(!child->GetAttribute(GENERAL_ATTR_NAME_STR, &nameValue))
			{
				ProcessError(child);
				return false;
			}
			
			if(resMgr.GetItemAndDiscMgr().GetItemType(nameValue) != ITEM_UNKNOWN)
			{
				scene.AddItem(resMgr.GetItemAndDiscMgr().GetItemType(nameValue));
			} else {
				ProcessError(child);
				return false;
			}
		}
		child = child->GetNext();
	}
	
	return true;
}

bool LoneWolfXmlReader::ParseSceneMerchadise(wxXmlNode* sceneMerchandiseNode, GlobalResourceManager& resMgr, Scene& scene)
{
	wxString tempValue;
	wxString itemName;
	long int price = 0;
	wxXmlNode* child = sceneMerchandiseNode->GetChildren();
	while(child)
	{
		if(child->GetName() == GENERAL_TAG_BUY_STR)
		{
			// buy item
			if(!child->GetAttribute(wxString(GENERAL_ATTR_ITEM_STR), &itemName))
			{
				ProcessError(child);
				return false;
			}
			
			if(resMgr.GetItemAndDiscMgr().GetItemType(itemName) == ITEM_UNKNOWN)
			{
				ProcessError(child);
				return false;
			}
			
			if(!child->GetAttribute(wxString(GENERAL_ATTR_GOLD_COUNT_STR), &tempValue))
			{
				ProcessError(child);
				return false;
			}
			tempValue.ToLong(&price);
			if(price <= 0)
			{
				ProcessError(child);
				return false;
			}
			
			scene.AddItemToBuy(resMgr.GetItemAndDiscMgr().GetItemType(itemName), price);
			
		} else if(child->GetName() == GENERAL_TAG_SELL_STR) {
			// sell item 
			if(!child->GetAttribute(wxString(GENERAL_ATTR_ITEM_STR), &itemName))
			{
				ProcessError(child);
				return false;
			}
			
			if(resMgr.GetItemAndDiscMgr().GetItemType(itemName) == ITEM_UNKNOWN)
			{
				ProcessError(child);
				return false;
			}
			
			if(!child->GetAttribute(wxString(GENERAL_ATTR_GOLD_COUNT_STR), &tempValue))
			{
				ProcessError(child);
				return false;
			}
			tempValue.ToLong(&price);
			if(price <= 0)
			{
				ProcessError(child);
				return false;
			}
			
			if(!scene.AddItemToSell(resMgr.GetItemAndDiscMgr().GetItemType(itemName), price))
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

bool LoneWolfXmlReader::ParseSceneEvents(wxXmlNode* sceneEventsNode, GlobalResourceManager& resMgr, Scene& scene)
{
	EventBase *pEvent = NULL;
	wxXmlNode* child = sceneEventsNode->GetChildren();
	while(child)
	{
		if(!ParseSceneEvent(child, resMgr, pEvent))
		{
			return false;
		}
		
		if(pEvent)
		{
			scene.m_events.push_back(pEvent);
			pEvent = NULL;
		} else {
			ProcessError(child);
			return false;
		}
		
		child = child->GetNext();
	}
	
	return true;
}

bool LoneWolfXmlReader::ParseSceneEvent(wxXmlNode* eventNode, GlobalResourceManager& resMgr, EventBase*& pEvent)
{
	wxString tempValue;
	wxString nameValue;
	wxScopedPtr<Event> spEvent;
	wxScopedPtr<EventList> spEventList;
	EEventType eventType = EVENT_UNKNOWN;
	
	if(eventNode->GetName() == GENERAL_TAG_EVENT_STR)
	{
		if(!eventNode->GetAttribute(wxString(GENERAL_ATTR_NAME_STR), &nameValue))
		{
			ProcessError(eventNode);
			return false;
		}
		eventType = Convertor::ConvertEventNameToType(nameValue);
		
		switch(eventType)
		{
			case EVENT_ADD_ITEM_TO_CHARACTER:
			case EVENT_ADD_ITEM_TO_SCENE:
			case EVENT_ADD_GOLD_TO_CHARACTER:
			case EVENT_ADD_GOLD_TO_SCENE:
			case EVENT_DEAD:
			case EVENT_CHARACTER:
			case EVENT_REMOVE_ITEM_FROM_CHARACTER:
			{
				spEvent.reset(new (std::nothrow) Event());
				if(spEvent.get() == NULL) 
				{
					ProcessError(eventNode);
					return false;
				}
				
				spEvent->m_type = eventType;
				
				if(!ReadEventAttr(eventNode, resMgr, spEvent->m_property))
				{
					return false;
				}
				
				pEvent = spEvent.release();
				break;
			}
			case EVENT_LOTERY:
			{
				wxXmlNode* selectNode = NULL;
				EventBase* pEventSelect = NULL;
				long unsigned int loteryFrom = 0;
				long unsigned int loteryTo = 0;
				spEventList.reset(new(std::nothrow) EventList());
				if(spEventList.get() == NULL) 
				{
					ProcessError(eventNode);
					return false;
				}
				spEventList->m_type = eventType;
				selectNode = eventNode->GetChildren();
				
				while(selectNode != NULL)
				{
					if(selectNode->GetName() != GENERAL_TAG_SELECTION_STR)
					{
						ProcessError(eventNode);
						return false;
					}
					
					// read selection numbers
					tempValue.Clear();
					if(!selectNode->GetAttribute(GENERAL_ATTR_NUMBERS_FROM_STR, &tempValue))
					{
						ProcessError(selectNode);
						return false;
					}
					tempValue.ToULong(&loteryFrom);
					
					tempValue.Clear();
					if(!selectNode->GetAttribute(GENERAL_ATTR_NUMBERS_TO_STR, &tempValue))
					{
						ProcessError(selectNode);
						return false;
					}
					tempValue.ToULong(&loteryTo);
					
					if(selectNode->GetChildren() == NULL)
					{
						ProcessError(selectNode);
						return false;
					}
					
					if(selectNode->GetChildren()->GetNext() == NULL)
					{
						// it has only one child
						if(!ParseSceneEvent(selectNode->GetChildren(), resMgr, pEventSelect))
						{
							return false;
						}
					} else {
						wxScopedPtr<EventList> spEventList2;
						spEventList2.reset(new (std::nothrow) EventList());
						spEventList2->m_type = EVENT_LIST;
						wxXmlNode* eventSelectNode = selectNode->GetChildren();
						while(eventSelectNode) 
						{
							EventBase* pEventBase = NULL;
							if(!ParseSceneEvent(eventSelectNode, resMgr, pEventBase))
							{
								return false;
							}
							spEventList2->m_eventList.push_back(pEventBase);
							eventSelectNode = eventSelectNode->GetNext();
						}
						pEventSelect = spEventList2.release();						
					}
					
					if(!spEventList->SetLoteryEvents(loteryFrom, loteryTo, pEventSelect))
					{
						ProcessError(selectNode);
						return false;
					}
					
					selectNode = selectNode->GetNext();
				}
				
				pEvent = spEventList.release();
				break;
			}
			case EVENT_UNKNOWN:
			default:
				ProcessError(eventNode);
				return false;
		}
		
	} else {
		ProcessError(eventNode);
		return false;
	}
	
	return true;
}

bool LoneWolfXmlReader::ParseChapter(wxXmlNode* chapterNode, GlobalResourceManager& resMgr, SceneManager& sceneMgr)
{
	wxString nameValue;
	wxString descValue;
	wxString tempValue;
	long int id = 0;
	wxXmlNode *child = chapterNode->GetChildren();
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
			if(!ParseScene(child, resMgr, sceneMgr))
			{
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

void LoneWolfXmlReader::ProcessError(wxXmlNode* errNode, const wxChar* addText)
{
	m_errorString.Clear();
	wxStringOutputStream outStream(&m_errorString);
	wxStdOutputStream output(outStream);

	wxXmlAttribute *attributes = errNode->GetAttributes();
	output << "***** ERROR *****\n";
	output << "Error on line: " << errNode->GetLineNumber() << "\n";
	output << "Name of node: " << errNode->GetName() << "\n";
	output << "Node attributes: \n";
	while (attributes)
	{
		output << "  " << attributes->GetName() << ":\"" << attributes->GetValue() << "\"\n";
		attributes = attributes->GetNext();
	}
	
	if(addText)
	{
		output << "Error description: " << wxString(addText).c_str() << "\n";
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
	
	if(xmlDoc.GetRoot()->GetName() == GENERAL_TAG_DEFINITIONS_STR)
	{
		if(!ParseDefinitions(xmlDoc.GetRoot(), resMgr))
		{
			return false;
		}
			
	} else {
		return false;
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
				return false;
			}
		} else if(child->GetName() == GENERAL_TAG_DISCIPLINES_STR) {
			if(!ParseDefDisciplines(child, resMgr))
			{
				return false;
			}
		} else if(child->GetName() == GENERAL_TAG_ITEMS_STR) {
			if(!ParseDefItems(child, resMgr))
			{
				return false;
			}
		} else if(child->GetName() == GENERAL_TAG_CHAPTERS_STR) {
			if(!ParseDefChapters(child, resMgr))
			{
				return false;
			}
		} else  {
			ProcessError(child);
			return false;
		}
		
		child = child->GetNext();
	}
	
	return true;
}

bool LoneWolfXmlReader::ParseDefDisciplines(wxXmlNode* defNode, GlobalResourceManager& resMgr)
{
	wxString idValue;
	wxString nameValue;
	wxString descValue;
	wxString titleValue;
	wxString addinfoValue;
	long int discId = 0;
	wxXmlNode *child = defNode->GetChildren();
	while(child)
	{
		if(child->GetName() == GENERAL_TAG_SKILL_STR)
		{		
			Discipline disc;
			wxXmlNode* textNode = child->GetChildren();
			// read id
			idValue.Clear();
			if(!child->GetAttribute(wxString(GENERAL_ATTR_ID_STR), &idValue))
			{
				ProcessError(child);
				return false;
			}
			idValue.ToLong(&discId);
			disc.m_id = discId;
			
			// read name
			nameValue.Clear();
			if(!child->GetAttribute(wxString(GENERAL_ATTR_NAME_STR), &nameValue))
			{
				ProcessError(child);
				return false;
			}
			
			if(!ReadDisciplineAttr(child, resMgr, disc.m_property))
			{
				return false;
			}
			
			// read title and description
			while(textNode)
			{
				wxString text;
				wxString lang;
				if(!ReadText(textNode, text, lang))
				{
					return false;
				}
				
				if(lang == m_language)
				{
					if(textNode->GetName() == GENERAL_TAG_TITLE_STR)
					{
						disc.m_title = text;
					} else if(textNode->GetName() == GENERAL_TAG_DESC_STR) {
						disc.m_desc = text;
					}
				}
				textNode = textNode->GetNext();
			}
		
			// create new action definition
			if(!resMgr.GetDisciplineMgr().AddDiscipline(nameValue, disc))
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

bool LoneWolfXmlReader::ParseDefItems(wxXmlNode* defNode, GlobalResourceManager& resMgr)
{
	wxXmlNode *child = defNode->GetChildren();
	while(child)
	{
		
		if(child->GetName() == GENERAL_TAG_ITEM_STR)
		{
			if(!ReadItemAttr(child, resMgr))
			{
				return false;
			}
		} else if(child->GetType() != wxXML_COMMENT_NODE) {
			ProcessError(child);
			return false;
		}
		
		child = child->GetNext();
	}
	
	return true;
}


bool LoneWolfXmlReader::ParseDefActions(wxXmlNode* defNode, GlobalResourceManager& resMgr)
{
	wxString descValue;
	wxXmlNode *child = defNode->GetChildren();
	while(child)
	{
		if((child->GetName() == GENERAL_TAG_ACTION_CREATE_STR)
			||(child->GetName() == GENERAL_TAG_ACTION_LOTERY_STR)
			||(child->GetName() == GENERAL_TAG_ACTION_MOVE_STR)
			||(child->GetName() == GENERAL_TAG_ACTION_FIGHT_STR))
		{
			// read attributes
			descValue.Clear();
			wxXmlNode *actionText = child->GetChildren();
			while(actionText)
			{
				if(actionText->GetName() == GENERAL_TAG_DESC_STR)
				{
					wxString lang;
					wxString text;
					if(!ReadText(actionText, text, lang))
					{
						return false;
					}
					// if text has correct language
					if(lang == m_language)
					{
						descValue = text;
					}
				} else {
					ProcessError(actionText);
					return false;					
				}
				
				actionText = actionText->GetNext();
			}
			
			// create new action definition
			if(Convertor::ConvertActionNameToType(child->GetName()) != ACTION_UNKNOWN)
			{
				resMgr.GetActionMgr().SetDefaultActionDesc(Convertor::ConvertActionNameToType(child->GetName()), descValue);
			}
			
		} else {
			ProcessError(child);
			return false;
		}
		
		child = child->GetNext();
	}
	
	return true;
}

bool LoneWolfXmlReader::ParseDefChapters(wxXmlNode* defNode, GlobalResourceManager& resMgr)
{
	wxString title;
	wxString filename;
	long int idVal = 0;
	wxXmlNode* child = defNode->GetChildren();
	while(child)
	{
		if(child->GetName() == GENERAL_TAG_CHAPTER_STR)
		{
			wxXmlNode* textNode = child->GetChildren();
			title.Clear();
			filename.Clear();
			idVal = 0;
			
			if(!child->GetAttribute(wxString(GENERAL_ATTR_ID_STR), &title))
			{
				ProcessError(child);
				return false;
			}
			title.ToLong(&idVal);
			
			title.Clear();
			if(!child->GetAttribute(wxString(GENERAL_ATTR_FILENAME_STR), &filename))
			{
				ProcessError(child);
				return false;
			}
			
			while(textNode)
			{
				if(textNode->GetName() == GENERAL_TAG_TITLE_STR)
				{
					wxString text;
					wxString lang;
					if(!ReadText(textNode, text, lang))
					{
						return false;
					}
					
					if(lang == m_language)
					{
						title = text;
					}
				}
				textNode = textNode->GetNext();
			}
			
			if(!resMgr.GetChapterMgr().AddChapter(idVal, title, filename))
			{
				ProcessError(child);
				return false;
			}
			
		}
		child = child->GetNext();
	}
	
	return true;
}


bool LoneWolfXmlReader::ReadItemAttr(wxXmlNode* itemNode, GlobalResourceManager& resMgr)
{
	Item item;
	wxString idValue;
	wxString nameValue;
	wxString placementValue;
	wxString tempValue;
	wxXmlNode* textNode = NULL;
	long int intValue = 0;
		
	if(!itemNode->GetAttribute(wxString(GENERAL_ATTR_ID_STR), &idValue))
	{
		ProcessError(itemNode);
		return false;
	}
	idValue.ToLong(&intValue);
	item.m_id = intValue;

	if(!itemNode->GetAttribute(wxString(GENERAL_ATTR_NAME_STR), &nameValue))
	{
		ProcessError(itemNode);
		return false;
	}

	if(itemNode->HasAttribute(GENERAL_ATTR_COND_ACT_STR))
	{
		tempValue = itemNode->GetAttribute(GENERAL_ATTR_COND_ACT_STR);
		tempValue.ToLong(&intValue);
		item.m_properties.m_actualCond = static_cast<wxInt32>(intValue);
	}

	if(itemNode->HasAttribute(GENERAL_ATTR_COND_MAX_STR))
	{
		tempValue = itemNode->GetAttribute(GENERAL_ATTR_COND_MAX_STR);
		tempValue.ToLong(&intValue);
		item.m_properties.m_maxCond = static_cast<wxInt32>(intValue);
	}

	if(itemNode->HasAttribute(GENERAL_ATTR_ONE_USE_STR))
	{
		item.m_properties.m_oneUse = true;
	} else {
		item.m_properties.m_oneUse = false;
	}
	
	if(itemNode->HasAttribute(wxString(GENERAL_ATTR_FREE_USE_STR)))
	{
		item.m_properties.m_freeUse = true;
	} else {
		item.m_properties.m_freeUse = false;
	}

	if(!itemNode->GetAttribute(wxString(GENERAL_ATTR_PLACEMENT_STR), &placementValue))
	{
		ProcessError(itemNode);
		return false;
	}
	
	item.m_placement = Convertor::ConvertPlacementNameToType(placementValue);
	
	// read title and description in correct language
	textNode = itemNode->GetChildren();
	while(textNode)
	{
		wxString text;
		wxString lang;
		if(!ReadText(textNode, text, lang))
		{
			return false;
		}
		
		if(lang == m_language)
		{
			if(textNode->GetName() == GENERAL_TAG_TITLE_STR)
			{
				item.m_title = text;
			} else if(textNode->GetName() == GENERAL_TAG_DESC_STR) {
				item.m_desc = text;
			}
		}
		
		textNode = textNode->GetNext();
	}
	
	// create new item definition
	if(!resMgr.GetItemAndDiscMgr().AddItem(nameValue, item))
	{
		ProcessError(itemNode);
		return false;
	}
	


	return true;
}


bool LoneWolfXmlReader::ReadEventAttr(wxXmlNode* eventNode, GlobalResourceManager& resMgr, EventProperties& properties)
{
	wxString tempValue;
	long int intValue;
	
	if(eventNode->HasAttribute(GENERAL_ATTR_COND_ACT_STR))
	{
		tempValue = eventNode->GetAttribute(wxString(GENERAL_ATTR_COND_ACT_STR));
		tempValue.ToLong(&intValue);
		properties.m_actualCond = intValue;
	}
	
	if(eventNode->HasAttribute(GENERAL_ATTR_COND_MAX_STR))
	{	
		tempValue = eventNode->GetAttribute(wxString(GENERAL_ATTR_COND_MAX_STR));
		tempValue.ToLong(&intValue);
		properties.m_baseCond = intValue;
	}
	
	if(eventNode->HasAttribute(GENERAL_ATTR_ATTACK_ACT_STR))
	{
		tempValue = eventNode->GetAttribute(wxString(GENERAL_ATTR_ATTACK_ACT_STR));
		tempValue.ToLong(&intValue);
		properties.m_actualAttack = intValue;
	}
	
	if(eventNode->HasAttribute(GENERAL_ATTR_ATTACK_BASE_STR))
	{
		tempValue = eventNode->GetAttribute(wxString(GENERAL_ATTR_ATTACK_BASE_STR));
		tempValue.ToLong(&intValue);
		properties.m_baseAttack = intValue;
	}
	
	if(eventNode->HasAttribute(GENERAL_ATTR_DURATION_STR))
	{
		tempValue = eventNode->GetAttribute(wxString(GENERAL_ATTR_DURATION_STR));
		tempValue.ToLong(&intValue);
		properties.m_duration = intValue;
	}
	
	if(eventNode->HasAttribute(GENERAL_ATTR_GOLD_COUNT_STR))
	{
		tempValue = eventNode->GetAttribute(wxString(GENERAL_ATTR_GOLD_COUNT_STR));
		tempValue.ToLong(&intValue);
		properties.m_goldCount = intValue;
	}
	
	if(eventNode->HasAttribute(GENERAL_ATTR_CANCELED_BY_ITEM_STR))
	{
		tempValue = eventNode->GetAttribute(wxString(GENERAL_ATTR_CANCELED_BY_ITEM_STR));
		properties.m_cancelItem = resMgr.GetItemAndDiscMgr().GetItemType(tempValue);
		if(properties.m_cancelItem == ITEM_UNKNOWN)
		{
			ProcessError(eventNode, wxT("Unknown item name"));
			return false;
		}
	}
	
	if(eventNode->HasAttribute(GENERAL_ATTR_CANCELED_BY_SKILL_STR))
	{
		tempValue = eventNode->GetAttribute(wxString(GENERAL_ATTR_CANCELED_BY_SKILL_STR));
		properties.m_cancelSkill = resMgr.GetDisciplineMgr().GetDisciplineType(tempValue);
		if(properties.m_cancelSkill == DISCIPLINE_UNKNOWN)
		{
			ProcessError(eventNode, wxT("Unknown skill name"));
			return false;
		}
	}
	
	if(eventNode->HasAttribute(GENERAL_ATTR_ITEM_NEEDED_STR))
	{
		tempValue = eventNode->GetAttribute(wxString(GENERAL_ATTR_ITEM_NEEDED_STR));
		properties.m_neededItem = resMgr.GetItemAndDiscMgr().GetItemType(tempValue);
		if(properties.m_neededItem == ITEM_UNKNOWN)
		{
			ProcessError(eventNode, wxT("Unknown item name"));
			return false;
		}
	}
	
	if(eventNode->HasAttribute(GENERAL_ATTR_SKILL_NEEDED_STR))
	{
		tempValue = eventNode->GetAttribute(wxString(GENERAL_ATTR_SKILL_NEEDED_STR));
		properties.m_neededSkill = resMgr.GetDisciplineMgr().GetDisciplineType(tempValue);
		if(properties.m_neededSkill == DISCIPLINE_UNKNOWN)
		{
			ProcessError(eventNode, wxT("Unknown skill name"));
			return false;
		}
	}

	
	return true;
}

bool LoneWolfXmlReader::ReadDisciplineAttr(wxXmlNode* discNode, GlobalResourceManager& resMgr, DisciplineProperties& properties)
{
		wxString tempValue;
	long int intValue;
	
	if(discNode->HasAttribute(wxString(GENERAL_ATTR_COND_ACT_STR)))
	{
		tempValue = discNode->GetAttribute(wxString(GENERAL_ATTR_COND_ACT_STR));
		tempValue.ToLong(&intValue);
		properties.m_actualCond = intValue;
	}
	
	if(discNode->HasAttribute(wxString(GENERAL_ATTR_ATTACK_ACT_STR)))
	{
		tempValue = discNode->GetAttribute(wxString(GENERAL_ATTR_ATTACK_ACT_STR));
		tempValue.ToLong(&intValue);
		properties.m_actualAttack = intValue;
	}
	
	if(discNode->HasAttribute(wxString(GENERAL_ATTR_CANCELED_BY_ITEM_STR)))
	{
		tempValue = discNode->GetAttribute(wxString(GENERAL_ATTR_CANCELED_BY_ITEM_STR));
		properties.m_cancelItem = resMgr.GetItemAndDiscMgr().GetItemType(tempValue);
		if(properties.m_cancelItem == ITEM_UNKNOWN)
		{
			ProcessError(discNode);
			return false;
		}
	}
	
	if(discNode->HasAttribute(wxString(GENERAL_ATTR_CANCELED_BY_SKILL_STR)))
	{
		tempValue = discNode->GetAttribute(wxString(GENERAL_ATTR_CANCELED_BY_SKILL_STR));
		properties.m_cancelSkill = resMgr.GetDisciplineMgr().GetDisciplineType(tempValue);
		if(properties.m_cancelSkill == DISCIPLINE_UNKNOWN)
		{
			ProcessError(discNode);
			return false;
		}
	}
	
	if(discNode->HasAttribute(wxString(GENERAL_ATTR_ITEM_NEEDED_STR)))
	{
		tempValue = discNode->GetAttribute(wxString(GENERAL_ATTR_ITEM_NEEDED_STR));
		properties.m_neededItem = resMgr.GetItemAndDiscMgr().GetItemType(tempValue);
		if(properties.m_neededItem == ITEM_UNKNOWN)
		{
			ProcessError(discNode);
			return false;
		}
	}
	
	if(discNode->HasAttribute(wxString(GENERAL_ATTR_SKILL_NEEDED_STR)))
	{
		tempValue = discNode->GetAttribute(wxString(GENERAL_ATTR_SKILL_NEEDED_STR));
		properties.m_neededSkill = resMgr.GetDisciplineMgr().GetDisciplineType(tempValue);
		if(properties.m_neededSkill == DISCIPLINE_UNKNOWN)
		{
			ProcessError(discNode);
			return false;
		}
	}
	
	if(discNode->HasAttribute(wxString(GENERAL_ATTR_NON_FIGTH_SKILL_STR)))
	{
		properties.m_fightSkill = false;
	} else {
		properties.m_fightSkill = true;
	}

	
	return true;
}

bool LoneWolfXmlReader::ReadText(wxXmlNode* textNode, wxString& text, wxString& language)
{
	if((textNode->GetName() == GENERAL_TAG_TITLE_STR)
		||(textNode->GetName() == GENERAL_TAG_DESC_STR))
	{	
		if(!textNode->GetAttribute(wxString(GENERAL_ATTR_LANGUAGE_STR), &language))
		{
			ProcessError(textNode);
			return false;
		}
		
		if((textNode->GetChildren() != NULL)&&(textNode->GetChildren()->GetType() == wxXML_TEXT_NODE))
		{
			text = textNode->GetChildren()->GetContent();
		} else {
			ProcessError(textNode);
			return false;
		}
		
	} else {
		ProcessError(textNode);
		return false;
	}
	
	return true;
}



