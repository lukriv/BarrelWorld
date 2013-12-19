#ifndef __GAME_SCENE_MAP_H__
#define __GAME_SCENE_MAP_H__


#include "actions.h"
#include "disciplines.h"


class Scene {
public:
	wxInt32 m_sceneId;
	wxString m_desc;
	ActionVector m_actions;

public:
	Scene() : m_sceneId(-1) {}
	~Scene() {}
	
	Scene& operator=(const Scene& scene);
};


class SceneManager {
private:
	typedef std::map<wxDword, Scene> TSceneMap;
	typedef std::pair<wxDword, Scene> TSceneMapPair;
private:
	wxInt32 m_id;
	wxString m_title;
	TSceneMap m_sceneMap;
	
	// managers
	ActionManager m_actionMgr;
	DisciplinesManager m_disciplineMgr;
	ItemAndWeaponManager m_itemAndWeaponMgr;
	
public:
	SceneManager() : m_id(0) {}
	~SceneManager() {}
	
	void SetTitle(const wxString& title) { m_title = title;}
	inline wxString& GetTitle() { return m_title;}
	
	void SetId(wxInt32 id) { m_id = id;}
	inline wxInt32 GetId() { return m_id;}
	
	bool AddScene(const Scene& pScene);
	void RemoveScene(wxDword sceneId);
	Scene* GetScene(wxDword sceneId);
	
	inline ActionManager& GetActionMgr() {return m_actionMgr;}
	inline DisciplinesManager& GetDisciplineMgr() {return m_disciplineMgr;}
	inline ItemAndWeaponManager& GetItemAndDiscMgr() {return m_itemAndWeaponMgr;}
	
};


#endif //__GAME_SCENE_MAP_H__