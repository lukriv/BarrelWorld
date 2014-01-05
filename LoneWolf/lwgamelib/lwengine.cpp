#include "lwengine.h"
#include <cstdlib>
#include <time.h>
#include "reader.h"
#include "lwxmldefs.h"


bool LWGameEngine::Initialize(LWGameEngineCallback* pCallback)
{
	
	if (pCallback == NULL)
	{
		m_errorStr.assign(wxT("Callback function cannot be NULL"));
		return false;
	}
	m_pUserInteractionCallback = pCallback;
	
	// initialize random generator
	srand(time(NULL));
	
	if(!LoneWolfXmlReader::LoadTextDefinitions(wxT("../../chapters/textdefs.xml"), m_resMgr))
	{
		m_errorStr.assign(wxT("Chyba pri cteni souboru ../../chapters/textdefs.xml\n"));
		m_errorStr.Append(LoneWolfXmlReader::GetLastErrorString());
		return false;
	}
	
	if(!LoneWolfXmlReader::LoadChapter(wxT("../../chapters/utokzetmy.xml"), m_resMgr, m_sceneMgr))
	{
		m_errorStr.assign(wxT("Chyba pri cteni souboru ../../chapters/utokzetmy.xml\n"));
		m_errorStr.Append(LoneWolfXmlReader::GetLastErrorString());
		return false;		
	}
	

	
	//if(scnMgr.SceneMapTest())
	//{
	//	m_errorStr.assign(wxT("Scene map test failed"));
	//	return false;
	//}
	
	if(!m_mainCharacter.Initialize(&m_resMgr))
	{
		m_errorStr.assign(wxT("Initialize new character failed"));
		return false;
	}

	if(!m_mainCharacter.GenerateNewCharacter())
	{
		m_errorStr.assign(wxT("Create new character failed"));
		return false;
	}
	
	m_pActualScene = m_sceneMgr.GetScene(0);
	
	if(m_pActualScene == NULL)
	{
		m_errorStr.assign(wxT("No scene with id 0 found"));
		return false;
	}
	
	return true;
	
}

bool LWGameEngine::RunAction(wxDword actionIndex)
{
	wxVector<wxDword> posibleActions;
	wxVector<wxDword>::iterator iter;
	
	//check if action is enabled
	m_pActualScene->GetPosibleActions(m_mainCharacter, posibleActions);
	for( iter = posibleActions.begin(); iter != posibleActions.end(); iter++)
	{
		if(actionIndex == *iter) break;
	}
	
	if(iter == posibleActions.end()||actionIndex > m_pActualScene->m_actions.size())
	{
		return true;
	}
	
	// action is enabled -> do action
	switch(m_pActualScene->m_actions[actionIndex].GetType())
	{
		case ACTION_CREATE_CHAR:
			if(!CreateNewCharacter())
			{
				return false;
			}
			m_pActualScene = m_sceneMgr.GetScene(m_pActualScene->m_actions[actionIndex].GetMoveTarget());
			break;
		case ACTION_MOVE:
			{
				wxDword nextTarget = m_pActualScene->m_actions[actionIndex].GetMoveTarget();
				m_pActualScene = m_sceneMgr.GetScene(nextTarget);
				if(m_pActualScene == NULL)
				{
					m_errorStr.Printf(wxT("Scene %u cannot be found"), nextTarget);
					return false;
				}
				break;
			}
		case ACTION_LOTERY:
			break;
		default:
			m_errorStr.assign(wxT("No actions found"));
			return false;
		break;
	}
	
	return true;
}

