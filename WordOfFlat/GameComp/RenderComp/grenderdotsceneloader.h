#ifndef __GAME_RENDER_DOT_SCENE_LOADER_H__
#define __GAME_RENDER_DOT_SCENE_LOADER_H__

#include <wx/xml/xml.h>
#include <OGRE/OgreSceneManager.h>
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
	//wxString getProperty(const wxString &ndNm, const wxString &prop);
 
    wxVector<nodeProperty> nodeProperties;
    wxVector<wxString> staticObjects;
    wxVector<wxString> dynamicObjects;
	
protected:
    GameErrorCode ProcessScene(wxXmlNode *XMLRoot);

    GameErrorCode ProcessNodes(wxXmlNode *XMLNode);
    GameErrorCode ProcessExternals(wxXmlNode *XMLNode);
    GameErrorCode ProcessEnvironment(wxXmlNode *XMLNode);
    GameErrorCode ProcessTerrain(wxXmlNode *XMLNode);
    GameErrorCode ProcessLight(wxXmlNode *XMLNode, Ogre::SceneNode *pParent = 0);
    GameErrorCode ProcessCamera(wxXmlNode *XMLNode, Ogre::SceneNode *pParent = 0);

    GameErrorCode ProcessNode(wxXmlNode *XMLNode, Ogre::SceneNode *pParent = 0);
    GameErrorCode ProcessLookTarget(wxXmlNode *XMLNode, Ogre::SceneNode *pParent);
    GameErrorCode ProcessTrackTarget(wxXmlNode *XMLNode, Ogre::SceneNode *pParent);
    GameErrorCode ProcessEntity(wxXmlNode *XMLNode, Ogre::SceneNode *pParent);
    GameErrorCode ProcessParticleSystem(wxXmlNode *XMLNode, Ogre::SceneNode *pParent);
    GameErrorCode ProcessBillboardSet(wxXmlNode *XMLNode, Ogre::SceneNode *pParent);
	GameErrorCode ProcessPlane(wxXmlNode *XMLNode, Ogre::SceneNode *pParent);
 
    GameErrorCode ProcessFog(wxXmlNode *XMLNode);
    GameErrorCode ProcessSkyBox(wxXmlNode *XMLNode);
    GameErrorCode ProcessSkyDome(wxXmlNode *XMLNode);
    GameErrorCode ProcessSkyPlane(wxXmlNode *XMLNode);
    GameErrorCode ProcessClipping(wxXmlNode *XMLNode);

    void ProcessLightRange(wxXmlNode *XMLNode, Ogre::Light *pLight);
    void ProcessLightAttenuation(wxXmlNode *XMLNode, Ogre::Light *pLight);

    Ogre::Real GetAttribReal(wxXmlNode *XMLNode, const wxString &parameter, Ogre::Real defaultValue = 0.0);
    bool GetAttribBool(wxXmlNode *XMLNode, const wxString &parameter, bool defaultValue = false);

    Ogre::Vector3 ParseVector3(wxXmlNode *XMLNode);
    Ogre::Quaternion ParseQuaternion(wxXmlNode *XMLNode);
    Ogre::ColourValue ParseColour(wxXmlNode *XMLNode);

	void ProcessUnknownTag(wxXmlNode *xmlNode);
	
protected:

    Ogre::SceneManager *m_sceneMgr;
    Ogre::SceneNode *m_pAttachNode;
    wxString m_sGroupName;
    wxString m_sPrependNode;

};

#endif // GRENDERDOTSCENELOADER_H
