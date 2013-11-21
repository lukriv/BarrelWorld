#ifndef __LONE_WOLF_ACTIONS_H__
#define __LONE_WOLF_ACTIONS_H__


#include "lwdefs.h"


class ActionManager {
public:
	ActionManager() {}
	
	bool SetDefaultActionDesc(EActionType type, const wxString& desc );
	
	bool GetDefaultActionDesc(EActionType type, wxString& desc );
	
};


class Action {
	EActionType m_type;
	wxString m_desc;
	wxVector<wxDword> m_loteryTargets;
public:
	Action() : m_type(ACTION_UNKNOWN) {}
	
	void SetType(EActionType type) { m_type = type; }
	void SetDesc(const wxString& desc) { m_desc = desc; }
	void SetLoteryTarget(wxDword id, wxDword target);
	void SetLoteryTarget(wxDword fromId, wxDword toId, wxDword target);
	
	EActionType GetType() { return m_type; }
	const wxString& GetDesc() { return m_desc; }
	wxDword GetLoteryTarget(wxDword id);
	
};



typedef wxVector<Action> ActionVector;



#endif //__LONE_WOLF_ACTIONS_H__