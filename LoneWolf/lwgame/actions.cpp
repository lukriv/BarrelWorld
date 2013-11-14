

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
