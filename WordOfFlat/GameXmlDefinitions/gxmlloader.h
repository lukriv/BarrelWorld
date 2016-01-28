#ifndef __GAME_XML_LOADER_H__
#define __GAME_XML_LOADER_H__

#include <wx/xml/xml.h>
#include <GameResHold/gdefloader.h>
#include <GameSystem/glog.h>
#include <GameSystem/gset.h>

//forward declaration
class GameEntity;
struct ParamDefinition;
class InputDef;

class GameXmlResourceLoader : public IGameDefLoader
{
private:
	bool m_isInitialized;
	GameLoggerPtr m_spLogger;
	wxString m_fileName;
	
private:
	GameErrorCode ParseDefinitions(wxXmlNode* pNode, GameCompManager& compMgr);
	GameErrorCode ParseScene(wxXmlNode* pNode, GameCompManager& compMgr);

	GameErrorCode LoadEntity(wxXmlNode* pNode, GameCompManager& compMgr);
	GameErrorCode LoadTransform(wxXmlNode* pNode, GameCompManager& compMgr, wxDword entID);
	GameErrorCode LoadInput(wxXmlNode* pNode, GameCompManager& compMgr, wxDword entID);
	GameErrorCode LoadRender(wxXmlNode* pNode, GameCompManager& compMgr, wxDword entID);
	GameErrorCode LoadMoveable(wxXmlNode* pNode, GameCompManager& compMgr, wxDword entID);
	GameErrorCode LoadPhysics(wxXmlNode* pNode, GameCompManager& compMgr, wxDword entID);
	
	GameErrorCode CreateInputDef(wxXmlNode* pNode, RefObjSmPtr<InputDef> &spInputDef);


	/**
	 * \brief Get and parse param tag and its attributes
	 * @param pNode Param tag node
	 * @param pDefTable Definition table for pObject
	 * @param tableSize Size of definition table
	 * @param paramName returned parameter name
	 * @param pObject Address of returned object
	 */
	
	GameErrorCode ParametersValidate(GameBasSet<wxString> &foundedParams, const ParamDefinition* pDefTable, wxInt32 tableSize );
	
	
	GameErrorCode GetAttrValue(wxXmlNode *pNode, wxString &value);
	GameErrorCode GetKeyValue(wxXmlNode *pNode, wxString &action, wxInt32 &keyCode);


public:
	GameXmlResourceLoader() : m_isInitialized(false) {}
	~GameXmlResourceLoader() {}


	/*!
	 * \brief Initialize GameXmlResourceLoader
	 * 
	 * Initialize XML loader to reading or storing scene from/to given xml scene file. 
	 * 
	 * \param xmlFileName File name without path
	 * \param xmlFilePath Path to file (without file name), nullptr means actual directory
	 * \param pLogger Logger
	 * 
	 * \retval FWG_NO_ERROR On success
	 * \retval Other error on fail.
	 */
	GameErrorCode Initialize(const wxChar *xmlFileName, const wxChar *xmlFilePath = nullptr, GameLogger *pLogger = nullptr);
	
public:
	virtual GameErrorCode Load(GameCompManager& compMgr) override;

};


#endif //__GAME_XML_LOADER_H__
