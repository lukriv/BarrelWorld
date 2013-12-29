#ifndef __LONE_WOLF_XML_READER_H__
#define __LONE_WOLF_XML_READER_H__




#include <wx/xml/xml.h>
#include "scenemgr.h"
#include "lwglobres.h"

class LoneWolfXmlReader {
public:
	static bool LoadChapter(const wxChar* xmlfilepath, SceneManager &sceneMgr);
	static bool LoadTextDefinitions(const wxChar* xmlfilepath, GlobalResourceManager &resMgr);
	
private:
	static bool ParseChapter(wxXmlNode* titleNode, SceneManager &sceneMgr);
	static bool ParseScene(wxXmlNode* sceneNode, SceneManager &sceneMgr);
	static bool ParseSceneActions(wxXmlNode* sceneActionNode, Scene &scene);
	
	
	static bool ParseDefinitions(wxXmlNode* defNode, GlobalResourceManager& resMgr);
	static bool ParseDefActions(wxXmlNode* defNode, GlobalResourceManager& resMgr);
	static bool ParseDefDisciplines(wxXmlNode* defNode, GlobalResourceManager& resMgr);
	static bool ParseDefWeapons(wxXmlNode* defNode, GlobalResourceManager& resMgr);
	static bool ParseDefBagItems(wxXmlNode* defNode, GlobalResourceManager& resMgr);
	static bool ParseDefSpecialItems(wxXmlNode* defNode, GlobalResourceManager& resMgr);
	
	static void ProcessError(wxXmlNode* errNode);
};




#endif //__LONE_WOLF_XML_READER_H__