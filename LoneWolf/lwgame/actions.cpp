

wxDword Action::GetLoteryTarget(wxDword id)
{
	if(m_loteryTargets.empty())
	{
		return 0;
	}
	
	return m_loteryTargets[id];
}

void Action::SetLoteryTarget(wxDword id, wxDword target)
{
	if ( m_loteryTargets.empty() )
	{
		m_loteryTargets.resize(10, 0);
	}
	
	if(id < 10)
	{
		m_loteryTargets[id] = target;
	}
}

void Action::SetLoteryTarget(wxDword fromId, wxDword toId, wxDword target)
{
	if(toId < fromId) return;
	if(toId > 9) return;
	
	if ( m_loteryTargets.empty() )
	{
		m_loteryTargets.resize(10, 0);
	}
	
	for (wxDword i = fromId; i <= toId; i++)
	{
		m_loteryTargets[i] = target;
	}
}

bool ActionManager::GetDefaultActionDesc(EActionType type, wxString& desc)
{
	TActionTranslateMap::iterator iter;
	iter = m_translateMap.find(type);
	if(iter != m_translateMap.end())
	{
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
	if(iter == m_translateMap.end())
	{
		m_translateMap.insert(TActionTranslateMapPair(type, desc));
	} else {
		iter->second = desc;
	}
	
	return true;	
}
