#ifndef __GAME_SCENE_MAP_H__
#define __GAME_SCENE_MAP_H__


class Scene {
public:
	wxDword m_sceneId;
	wxString m_desc;
	ActionVector m_actions;

public:
	Scene() : m_sceneId(0) {}
	~Scene() {}
	
	Scene& operator=(const Scene& scene);
};


class SceneManager {
private:
	typedef std::map<wxDword, Scene> TSceneMap;
	typedef std::pair<wxDword, Scene> TSceneMapPair;
private:
	TSceneMap m_sceneMap;
	
	// managers
	ActionManager m_actionMgr;
	
public:
	SceneManager() {}
	~SceneManager() {}
	
	bool AddScene(const Scene& pScene);
	void RemoveScene(wxDword sceneId);
	Scene* GetScene(wxDword sceneId);
	
	inline ActionManager& GetActionMgr() {return m_actionMgr;}
	
};


#endif //__GAME_SCENE_MAP_H__