#ifndef __GAME_RENDER_DOT_SCENE_LOADER_H__
#define __GAME_RENDER_DOT_SCENE_LOADER_H__

#include <wx/xml/xml.h>
#include <GameSystem/glog.h>
#include <GameSystem/gset.h>

class nodeProperty
{
public:
    wxString nodeName;
    wxString propertyNm;
    wxString valueName;
    wxString typeName;

    nodeProperty(const wxString &node, const wxString &propertyName, const wxString &value, const wxString &type)
        : nodeName(node), propertyNm(propertyName), valueName(value), typeName(type) {}
};

class RenderDotSceneLoader
{
private:
	GameLoggerPtr m_pLogger;
public:
	RenderDotSceneLoader(GameLogger *pLogger);
	~RenderDotSceneLoader();
	
	GameErrorCode ParseDotScene(const wxString &SceneString, Ogre::SceneManager *yourSceneMgr, Ogre::SceneNode *pAttachNode = NULL, const wxString &sPrependNode = wxT(""));    
	wxString getProperty(const wxString &ndNm, const wxString &prop);
 
    wxVector<nodeProperty> nodeProperties;
    wxVector<wxString> staticObjects;
    wxVector<wxString> dynamicObjects;
	
protected:
    GameErrorCode ProcessScene(wxXmlNode *XMLRoot);

    GameErrorCode ProcessNodes(wxXmlNode *XMLNode);
    void processExternals(TiXmlElement *XMLNode);
    void processEnvironment(TiXmlElement *XMLNode);
    void processTerrain(TiXmlElement *XMLNode);
    void processUserDataReference(TiXmlElement *XMLNode, SceneNode *pParent = 0);
    void processUserDataReference(TiXmlElement *XMLNode, Entity *pEntity);
    void processOctree(TiXmlElement *XMLNode);
    void processLight(TiXmlElement *XMLNode, SceneNode *pParent = 0);
    void processCamera(TiXmlElement *XMLNode, SceneNode *pParent = 0);

    GameErrorCode ProcessNode(wxXmlNode *XMLNode, SceneNode *pParent = 0);
    void processLookTarget(TiXmlElement *XMLNode, SceneNode *pParent);
    void processTrackTarget(TiXmlElement *XMLNode, SceneNode *pParent);
    void processEntity(TiXmlElement *XMLNode, SceneNode *pParent);
    void processParticleSystem(TiXmlElement *XMLNode, SceneNode *pParent);
    void processBillboardSet(TiXmlElement *XMLNode, SceneNode *pParent);
    void processPlane(TiXmlElement *XMLNode, SceneNode *pParent);

    void processFog(TiXmlElement *XMLNode);
    void processSkyBox(TiXmlElement *XMLNode);
    void processSkyDome(TiXmlElement *XMLNode);
    void processSkyPlane(TiXmlElement *XMLNode);
    void processClipping(TiXmlElement *XMLNode);

    void processLightRange(TiXmlElement *XMLNode, Light *pLight);
    void processLightAttenuation(TiXmlElement *XMLNode, Light *pLight);

    String getAttrib(TiXmlElement *XMLNode, const String &parameter, const String &defaultValue = "");
    Real getAttribReal(TiXmlElement *XMLNode, const String &parameter, Real defaultValue = 0);
    bool GetAttribBool(wxXmlNode *XMLNode, const wxString &parameter, bool defaultValue = false);

    Vector3 parseVector3(TiXmlElement *XMLNode);
    Quaternion parseQuaternion(TiXmlElement *XMLNode);
    ColourValue parseColour(TiXmlElement *XMLNode);

	void ProcessUnknownTag(wxXmlNode *xmlNode);
	
protected:

    SceneManager *m_sceneMgr;
    SceneNode *m_pAttachNode;
    String m_sGroupName;
    String m_sPrependNode;

};

#endif // GRENDERDOTSCENELOADER_H
