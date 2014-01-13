#ifndef __LONE_WOLF_XML_READER_H__
#define __LONE_WOLF_XML_READER_H__




#include <wx/xml/xml.h>
#include "scenemgr.h"
#include "lwglobres.h"
#include "fight.h"


static const wxChar* CZECH_LANG __attribute__((unused)) = wxT("cs");

class LoneWolfXmlReader {
private:
	static wxString m_errorString;
	static wxString m_language;
public:
	static bool LoadChapter(const wxChar* xmlfilepath, GlobalResourceManager &resMgr, SceneManager &sceneMgr);
	static bool LoadTextDefinitions(const wxChar* xmlfilepath, GlobalResourceManager &resMgr);
	
	static const wxString& GetLastErrorString() {return m_errorString;}
	
	static bool SetLanguage(const wxString& lang);
	
private:
	static bool ParseChapter(wxXmlNode* chapterNode, GlobalResourceManager &resMgr, SceneManager &sceneMgr);
	static bool ParseScene(wxXmlNode* sceneNode, GlobalResourceManager &resMgr, SceneManager &sceneMgr);
	static bool ParseSceneActions(wxXmlNode* sceneActionNode, GlobalResourceManager &resMgr, Scene &scene);
	static bool ParseSceneItems(wxXmlNode* sceneActionNode, GlobalResourceManager &resMgr, Scene &scene);
	static bool ParseSceneEvents(wxXmlNode* sceneEventsNode, GlobalResourceManager &resMgr, Scene& scene);
	static bool ParseSceneMerchadise(wxXmlNode* sceneMerchandiseNode, GlobalResourceManager &resMgr, Scene& scene);
	
	static bool ParseSceneEvent(wxXmlNode* eventNode, GlobalResourceManager &resMgr, EventBase *&pEvent);
	
	static bool ReadActionAttr(wxXmlNode* actionNode, GlobalResourceManager &resMgr, Action& action);
	static bool ReadActionAttr(wxXmlNode* actionNode, GlobalResourceManager &resMgr, ActionFight& action);
	
	
	static bool ParseDefinitions(wxXmlNode* defNode, GlobalResourceManager& resMgr);
	static bool ParseDefActions(wxXmlNode* defNode, GlobalResourceManager& resMgr);
	static bool ParseDefDisciplines(wxXmlNode* defNode, GlobalResourceManager& resMgr);
	static bool ParseDefItems(wxXmlNode* defNode, GlobalResourceManager& resMgr);
	static bool ParseDefChapters(wxXmlNode* defNode, GlobalResourceManager& resMgr);
	static bool ReadItemAttr(wxXmlNode*, GlobalResourceManager& resMgr);
	
	static bool ReadDisciplineAttr(wxXmlNode*, GlobalResourceManager& resMgr, DisciplineProperties& properties);
	
	/*!
	 * \brief Read attributes cond_act, cond_max, attack_max, duration
	 * \param eventNode Arbirary node with event attributes
	 * \param properties Struct for storing output
	 * \return bool if method was successful
	 */
	static bool ReadEventAttr(wxXmlNode* eventNode, GlobalResourceManager &resMgr, EventProperties& properties);

	/*!
	 * \brief Read text from tags "title" or "desc"
	 * \param textNode Input textNode 'title' or 'desc' element node
	 * \param text Output readed lang text if node has correct language attribute
	 * \param language Output laguage of the text
	 * \return true if reading was successful, false otherwise
	 */
	static bool ReadText(wxXmlNode* textNode, wxString& text, wxString& language);
	
	static void ProcessError(wxXmlNode* errNode);
};




#endif //__LONE_WOLF_XML_READER_H__