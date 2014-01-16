#ifndef __LONE_WOLF_ACTIONS_H__
#define __LONE_WOLF_ACTIONS_H__

#include <iostream>
#include <map>
#include <wx/defs.h>
#include <wx/vector.h>
#include "disciplines.h"




class ActionManager
{
	typedef std::map<EActionType,wxString> TActionTranslateMap;
	typedef std::pair<EActionType,wxString> TActionTranslateMapPair;
private:
	TActionTranslateMap m_translateMap;
public:
	ActionManager() {}

	bool SetDefaultActionDesc(EActionType type, const wxString& desc );

	bool GetDefaultActionDesc(EActionType type, wxString& desc );

};


class ActionBase
{
public:
	virtual EActionType GetType() = 0;
	virtual bool IsConditioned() = 0;
	virtual EDisciplines GetRequiredSkill() = 0;
	virtual EItem GetRequiredItem() = 0;
	virtual wxInt32 GetMinimumGold() = 0;

	virtual ~ActionBase() {}
};


class Action : public ActionBase
{
	EActionType m_type;
	wxString m_desc;
	wxVector<wxInt32> m_loteryTargets;

	EDisciplines m_requiredSkill;
	EItem m_requiredItem;
	wxInt32 m_minimumGold;

	bool m_conditioned;

	friend std::ostream& operator<< (std::ostream& output, const Action& action);
	friend std::wostream& operator<< (std::wostream& output, const Action& action);
public:
	Action() : m_type(ACTION_UNKNOWN),
		m_requiredSkill(DISCIPLINE_UNKNOWN),
		m_requiredItem(ITEM_UNKNOWN),
		m_minimumGold(0),
		m_conditioned(false) {}
	Action(EActionType type) : m_type(type),
		m_requiredSkill(DISCIPLINE_UNKNOWN),
		m_requiredItem(ITEM_UNKNOWN),
		m_minimumGold(0),
		m_conditioned(false) {}
	~Action() {}

	void SetType(EActionType type) {
		m_type = type;
	}
	void SetDesc(const wxString& desc) {
		m_desc = desc;
	}
	void SetMoveTarget(wxInt32 target);
	void SetLoteryTarget(wxDword id, wxInt32 target);
	void SetLoteryTarget(wxDword fromId, wxDword toId, wxInt32 target);

	void SetCondition(bool conditioned) {
		m_conditioned = conditioned;
	}
	bool IsConditioned() {
		return m_conditioned;
	}
	void SetMinimumGold(wxInt32 minGold) {
		if(minGold > 0) {
			m_conditioned = true;
			m_minimumGold = minGold;
		} else {
			m_minimumGold = 0;
			m_conditioned = false;
		}
	}
	wxInt32 GetMinimumGold() {
		return m_minimumGold;
	}
	void SetRequiredSkill( EDisciplines skill) {
		m_requiredSkill = skill;
		m_conditioned = true;
	}
	EDisciplines GetRequiredSkill() {
		return m_requiredSkill;
	}

	void SetRequiredItem( EItem item ) {
		m_requiredItem = item;
		m_conditioned = true;
	}
	EItem GetRequiredItem() {
		return m_requiredItem;
	}

	EActionType GetType() {
		return m_type;
	}
	const wxString& GetDesc() {
		return m_desc;
	}
	wxInt32 GetMoveTarget();
	wxInt32 GetLoteryTarget(wxDword id);

};

class ActionChoose : public ActionBase {
private:
	EActionType m_type;
	wxString m_desc;
	wxVector<wxInt32> m_loteryTargets;

	EDisciplines m_requiredSkill;
	EItem m_requiredItem;
	wxInt32 m_minimumGold;

	bool m_conditioned;
public:
	virtual wxInt32 GetMinimumGold();
	virtual EItem GetRequiredItem();
	virtual EDisciplines GetRequiredSkill();
	virtual EActionType GetType();
	virtual bool IsConditioned();
	
	wxInt32 GetMoveTarget();
	
	void SetRequiredItem( EItem item ) {
		m_requiredItem = item;
		m_conditioned = true;
	}
	
	void SetRequiredSkill( EDisciplines skill) {
		m_requiredSkill = skill;
		m_conditioned = true;
	}


};


std::ostream& operator<< (std::ostream& output, const Action& action);
std::wostream& operator<< (std::wostream& output, const Action& action);

typedef wxVector<ActionBase*> ActionVector;



#endif //__LONE_WOLF_ACTIONS_H__
