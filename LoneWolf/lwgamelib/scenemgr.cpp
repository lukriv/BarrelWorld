#include "scenemgr.h"
#include <iostream>
#include <list>


std::ostream& operator<< (std::ostream &output, const Scene& scene) 
{
	output << "--------------------------\n";
	output << "Scene ID: " << scene.m_sceneId << "\n";
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
	output << L"Scene ID: " << scene.m_sceneId << "\n";
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


Scene& Scene::operator=(const Scene& scene)
{
	if(&scene == this) return *this;
	
	m_sceneId = scene.m_sceneId;
	m_desc = scene.m_desc;
	m_actions = scene.m_actions;
}

bool SceneManager::AddScene(const Scene& scene)
{
	std::pair<TSceneMap::iterator,bool> retval;
	retval = m_sceneMap.insert( TSceneMapPair(scene.m_sceneId, scene));
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

	return &(iter->second);
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
			for(actionIter = sceneIter->second.m_actions.begin(); actionIter != sceneIter->second.m_actions.end(); actionIter++)
			{
				switch(actionIter->GetType())
				{
				case ACTION_MOVE:
				case ACTION_CREATE_CHAR:
					sceneNodeQueue.push_back(actionIter->GetMoveTarget());
					break;
				case ACTION_LOTERY:
					for(wxDword i = 0; i < 9; i++)
					{
						wxDword max_count = i;
						bool addNode = true;
						for (sceneNodeIter = sceneNodeQueue.rbegin(); sceneNodeIter != sceneNodeQueue.rend(); sceneNodeIter++)
						{
							if(*sceneNodeIter == actionIter->GetLoteryTarget(i))
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
							sceneNodeQueue.push_back(actionIter->GetLoteryTarget(i));
						}
					}
					break;
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
