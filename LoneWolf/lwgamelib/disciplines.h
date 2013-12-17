#ifndef __LONE_WOLF_DISCIPLINES_H__
#define __LONE_WOLF_DISCIPLINES_H__


struct Discipline {
	EDisciplines m_type;
	wxString m_title;
	wxString m_desc;
public:
	Discipline() : m_type(DISCIPLINE_UNKNOWN) {}
	Discipline(EDisciplines discType) : m_type(discType) {}
	Discipline(EDisciplines discType, const wxString &title, const wxString &desc);
	~Discipline() {}
};




class DisciplinesManager {
	typedef std::map<EDisciplines, Discipline> TDisciplinesMap;
	typedef std::pair<EDisciplines, Discipline> TDisciplinesMapPair;
	
	const EDisciplines UNKNOWN_DISC;
private:
	TDisciplinesMap m_disciplineDescMap;
	
public:
	DisciplinesManager() {}
	~DisciplinesManager() {}
	
	bool AddDisciplineDesc(EDisciplines type, const wxString &title, wxString &desc);
	Discipline& GetDiscipline(EDisciplines type);
};



#endif //__LONE_WOLF_DISCIPLINES_H__