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
	
	m_mainCharacter.SetCharacterName(wxString(wxT("LoneWolf")));
	
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
	
	if(iter == posibleActions.end()
		||actionIndex > m_pActualScene->m_actions.size()
		||(m_mainCharacter.GetActualConditions() <= 0)) // character is dead
	{
		return true;
	}
	
	// action is enabled -> do action
	switch(m_pActualScene->m_actions[actionIndex]->GetType())
	{
		case ACTION_CREATE_CHAR:
		{
			Action *pAction = static_cast<Action*>(m_pActualScene->m_actions[actionIndex]);
			if(!CreateNewCharacter())
			{
				return false;
			}
			m_pActualScene = m_sceneMgr.GetScene(pAction->GetMoveTarget());
			break;
		}
		case ACTION_MOVE:
		{
			Action *pAction = static_cast<Action*>(m_pActualScene->m_actions[actionIndex]);
			wxInt32 nextTarget = pAction->GetMoveTarget();
			m_pActualScene = m_sceneMgr.GetScene(nextTarget);
			if(m_pActualScene == NULL)
			{
				m_errorStr.Printf(wxT("Scene %u cannot be found"), nextTarget);
				return false;
			}
			break;
		}
		case ACTION_LOTERY:
		{
			Action *pAction = static_cast<Action*>(m_pActualScene->m_actions[actionIndex]);
			wxInt32 nextTarget = pAction->GetLoteryTarget(RandomSpin()); 
			m_pActualScene = m_sceneMgr.GetScene(nextTarget);
			if(m_pActualScene == NULL)
			{
				m_errorStr.Printf(wxT("Scene %u cannot be found"), nextTarget);
				return false;
			}
			break;
		}
		case ACTION_FIGHT:
		{
			ActionFight* pActionFight = static_cast<ActionFight*>(m_pActualScene->m_actions[actionIndex]);
			wxInt32 nextTarget = pActionFight->StartFight(m_mainCharacter, m_pUserInteractionCallback);
			if(nextTarget != TARGET_UNKNOWN)
			{
				m_pActualScene = m_sceneMgr.GetScene(nextTarget);
				if(m_pActualScene == NULL)
				{
					m_errorStr.Printf(wxT("Scene %u cannot be found"), nextTarget);
					return false;
				}
			}
			break;
		}
		default:
			m_errorStr.assign(wxT("No actions found"));
			return false;
		break;
	}
	
	if(m_mainCharacter.GetActualConditions() > 0)
	{
		if(!RunSceneEvents())
		{
			return false;
		}		
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
		if(!m_mainCharacter.AddNewCharacterDiscipline(*iter))
		{
			m_errorStr.assign(wxT("Add new character disciplines failed"));
			return false;
		}
	}
	
	return true;
}

bool LWGameEngine::RunSceneEvents()
{
	for(EventVector::iterator iter = m_pActualScene->m_events.begin(); iter != m_pActualScene->m_events.end(); iter++)
	{
		if(!RunEvent(*iter))
		{
			return false;
		}
	}
	return true;
}

bool LWGameEngine::RunEvent(EventBase* pEvent)
{
	if(pEvent == NULL)
	{
		m_errorStr.assign(wxT("RunEvent() parameter cannot be NULL\n"));
		return false;	
	}
	
	switch(pEvent->GetEventType())
	{
		case EVENT_ADD_ITEM_TO_CHARACTER:
			if(!m_mainCharacter.AddItem(pEvent->GetProperties()->m_neededItem))
			{
				m_errorStr.assign(wxT("Add item to character failed (during events)"));
				return false;
			}
			break;
		case EVENT_ADD_ITEM_TO_SCENE:
			m_pActualScene->AddItem(pEvent->GetProperties()->m_neededItem);
			break;
		case EVENT_ADD_GOLD_TO_CHARACTER:
		case EVENT_ADD_GOLD_TO_SCENE:
			if(!m_pActualScene->AddGold(pEvent->GetProperties()->m_goldCount))
			{
				m_errorStr.assign(wxT("Add gold to scene failed (during events)"));
				return false;
			}
			break;
		case EVENT_LOTERY:
			if(!RunEvent(pEvent->GetRandomEvent(RandomSpin())))
			{
				return false;
			}
			break;
		case EVENT_LIST:
		{
			EventList *pEventList = static_cast<EventList*>(pEvent);
			EventVector::iterator iter;
			for(iter = pEventList->m_eventList.begin(); iter != pEventList->m_eventList.end(); iter++)
			{
				if(!RunEvent(*iter))
				{
					return false;
				}
			}
			break;
		}
		case EVENT_DEAD:
		{
			// kill main character
			m_mainCharacter.AddActualCondition(-m_mainCharacter.GetActualConditions());
			break;
		}
		case EVENT_UNKNOWN:
		default:
			return false;
	}
	return true;
}
