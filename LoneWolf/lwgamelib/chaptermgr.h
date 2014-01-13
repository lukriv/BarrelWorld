#ifndef __LONE_WOLF_CHAPTER_MANAGER_H__
#define __LONE_WOLF_CHAPTER_MANAGER_H__

#include <map>
#include <wx/defs.h>
#include <wx/string.h>


struct Chapter {
	wxInt32 m_id;
	wxString m_title;
	wxString m_file;
public:
	Chapter() : m_id(0) {}
};


static const wxInt32 INITIAL_CHAPTER = 1;

class ChapterMgr {
private:
	typedef std::map<wxInt32,Chapter> TChapterMap;
	typedef std::pair<wxInt32,Chapter> TChapterMapPair;
private:
	TChapterMap m_chapterMap;
public:
	ChapterMgr() {}
	
	bool AddChapter(wxInt32 id, const wxString& title, const wxString& fileName)
	{
		Chapter chapter;
		std::pair<TChapterMap::iterator, bool> retval;
			
		if(id < 1) return false;
		chapter.m_id = id;
		chapter.m_title = title;
		chapter.m_file = fileName;
		
		retval = m_chapterMap.insert(TChapterMapPair(id, chapter));
		return retval.second;
	}
	
	Chapter* GetChapter(wxInt32 id)
	{
		TChapterMap::iterator iter = m_chapterMap.find(id);
		if(iter == m_chapterMap.end())
		{
			return NULL;
		}
		
		return &(iter->second);
	}
};


#endif //__LONE_WOLF_CHAPTER_MANAGER_H__