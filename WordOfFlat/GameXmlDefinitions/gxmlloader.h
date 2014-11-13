#ifndef __GAME_XML_LOADER_H__
#define __GAME_XML_LOADER_H__

#include <wx/xml/xml.h>
#include "../GameResHold/gdefloader.h"
#include "../GameResHold/gdefholder.h"
#include "../GameSystem/glog.h"

class GameXmlResourceLoader : public IGameDefLoader
{
private:
	bool m_isInitialized;
	GameLoggerPtr m_spLogger;
	wxString m_fileName;
	
private:
	GameErrorCode ParseDefinitions(wxXmlNode* pNode, GameDefinitionHolder& defHolder);
	GameErrorCode ParseScene(wxXmlNode* pNode, GameDefinitionHolder& defHolder);

	GameErrorCode LoadMeshes(wxXmlNode* pNode, GameDefinitionHolder& defHolder);
	GameErrorCode LoadMaterials(wxXmlNode* pNode, GameDefinitionHolder& defHolder);
	GameErrorCode LoadRenderEntities(wxXmlNode* pNode, GameDefinitionHolder& defHolder);
	GameErrorCode LoadCameras(wxXmlNode* pNode, GameDefinitionHolder& defHolder);
	GameErrorCode LoadRenderDef(wxXmlNode* pNode, GameDefinitionHolder& defHolder);
	GameErrorCode LoadAnimations(wxXmlNode* pNode, GameDefinitionHolder& defHolder);
	GameErrorCode LoadAnimators(wxXmlNode* pNode, GameDefinitionHolder& defHolder);
	GameErrorCode LoadInput(wxXmlNode* pNode, GameDefinitionHolder& defHolder);
	GameErrorCode LoadLogic(wxXmlNode* pNode, GameDefinitionHolder& defHolder);
		
	GameErrorCode CreateMesh(wxXmlNode* pNode, wxString& name, RefObjSmPtr<NameDef> &spDef);
	GameErrorCode CreateMaterial(wxXmlNode* pNode, wxString& name, RefObjSmPtr<NameDef> &spDef);
	GameErrorCode CreateRenderEntity(wxXmlNode* pNode, GameDefinitionHolder& defHolder, wxString& name, RefObjSmPtr<RenderEntityDef> &spDef);
	GameErrorCode CreateRender(wxXmlNode* pNode, GameDefinitionHolder& defHolder, wxString& name, RefObjSmPtr<RenderDef> &spDef);
	GameErrorCode CreateCamera(wxXmlNode* pNode, wxString& name, RefObjSmPtr<CameraDef> &spCameraDef);
	GameErrorCode CreateInput(wxXmlNode* pNode, wxString& name, RefObjSmPtr<InputDef> &spInputDef);
	GameErrorCode CreateLogic(wxXmlNode* pNode, wxString& name, RefObjSmPtr<LogicDef> &spLogicDef);
	GameErrorCode CreateEntity(wxXmlNode* pNode, GameDefinitionHolder& defHolder, wxString& name, RefObjSmPtr<EntityDef> &spEntity);
	GameErrorCode CreateTransform(wxXmlNode* pNode, RefObjSmPtr<TransformDef> &spTransform);
	
	
	GameErrorCode GetAttrXYZ(wxXmlNode* pNode, Ogre::Vector3 &vector);
	GameErrorCode GetAttrQuat(wxXmlNode* pNode, Ogre::Quaternion &quat);
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
	virtual GameErrorCode Load(GameDefinitionHolder& defHolder) override;

};


#endif //__GAME_XML_LOADER_H__