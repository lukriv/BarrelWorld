#ifndef __GAME_XML_LOADER_H__
#define __GAME_XML_LOADER_H__

#include <wx/xml/xml.h>
#include <GameResHold/gdefloader.h>
#include <GameResHold/gdefholder.h>
#include <GameSystem/glog.h>
#include <GameSystem/gset.h>

struct ParamDefinition;

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
	GameErrorCode CreatePhysicsShape(wxXmlNode* pNode, RefObjSmPtr<PhysShapeDef> &spPhysicsShape);
	GameErrorCode CreatePhysics(wxXmlNode* pNode, RefObjSmPtr<PhysCompDef> &spPhysics);
	
	/**
	 * \brief Get and parse param tag and its attributes
	 * @param pNode Param tag node
	 * @param pDefTable Definition table for pObject
	 * @param tableSize Size of definition table
	 * @param paramName returned parameter name
	 * @param pObject Address of returned object
	 */
	GameErrorCode GetParameter(wxXmlNode* pNode, const ParamDefinition* pDefTable, wxInt32 tableSize, wxString &paramName, void* pObject);
	
	GameErrorCode ParametersValidate(GameBasSet<wxString> &foundedParams, const ParamDefinition* pDefTable, wxInt32 tableSize );
	
	
	GameErrorCode ConvertToVec3(const wxString& input, Ogre::Vector3 *resultVec);
	GameErrorCode ConvertToQuat(const wxString& input, Ogre::Quaternion *resultQuat);
	GameErrorCode ConvertToShapeType(const wxString& input, wxInt32 &retType);
		
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