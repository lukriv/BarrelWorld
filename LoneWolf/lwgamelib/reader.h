#ifndef __LONE_WOLF_XML_READER_H__
#define __LONE_WOLF_XML_READER_H__




#include <wx/xml/xml.h>
#include "scenemgr.h"
#include "lwglobres.h"

class LoneWolfXmlReader {
private:
	static wxString m_errorString;
public:
	static bool LoadChapter(const wxChar* xmlfilepath, GlobalResourceManager &resMgr, SceneManager &sceneMgr);
	static bool LoadTextDefinitions(const wxChar* xmlfilepath, GlobalResourceManager &resMgr);
	
	static const wxString& GetLastErrorString() {return m_errorString;}
	
private:
	static bool ParseChapter(wxXmlNode* chapterNode, GlobalResourceManager &resMgr, SceneManager &sceneMgr);
	static bool ParseScene(wxXmlNode* sceneNode, GlobalResourceManager &resMgr, SceneManager &sceneMgr);
	static bool ParseSceneActions(wxXmlNode* sceneActionNode, GlobalResourceManager &resMgr, Scene &scene);
	static bool ParseSceneItems(wxXmlNode* sceneActionNode, GlobalResourceManager &resMgr, Scene &scene);
	static bool ParseSceneEvents(wxXmlNode* sceneEventsNode, GlobalResourceManager &resMgr, Scene& scene);
	
	
	
	static bool ParseDefinitions(wxXmlNode* defNode, GlobalResourceManager& resMgr);
	static bool ParseDefActions(wxXmlNode* defNode, GlobalResourceManager& resMgr);
	static bool ParseDefDisciplines(wxXmlNode* defNode, GlobalResourceManager& resMgr);
	static bool ParseDefWeapons(wxXmlNode* defNode, GlobalResourceManager& resMgr);
	static bool ParseDefItems(wxXmlNode* defNode, GlobalResourceManager& resMgr);
	static bool ParseDefSpecialItems(wxXmlNode* defNode, GlobalResourceManager& resMgr);
	static bool ReadItemAttr(wxXmlNode*, GlobalResourceManager& resMgr);
	
	/*!
	 * \brief Read attributes cond_act, cond_max, attack_max, duration
	 * \param eventNode Arbirary node with event attributes
	 * \param properties Struct for storing output
	 * \return bool if method was successful
	 */
	static bool ReadEventAttr(wxXmlNode* eventNode, GlobalResourceManager &resMgr, EventProperties& properties);
	
	static void ProcessError(wxXmlNode* errNode);
};




#endif //__LONE_WOLF_XML_READER_H__