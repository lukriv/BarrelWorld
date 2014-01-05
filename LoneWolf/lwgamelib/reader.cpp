#include "reader.h"

#include <iostream>
#include <wx/sstream.h>
#include <wx/stdstream.h>
#include <wx/xml/xml.h>
#include "lwxmldefs.h"
#include <wx/scopedptr.h>

wxString LoneWolfXmlReader::m_errorString = wxString();

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
	scene.SetSceneId(readedNumber);
			
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
			if(!ParseSceneActions(child, resMgr, scene))
			{
				ProcessError(child);
				return false;
			}
		} else if (child->GetName() == GENERAL_TAG_ITEMS_STR) {
			if(!ParseSceneItems(child, resMgr, scene))
			{
				ProcessError(child);
				return false;
			}
		} else if (child->GetName() == GENERAL_TAG_EVENTS_STR) {
			if(!ParseSceneEvents(child, resMgr, scene))
			{
				ProcessError(child);
				return false;
			}
		}
		
		child = child->GetNext();
	}
	
	if(!sceneMgr.AddScene(scene))
	{
		ProcessError(sceneNode);
		return false;
	}
	
	return true;
}

bool LoneWolfXmlReader::ParseSceneActions(wxXmlNode* sceneActionNode, GlobalResourceManager& resMgr, Scene& scene)
{
	long unsigned int loteryFrom = 0, loteryTo = 0;
	long unsigned int target = 0;
	wxString tempValue;
	wxString nameValue;
	wxString requiredSkillValue;
	wxString requiredItemValue;
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
			
			action.SetCondition(child->HasAttribute(GENERAL_ATTR_CONDITIONED_STR));
			
			requiredSkillValue = child->GetAttribute(wxString(GENERAL_ATTR_SKILL_NEEDED_STR));
			if(resMgr.GetDisciplineMgr().GetDisciplineType(requiredSkillValue) != DISCIPLINE_UNKNOWN)
			{
				action.SetRequiredSkill(resMgr.GetDisciplineMgr().GetDisciplineType(requiredSkillValue));
			}
			
			requiredItemValue = child->GetAttribute(wxString(GENERAL_ATTR_ITEM_NEEDED_STR));
			if(resMgr.GetItemAndDiscMgr().GetItemType(requiredItemValue) != ITEM_UNKNOWN)
			{
				action.SetRequiredItem(resMgr.GetItemAndDiscMgr().GetItemType(requiredItemValue));
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


bool LoneWolfXmlReader::ParseSceneItems(wxXmlNode* sceneActionNode, GlobalResourceManager& resMgr, Scene& scene)
{
	wxString tempValue;
	wxString nameValue;
	wxXmlNode* child = sceneActionNode->GetChildren();
	while(child)
	{
		if((child->GetName() == GENERAL_TAG_WEAPON_STR)||(child->GetName() == GENERAL_TAG_ITEM_STR))
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

bool LoneWolfXmlReader::ParseSceneEvents(wxXmlNode* sceneEventsNode, GlobalResourceManager& resMgr, Scene& scene)
{
	wxString tempValue;
	wxString nameValue;
	wxXmlNode* child = sceneEventsNode->GetChildren();
	wxXmlNode* eventLoteryNode = NULL;
	EEventType eventType = EVENT_UNKNOWN;
	wxScopedPtr<Event> spEvent;
	wxScopedPtr<EventList> spEventList;
	while(child)
	{
		if(child->GetName() == GENERAL_TAG_EVENT_STR)
		{
			if(!child->GetAttribute(wxString(GENERAL_ATTR_NAME_STR), &nameValue))
			{
				ProcessError(child);
				return false;
			}
			eventType = Convertor::ConvertEventNameToType(nameValue);
			
			switch(eventType)
			{
				case EVENT_ADD_ITEM_TO_CHARACTER:
				case EVENT_ADD_ITEM_TO_SCENE:
				case EVENT_ADD_GOLD_TO_CHARACTER:
				case EVENT_ADD_GOLD_TO_SCENE:
					{
						spEvent.reset(new (std::nothrow) Event());
						if(spEvent.get() == NULL) 
						{
							ProcessError(child);
							return false;
						}
						
						spEvent->m_type = eventType;
						
						if(!ReadEventAttr(child, resMgr, spEvent->m_property))
						{
							return false;
						}
						
						scene.m_events.push_back(spEvent.release());
					}
					break;
				case EVENT_LOTERY:
					{
						long unsigned int loteryFrom = 0;
						long unsigned int loteryTo = 0;
						spEventList.reset(new(std::nothrow) EventList());
						if(spEventList.get() == NULL) 
						{
							ProcessError(child);
							return false;
						}
						spEventList->m_type = eventType;
						eventLoteryNode = child->GetChildren();
						
						while(eventLoteryNode != NULL)
						{
							Event event;
							if(!eventLoteryNode->GetAttribute(wxString(GENERAL_ATTR_NAME_STR), &nameValue))
							{
								ProcessError(child);
								return false;
							}
							
							event.m_type = Convertor::ConvertEventNameToType(nameValue);
							if(event.m_type == EVENT_UNKNOWN)
							{
								ProcessError(eventLoteryNode);
								return false;
							}
							
							if(!ReadEventAttr(eventLoteryNode, resMgr, event.m_property))
							{
								return false;
							}
							
							tempValue.Clear();
							if(!eventLoteryNode->GetAttribute(GENERAL_ATTR_NUMBERS_FROM_STR, &tempValue))
							{
								ProcessError(eventLoteryNode);
								return false;
							}
							tempValue.ToULong(&loteryFrom);
							
							tempValue.Clear();
							if(!eventLoteryNode->GetAttribute(GENERAL_ATTR_NUMBERS_TO_STR, &tempValue))
							{
								ProcessError(eventLoteryNode);
								return false;
							}
							tempValue.ToULong(&loteryTo);
							
							spEventList->SetLoteryEvents(loteryFrom, loteryTo, event);
							
							eventLoteryNode = eventLoteryNode->GetNext();
						}
						
						scene.m_events.push_back(spEventList.release());
					}
					break;
				case EVENT_UNKNOWN:
				default:
					ProcessError(child);
					return false;
			}
			
		}
		child = child->GetNext();
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
			if(!ParseScene(child, resMgr, sceneMgr))
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
			
			disc.m_title = child->GetAttribute(wxString(GENERAL_ATTR_TITLE_STR));
			disc.m_desc = child->GetAttribute(wxString(GENERAL_ATTR_DESC_STR));
			
			if(!ReadEventAttr(child, resMgr, disc.m_property))
			{
				return false;
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
		} else if(child->GetName() == GENERAL_TAG_WEAPON_STR) {
			if(!ReadItemAttr(child, resMgr))
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

bool LoneWolfXmlReader::ReadItemAttr(wxXmlNode* itemNode, GlobalResourceManager& resMgr)
{
	Item item;
	wxString idValue;
	wxString nameValue;
	wxString placementValue;
	wxString tempValue;
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

	item.m_title = itemNode->GetAttribute(wxString(GENERAL_ATTR_TITLE_STR));
	item.m_desc = itemNode->GetAttribute(wxString(GENERAL_ATTR_DESC_STR));


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

	

	if(!itemNode->GetAttribute(wxString(GENERAL_ATTR_PLACEMENT_STR), &placementValue))
	{
		ProcessError(itemNode);
		return false;
	}
	
	item.m_placement = Convertor::ConvertPlacementNameToType(placementValue);
	
		
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
		properties.m_maxCond = intValue;
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
	}
	
	if(eventNode->HasAttribute(GENERAL_ATTR_CANCELED_BY_SKILL_STR))
	{
		tempValue = eventNode->GetAttribute(wxString(GENERAL_ATTR_CANCELED_BY_SKILL_STR));
		properties.m_cancelSkill = resMgr.GetDisciplineMgr().GetDisciplineType(tempValue);
	}
	
	if(eventNode->HasAttribute(GENERAL_ATTR_ITEM_NEEDED_STR))
	{
		tempValue = eventNode->GetAttribute(wxString(GENERAL_ATTR_ITEM_NEEDED_STR));
		properties.m_neededItem = resMgr.GetItemAndDiscMgr().GetItemType(tempValue);
	}
	
	if(eventNode->HasAttribute(GENERAL_ATTR_SKILL_NEEDED_STR))
	{
		tempValue = eventNode->GetAttribute(wxString(GENERAL_ATTR_SKILL_NEEDED_STR));
		properties.m_neededSkill = resMgr.GetDisciplineMgr().GetDisciplineType(tempValue);
	}

	
	return true;
}

