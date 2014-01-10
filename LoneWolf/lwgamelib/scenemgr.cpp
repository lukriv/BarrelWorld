#include "scenemgr.h"
#include <iostream>
#include <list>

#include "character.h"
#include "fight.h"


std::ostream& operator<< (std::ostream &output, const Scene& scene) 
{
	output << "--------------------------\n";
	output << "Scene ID: " << scene.GetSceneId() << "\n";
	output << "Desc: " << scene.m_desc.ToStdString() << "\n";
	output << "Actions: \n";
	for(ActionVector::const_iterator iter = scene.m_actions.begin(); iter != scene.m_actions.end(); iter++)
	{
		output << *iter;
	}
	
	return output;
}

std::ostream& operator<< (std::ostream &output, const SceneManager& sceneMgr)
{
	SceneManager::TSceneMap::const_iterator iter;
	for(iter = sceneMgr.m_sceneMap.begin(); iter != sceneMgr.m_sceneMap.end(); iter++)
	{
		output << iter->second;
	}
	return output;
}

std::wostream& operator<< (std::wostream &output, const Scene& scene)
{
	output << L"--------------------------\n";
	output << L"Scene ID: " << scene.GetSceneId() << "\n";
	output << L"Desc: " << scene.m_desc.ToStdWstring() << "\n";
	
	return output;
}

std::wostream& operator<< (std::wostream &output, const SceneManager& sceneMgr)
{
	SceneManager::TSceneMap::const_iterator iter;
	for(iter = sceneMgr.m_sceneMap.begin(); iter != sceneMgr.m_sceneMap.end(); iter++)
	{
		output << iter->second;
	}
	return output;
}
//////////////////////////////////////////////
/////////////// Scene ////////////////////////
//////////////////////////////////////////////
void Scene::GetPosibleActions(Character& character, wxVector<wxDword>& outPosibleActions)
{
	bool defaultCondActions = true;
	outPosibleActions.clear();
	for(wxDword i = 0; i < m_actions.size(); i++)
	{
		if(m_actions[i]->IsConditioned())
		{
			if((m_actions[i]->GetRequiredSkill() != DISCIPLINE_UNKNOWN)
				&&(m_actions[i]->GetRequiredItem() == ITEM_UNKNOWN)
				&& character.GetDisciplines().Contains(m_actions[i]->GetRequiredSkill()))
			{
				defaultCondActions = false;
				break;
			}
			
			if((m_actions[i]->GetRequiredSkill() == DISCIPLINE_UNKNOWN)
				&&(m_actions[i]->GetRequiredItem() != ITEM_UNKNOWN)
				&& character.ContainsItem(m_actions[i]->GetRequiredItem()))
			{
				defaultCondActions = false;
				break;
			}

			if((m_actions[i]->GetRequiredSkill() != DISCIPLINE_UNKNOWN)
				&&(m_actions[i]->GetRequiredItem() != ITEM_UNKNOWN)
				&& character.GetDisciplines().Contains(m_actions[i]->GetRequiredSkill())
				&& character.ContainsItem(m_actions[i]->GetRequiredItem()))
			{
				defaultCondActions = false;
				break;
			}
		}
	}
	
	for(wxDword i = 0; i < m_actions.size(); i++)
	{
		if(!m_actions[i]->IsConditioned())
		{
			outPosibleActions.push_back(i);
		} else {
			if((m_actions[i]->GetRequiredSkill() != DISCIPLINE_UNKNOWN)
				||(m_actions[i]->GetRequiredItem() != ITEM_UNKNOWN))
			{
				if((m_actions[i]->GetRequiredSkill() == DISCIPLINE_UNKNOWN)
					&& character.ContainsItem(m_actions[i]->GetRequiredItem()))
				{
					outPosibleActions.push_back(i);
				}
				
				if((m_actions[i]->GetRequiredItem() == ITEM_UNKNOWN)
					&& character.GetDisciplines().Contains(m_actions[i]->GetRequiredSkill()))
				{
					outPosibleActions.push_back(i);
				}
				
				if(character.ContainsItem(m_actions[i]->GetRequiredItem())
					&&character.GetDisciplines().Contains(m_actions[i]->GetRequiredSkill()))
				{
					outPosibleActions.push_back(i);
				}
				
			} else {
				// it is default action
				if(defaultCondActions)
				{
					outPosibleActions.push_back(i);
				}
			}
		}
	}
	
}

void Scene::AddItem(EItem item)
{
	m_sceneItems.push_back(item);
}

void Scene::GetItemList(Scene::TItemList& sceneItemList)
{
	Scene::TItemList tempList(m_sceneItems);
	sceneItemList.swap(tempList);
}



