#ifndef __LONE_WOLF_GAME_ENGINE_H__
#define __LONE_WOLF_GAME_ENGINE_H__

#include <set>
#include <wx/defs.h>
#include "scenemgr.h"
#include "lwglobres.h"
#include "character.h"

class ActionFight;

class LWGameEngineCallback {
public:
	virtual void SelectDisciplines(wxDword discToSelect, std::set<EDisciplines> &chosenDisc) = 0;
	virtual void FightTurn(ActionFight &fight) = 0;
	virtual void FightFinish(ActionFight &fight) = 0;
		
};


class LWGameEngine {
	Scene* m_pActualScene;
	SceneManager m_sceneMgr;
	GlobalResourceManager m_resMgr;
	Character m_mainCharacter;
	
	LWGameEngineCallback* m_pUserInteractionCallback;
	wxString m_errorStr;
private:
	bool PromoteCharacter();
	bool CreateNewCharacter();
	bool AddNewCharacterDiscipline(EDisciplines disc);
	
	bool RunSceneEvents();
	bool RunEvent(EventBase* event);
public:
	LWGameEngine() : m_pActualScene(NULL), m_pUserInteractionCallback(NULL) {}
	~LWGameEngine() {}
	
	bool Initialize(LWGameEngineCallback *pCallback);
	bool NextChapter(wxInt32 nextChapter);
	bool NewGame();
	
	inline Scene* GetActualScene() { return m_pActualScene;}
	inline GlobalResourceManager& GetResMgr() { return m_resMgr; }
	inline Character& GetMainCharacter() { return m_mainCharacter; }
	
	bool RunAction(wxDword actionIndex);
	
	wxString& GetLastErrorString() { return m_errorStr; }
};



#endif //__LONE_WOLF_GAME_ENGINE_H__