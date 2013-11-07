#ifndef __LONE_WOLF_ACTIONS_H__
#define __LONE_WOLF_ACTIONS_H__

enum ActionType {
	ACTION_UNKNOWN 		= 0,
	ACTION_CREATE_CHAR	= 1,
	ACTION_MOVE			= 2,
	ACTION_LOTERY		= 3		
};


class ActionManager {
public:
	ActionManager() {}
	
	bool SetDefaultActionDesc(ActionType type, const wxString& title );
	bool SetDefaultActionDesc(const wxString& actionName, const wxString& title );
	
	const wxString& GetDefaultActionDesc(ActionType type);
	
	static ActionType ConvertActionNameToType(const wxString &actionName);
};


class Action {
	ActionType m_type;
	wxString m_desc;
	wxVector<wxDword> m_loteryTargets;
public:
	Action() : m_type(ACTION_UNKNOWN) {}
	
	void SetType(ActionType type) { m_type = type; }
	void SetDesc(const wxString& desc) { m_desc = desc; }
	void SetLoteryTarget(wxDword id, wxDword target);
	
	ActionType GetType() { return m_type; }
	const wxString& GetDesc() { return m_desc; }
	wxDword GetLoteryTarget(wxDword id);
	
};



typedef wxVector<Action> ActionVector;



#endif //__LONE_WOLF_ACTIONS_H__