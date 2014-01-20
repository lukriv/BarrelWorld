#include "disciplines.h"

static Discipline UNKNOWN_DISC = Discipline();

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

bool DisciplinesManager::DisciplineExists(EDisciplines type)
{
	return (m_disciplineDescMap.find(type) != m_disciplineDescMap.end());
}

bool DisciplinesManager::AddDiscipline(const wxString& keyName, const Discipline& disc)
{
	std::pair<TDiscConvertTable::iterator, bool> retvalConvert;
	std::pair<TDisciplinesMap::iterator, bool> retvalDisc;
	
	if((disc.m_id == DISCIPLINE_UNKNOWN)||(keyName.IsEmpty())) return false;
	
	retvalConvert = m_discConvertTable.insert(TDiscConvertTablePair(keyName, disc.m_id));
	if(retvalConvert.second == false)
	{
		return false;
	}
	
	retvalDisc = m_disciplineDescMap.insert(TDisciplinesMapPair(disc.m_id, disc));
	return retvalDisc.second;
}

EDisciplines DisciplinesManager::GetDisciplineType(const wxString& keyName)
{
	TDiscConvertTable::iterator iter = m_discConvertTable.find(keyName);
	if(iter == m_discConvertTable.end())
	{
		return DISCIPLINE_UNKNOWN;
	}
	
	return iter->second;
}