void Scene::RemoveItem(EItem item)
{
	for(Scene::TItemList::iterator iter = m_sceneItems.begin(); iter != m_sceneItems.end(); iter++)
	{
		if(*iter == item)
		{
			(*iter) = m_sceneItems.back();
			m_sceneItems.pop_back();
			return;
		}
	}
}

bool Scene::ContainsItem(EItem item)
{
	Scene::TItemList::iterator iter;
	for(iter = m_sceneItems.begin(); iter != m_sceneItems.end(); iter++)
	{
		if(*iter == item) return true;
	}
	
	return false;
}

//////////////////////////////////////////////
/////////////// SceneManager //////////////////
//////////////////////////////////////////////


bool SceneManager::AddScene(Scene* pScene)
{
	if(pScene == NULL) return false;
	std::pair<TSceneMap::iterator,bool> retval;
	retval = m_sceneMap.insert( TSceneMapPair(pScene->GetSceneId(), pScene));
	return retval.second;
}

Scene* SceneManager::GetScene(wxDword sceneId)
{
	TSceneMap::iterator iter;
	iter = m_sceneMap.find(sceneId);
	if(iter == m_sceneMap.end())
	{
		return NULL;
	} 

	return iter->second;
}

void SceneManager::RemoveScene(wxDword sceneId)
{
	TSceneMap::iterator iter;
	m_sceneMap.erase(sceneId);
}

bool SceneManager::SceneMapTest()
{
	std::list<wxInt32> sceneNodeQueue;
	std::list<wxInt32>::reverse_iterator sceneNodeIter;
	wxVector<wxDword> testVector;
	wxVector<wxDword>::iterator testIter;
	TSceneMap::iterator sceneIter;
	ActionVector::iterator actionIter;
	
	testVector.resize(m_sceneMap.size(), 0);
	
	for (sceneIter = m_sceneMap.begin(); sceneIter != m_sceneMap.end(); sceneIter++)
	{
		testVector[sceneIter->first] = 1;
	}
	
	for (testIter = testVector.begin(); testIter != testVector.end(); testIter++)
	{
		if(*testIter != 1)
		{
			return false;
		}
	}
	
	sceneNodeQueue.push_back(0);
	
	while (!sceneNodeQueue.empty())
	{
		sceneIter = m_sceneMap.find(sceneNodeQueue.front());
		sceneNodeQueue.pop_front(); // remove node from queue
		if(sceneIter == m_sceneMap.end())
		{
			// id is missing failed
			return false;
		}
		if(testVector[sceneIter->first] != 2)
		{
			testVector[sceneIter->first] = 2;
			for(actionIter = sceneIter->second->m_actions.begin(); actionIter != sceneIter->second->m_actions.end(); actionIter++)
			{
				switch((*actionIter)->GetType())
				{
				case ACTION_MOVE:
				case ACTION_CREATE_CHAR:
				{
					Action *pAction = static_cast<Action*>(*actionIter);
					sceneNodeQueue.push_back(pAction->GetMoveTarget());
					break;
				}
				case ACTION_LOTERY:
				{
					Action *pAction = static_cast<Action*>(*actionIter);
					for(wxDword i = 0; i < 9; i++)
					{
						wxDword max_count = i;
						bool addNode = true;
						for (sceneNodeIter = sceneNodeQueue.rbegin(); sceneNodeIter != sceneNodeQueue.rend(); sceneNodeIter++)
						{
							if(*sceneNodeIter == pAction->GetLoteryTarget(i))
							{
								addNode = false;
								break;
							}
							if(max_count == 0)
							{
								break;
							} else {
								max_count--;
							}
						}
						if(addNode)
						{
							sceneNodeQueue.push_back(pAction->GetLoteryTarget(i));
						}
					}
					break;
				}
				case ACTION_FIGHT:
				{
					ActionFight *pFightAction = static_cast<ActionFight*>(*actionIter);
					if(pFightAction->GetWinTarget() != TARGET_UNKNOWN)
					{
						sceneNodeQueue.push_back(pFightAction->GetWinTarget());	
					}
					
					if(pFightAction->GetLoseTarget() != TARGET_UNKNOWN)
					{
						sceneNodeQueue.push_back(pFightAction->GetLoseTarget());	
					}
					
					if(pFightAction->GetRetreatTarget() != TARGET_UNKNOWN)
					{
						sceneNodeQueue.push_back(pFightAction->GetRetreatTarget());	
					}
					
					break;
				}
				default:
					return false;
				}
			}
		}
	}
	
	for (testIter = testVector.begin(); testIter != testVector.end(); testIter++)
	{
		if(*testIter != 2)
		{
			return false;
		}
	}
	
	return true;
	
}
