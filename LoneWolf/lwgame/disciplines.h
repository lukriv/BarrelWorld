#ifndef __LONE_WOLF_DISCIPLINES_H__
#define __LONE_WOLF_DISCIPLINES_H__


struct Discipline {
	EDisciplines m_type;
	wxString m_title;
	wxString m_desc;
};




class DisciplinesManager {
	typedef std::map<EDisciplines, Discipline> TDisciplinesMap;
	typedef std::pair<EDisciplines, Discipline> TDisciplinesMapPair;
private:
	TDisciplinesMap m_disciplineDescMap;
public:
	DisciplinesManager() {}
	~DisciplinesManager() {}
	
	bool AddDisciplineDesc(EDisciplines type, const wxString &title, wxString &desc);
	Discipline& GetDiscipline(EDisciplines type);
};



#endif //__LONE_WOLF_DISCIPLINES_H__