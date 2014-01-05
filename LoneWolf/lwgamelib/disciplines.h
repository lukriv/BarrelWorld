#ifndef __LONE_WOLF_DISCIPLINES_H__
#define __LONE_WOLF_DISCIPLINES_H__

#include <map>
#include <wx/string.h>
#include "lwdefs.h"
#include "eventprop.h"



struct Discipline {
	EDisciplines m_id;
	wxString m_title;
	wxString m_desc;
	EventProperties m_property;
public:
	Discipline() : m_id(DISCIPLINE_UNKNOWN) {}
	Discipline(EDisciplines discType) : m_id(discType) {}
	Discipline(EDisciplines discType, const wxString &title, const wxString &desc);
	~Discipline() {}
};




class DisciplinesManager {
public:
	class Iterator {
		std::map<EDisciplines, Discipline>::const_iterator m_iter;
	public:
		Iterator() {}
		Iterator(const std::map<EDisciplines, Discipline>::const_iterator& iter) : m_iter(iter) {}
		
		const Discipline& operator*() const 
		{
			return m_iter->second;
		}
		
		Iterator& operator++()
		{
			m_iter++;
			return *this;
		}
		
		Iterator operator++(int)
		{
			Iterator iter = *this;
			operator++();
			return iter;
		}
		
		const Discipline* operator->() const
		{
			return &(m_iter->second);
		}
		
		bool operator==(const Iterator& iter)
		{
			return (m_iter == iter.m_iter);
		}
		
		bool operator!=(const Iterator& iter)
		{
			return (m_iter != iter.m_iter);
		}
	};
	
private:
	typedef std::map<EDisciplines, Discipline> TDisciplinesMap;
	typedef std::pair<EDisciplines, Discipline> TDisciplinesMapPair;
	
	typedef std::map<wxString, EDisciplines> TDiscConvertTable;
	typedef std::pair<wxString, EDisciplines> TDiscConvertTablePair;
	
private:
	TDisciplinesMap m_disciplineDescMap;
	TDiscConvertTable m_discConvertTable;
public:
	DisciplinesManager() {}
	~DisciplinesManager() {}
	
	bool AddDiscipline(const wxString& keyName, const Discipline& disc);
	bool DisciplineExists(EDisciplines type);
	Discipline& GetDiscipline(EDisciplines type);
	
	EDisciplines GetDisciplineType(const wxString& keyName);
	
	
	Iterator begin() const
	{
		return Iterator( m_disciplineDescMap.begin());
	}
	
	Iterator end() const
	{
		return Iterator(m_disciplineDescMap.end());
	}
};



#endif //__LONE_WOLF_DISCIPLINES_H__