bool LWGameEngine::CreateNewCharacter()
{
	std::set<EDisciplines> chosenDisc;
	bool okState = true;

	
	m_mainCharacter.GetWeapons().SetMaxCount(2);
	m_mainCharacter.GetSpecialItems().SetMaxCount(10);
	
	m_pUserInteractionCallback->SelectDisciplines(5, chosenDisc);
	
	for (std::set<EDisciplines>::iterator iter = chosenDisc.begin(); iter != chosenDisc.end(); iter++)
	{
		if(!AddNewCharacterDiscipline(*iter))
		{
			return false;
		}
	}
	
	return true;
}
bool LWGameEngine::AddNewCharacterDiscipline(EDisciplines disc)
{
	EventProperties *discProp = NULL;
	if(!m_resMgr.GetDisciplineMgr().DisciplineExists(disc)) 
	{
		m_errorStr.assign(wxT("Discipline does not exists"));
		return false;
	}
	
	if(!m_mainCharacter.GetDisciplines().Add(disc, m_resMgr.GetDisciplineMgr().GetDiscipline(disc).m_property))
	{
		m_errorStr.assign(wxT("Add discipline to character failed"));
		return false;
	}
	discProp = m_mainCharacter.GetDisciplines().FindValue(disc);
	// if some discipline has random weapon type as required item choose one weapon for it
	if((discProp != NULL)&&(discProp->m_neededItem == m_resMgr.GetItemAndDiscMgr().GetRandomWeaponType()))
	{
		switch(RandomSpin())
		{
			case 0:
				discProp->m_neededItem = m_resMgr.GetItemAndDiscMgr().GetItemType(wxString(WEAPON_DAGGER_STR));
				break;
			case 1:
				discProp->m_neededItem = m_resMgr.GetItemAndDiscMgr().GetItemType(wxString(WEAPON_SPEAR_STR));
				break;
			case 2:
				discProp->m_neededItem = m_resMgr.GetItemAndDiscMgr().GetItemType(wxString(WEAPON_MACE_STR));;
				break;
			case 3: 
				discProp->m_neededItem = m_resMgr.GetItemAndDiscMgr().GetItemType(wxString(WEAPON_SHORT_SWORD_STR));;
				break;
			case 4:
				discProp->m_neededItem = m_resMgr.GetItemAndDiscMgr().GetItemType(wxString(WEAPON_WARHAMMER_STR));
				break;
			case 5:
			case 7:
				discProp->m_neededItem = m_resMgr.GetItemAndDiscMgr().GetItemType(wxString(WEAPON_SWORD_STR));
				break;
			case 6:
				discProp->m_neededItem = m_resMgr.GetItemAndDiscMgr().GetItemType(wxString(WEAPON_AXE_STR));
				break;
			case 8:
				discProp->m_neededItem = m_resMgr.GetItemAndDiscMgr().GetItemType(wxString(WEAPON_QUATERSTAFF_STR));
				break;
			case 9:
				discProp->m_neededItem = m_resMgr.GetItemAndDiscMgr().GetItemType(wxString(WEAPON_BROADSWORD_STR));
				break;
			default:
				return false;
		}
	}
	
	return true;
}

bool LWGameEngine::RunEvents()
{
	for(EventVector::iterator iter = m_pActualScene->m_events.begin(); iter != m_pActualScene->m_events.end(); iter++)
	{
		switch((*iter)->GetEventType())
		{
			case EVENT_ADD_ITEM_TO_CHARACTER:
				if(!m_mainCharacter.AddItem((*iter)->GetProperties()->m_neededItem))
				{
					m_errorStr.assign(wxT("Add item to character failed (during events)"));
					return false;
				}
				break;
			case EVENT_ADD_ITEM_TO_SCENE:
				m_pActualScene->AddItem((*iter)->GetProperties()->m_neededItem);
				break;
			case EVENT_ADD_GOLD_TO_CHARACTER:
			case EVENT_ADD_GOLD_TO_SCENE:
				if(!m_pActualScene->AddGold((*iter)->GetProperties()->m_goldCount))
				{
					m_errorStr.assign(wxT("Add gold to scene failed (during events)"));
					return false;
				}
				break;
			case EVENT_LOTERY:
				break;
			case EVENT_UNKNOWN:
			default:
				return false;
		}
	}
	return true;
}
