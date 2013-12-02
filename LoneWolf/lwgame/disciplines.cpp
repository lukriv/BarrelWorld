#include "disciplines.h"

const EDisciplines DisciplinesManager::UNKNOWN_DISC = Discipline();

bool DisciplinesManager::AddDisciplineDesc(EDisciplines type, const wxString& title, wxString& desc)
{
	TDisciplinesMap::iterator iter;
	iter = m_disciplineDescMap.find(type);
	if(iter == m_disciplineDescMap.end())
	{
		//add new map item
		std::pair<TDisciplinesMap::iterator, bool> insertPair;
		insertPair = m_disciplineDescMap.insert(type);
		if(insertPair.second)
		{
			insertPair.first->second.m_type = type;
			insertPair.first->second.m_title = title;
			insertPair.first->second.m_desc = desc;
		} else {
			return false;
		}
	} else {
		iter->second.m_type = type;
		iter->second.m_title = title;
		iter->second.m_desc = desc;
	}
	
	return true;
}

Discipline& DisciplinesManager::GetDiscipline(EDisciplines type)
{
	TDisciplinesMap::iterator iter;
	iter = m_disciplineDescMap.find(type);
	if(iter != m_disciplineDescMap.end())
	{
		return iter->second;
	}
	
	return UNKNOWN_DISC;	
}
