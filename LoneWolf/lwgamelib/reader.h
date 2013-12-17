#ifndef __LONE_WOLF_XML_READER_H__
#define __LONE_WOLF_XML_READER_H__




#include <wx/xml.h>

class LoneWolfXmlReader {
public:
	static bool CreateGameFromXmlFile(const wxChar* xmlfilepath, SceneManager &sceneMgr);
	
private:
	static bool ParseDefinitions(wxXmlNode* defNode, SceneManager &sceneMgr);
	static bool ParseChapter(wxXmlNode* titleNode, SceneManager &sceneMgr);
	static bool ParseScene(wxXmlNode* sceneNode, SceneManager &sceneMgr);
	
	
	static bool ParseDefActions(wxXmlNode* defNode, SceneManager &sceneMgr);
	static bool ParseDefDisciplines(wxXmlNode* defNode, SceneManager &sceneMgr);
	static bool ParseDefWeapons(wxXmlNode* defNode, SceneManager &sceneMgr);
	static bool ParseDefBagItems(wxXmlNode* defNode, SceneManager &sceneMgr);
	static bool ParseDefSpecialItems(wxXmlNode* defNode, SceneManager &sceneMgr);
};




#endif //__LONE_WOLF_XML_READER_H__