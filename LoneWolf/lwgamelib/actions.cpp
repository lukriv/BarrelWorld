#include "actions.h"


wxInt32 Action::GetLoteryTarget(wxDword id)
{
	if(m_loteryTargets.empty()) {
		return TARGET_UNKNOWN;
	}

	return m_loteryTargets[id];
}

void Action::SetLoteryTarget(wxDword id, wxInt32 target)
{
	if ( m_loteryTargets.empty() ) {
		m_loteryTargets.resize(10, TARGET_UNKNOWN);
	}

	if(id < 10) {
		m_loteryTargets[id] = target;
	}
}

void Action::SetLoteryTarget(wxDword fromId, wxDword toId, wxInt32 target)
{
	if(toId < fromId) return;
	if(toId > 9) return;

	if ( m_loteryTargets.empty() ) {
		m_loteryTargets.resize(10, TARGET_UNKNOWN);
	}

	for (wxDword i = fromId; i <= toId; i++) {
		m_loteryTargets[i] = target;
	}
}

wxInt32 Action::GetMoveTarget()
{
	if(!m_loteryTargets.empty()) {
		return m_loteryTargets[0];
	}

	return 0;
}

void Action::SetMoveTarget(wxInt32 target)
{
	if ( m_loteryTargets.empty() ) {
		m_loteryTargets.resize(1);
	}

	m_loteryTargets[0] = target;
}

bool ActionManager::GetDefaultActionDesc(EActionType type, wxString& desc)
{
	TActionTranslateMap::iterator iter;
	iter = m_translateMap.find(type);
	if(iter != m_translateMap.end()) {
		desc = iter->second;
		return true;
	} else {
		return false;
	}
}

bool ActionManager::SetDefaultActionDesc(EActionType type, const wxString& desc)
{
	TActionTranslateMap::iterator iter;
	iter = m_translateMap.find(type);
	if(iter == m_translateMap.end()) {
		m_translateMap.insert(TActionTranslateMapPair(type, desc));
	} else {
		iter->second = desc;
	}

	return true;
}


std::ostream& operator<< (std::ostream& output, const Action& action)
{
	switch(action.m_type) {
	case ACTION_CREATE_CHAR:
		output << "Type: Create character; Target: " << action.m_loteryTargets[0] << "\n";
		break;
	case ACTION_MOVE:
		output << "Type: Move; Target: " << action.m_loteryTargets[0] << "\n";
		output << "   Desc: " << action.m_desc.c_str() << "\n";
		break;
	case ACTION_LOTERY:
		output << "Type: Lotery; Targets: ";
		for (int i = 0; i < 10; ++i) {
			output << action.m_loteryTargets[i] << ", ";
		}
		output << "\n";
		break;
	default:
		break;
	}

	return output;
}


std::wostream& operator<< (std::wostream& output, const Action& action)
{
	switch(action.m_type) {
	case ACTION_CREATE_CHAR:
		output << L"Type: Create character; Target: " << action.m_loteryTargets[0] << L"\n";
		break;
	case ACTION_MOVE:
		output << L"Type: Move; Target: " << action.m_loteryTargets[0] << L"\n";
		output << L"   Desc: " << action.m_desc.ToStdWstring() << L"\n";
		break;
	case ACTION_LOTERY:
		output << L"Type: Lotery; Targets: ";
		for (int i = 0; i < 10; ++i) {
			output << action.m_loteryTargets[i] << L", ";
		}
		output << L"\n";
		break;
	default:
		break;
	}

	return output;
}
wxInt32 ActionChoose::GetMinimumGold()
{
}

EItem ActionChoose::GetRequiredItem()
{
}

EDisciplines ActionChoose::GetRequiredSkill()
{
}

EActionType ActionChoose::GetType()
{
}

bool ActionChoose::IsConditioned()
{
}
