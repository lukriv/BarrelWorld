#ifndef __GAME_XML_LOADER_H__
#define __GAME_XML_LOADER_H__

#include "../GameResHold/gdefloader.h"
#include "../GameResHold/gdefholder.h"
#include "../GameSystem/glog.h"

class GameXmlResourceLoader : public IGameDefLoader
{
private:
	bool m_isInitialized;
	GameLoggerPtr m_spLogger;
	
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
	GameErrorCode LoadEntities(wxXmlNode* pNode, GameDefinitionHolder& defHolder);
	
	GameErrorCode CreateMesh(wxXmlNode* pNode, wxString& name, RefObjSmPtr<NameDef> &spDef);
public:
	GameTestResourceLoader() : m_isInitialized(false),
						m_sceneNode(nullptr),
						m_meshDefNode(nullptr),
						m_materialDefNode(nullptr),
						m_renderEntityDefNode(nullptr),
						m_cameraDefNode(nullptr),
						m_logicDefNode(nullptr),
						m_inputDefNode(nullptr),
						m_animationDefNode(nullptr),
						m_animatorDefNode(nullptr) {}
	~GameTestResourceLoader() {}

	GameErrorCode Initialize(GameLogger *pLogger = NULL);
public:
	virtual GameErrorCode Load(GameDefinitionHolder& defHolder);

};


#endif //__GAME_XML_LOADER_H__