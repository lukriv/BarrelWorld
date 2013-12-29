#ifndef __LONE_WOLF_ACTIONS_H__
#define __LONE_WOLF_ACTIONS_H__

#include <iostream>
#include <map>
#include <wx/defs.h>
#include <wx/vector.h>
#include "lwdefs.h"




class ActionManager {
	typedef std::map<EActionType,wxString> TActionTranslateMap;
	typedef std::pair<EActionType,wxString> TActionTranslateMapPair;
private:
	TActionTranslateMap m_translateMap;
public:
	ActionManager() {}
	
	bool SetDefaultActionDesc(EActionType type, const wxString& desc );
	
	bool GetDefaultActionDesc(EActionType type, wxString& desc );
	
};


class Action {
	EActionType m_type;
	wxString m_desc;
	wxVector<wxDword> m_loteryTargets;
	
	friend std::ostream& operator<< (std::ostream& output, const Action& action);
	friend std::wostream& operator<< (std::wostream& output, const Action& action);
public:
	Action() : m_type(ACTION_UNKNOWN) {}
	Action(EActionType type) : m_type(type) {}
	
	void SetType(EActionType type) { m_type = type; }
	void SetDesc(const wxString& desc) { m_desc = desc; }
	void SetMoveTarget(wxDword target);
	void SetLoteryTarget(wxDword id, wxDword target);
	void SetLoteryTarget(wxDword fromId, wxDword toId, wxDword target);
	
	EActionType GetType() { return m_type; }
	const wxString& GetDesc() { return m_desc; }
	wxDword GetMoveTarget();
	wxDword GetLoteryTarget(wxDword id);
	
};

std::ostream& operator<< (std::ostream& output, const Action& action);
std::wostream& operator<< (std::wostream& output, const Action& action);

typedef wxVector<Action> ActionVector;



#endif //__LONE_WOLF_ACTIONS_H__