#ifndef __LONE_WOLF_ACTIONS_H__
#define __LONE_WOLF_ACTIONS_H__




class ActionManager {
public:
	ActionManager() {}
	
	bool SetDefaultActionDesc(EActionType type, const wxString& title );
	bool SetDefaultActionDesc(const wxString& actionName, const wxString& title );
	
	const wxString& GetDefaultActionDesc(EActionType type);
	static EActionType ConvertActionNameToType(const wxString &actionName);
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