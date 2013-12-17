#include "scenemgr.h"

Scene& Scene::operator=(const Scene& scene)
{
	if(&scene == this) return this;
	
	m_sceneId = scene.m_sceneId;
	m_desc = scene.m_desc;
	m_actions = scene.m_actions;
}

bool SceneManager::AddScene(const Scene& scene)
{
	pair<TSceneMap::iterator,bool> retval;
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
