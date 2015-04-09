#include "gRenderDotSceneLoader.h"
#include <wx/sstream.h>

RenderDotSceneLoader::RenderDotSceneLoader(GameLogger *pLogger) : m_pLogger(pLogger),
																m_sceneMgr(nullptr),
																m_pAttachNode(nullptr)
{
}

RenderDotSceneLoader::~RenderDotSceneLoader()
{
}

GameErrorCode RenderDotSceneLoader::ParseDotScene(const wxString& SceneString, Ogre::SceneManager* yourSceneMgr, Ogre::SceneNode* pAttachNode, const wxString& sPrependNode)
{
    // set up shared object values
    m_sceneMgr = yourSceneMgr;
    m_sPrependNode = sPrependNode;
    staticObjects.clear();
    dynamicObjects.clear();
 
    wxXmlDocument XMLDoc;
	wxStringInputStream inputStream(SceneString);
    wxXmlNode *XMLRoot = nullptr;
 
	if(!XMLDoc.Load(inputStream)) 
	{
		FWGLOG_ERROR(wxT("Load dot scene failed"), m_pLogger);
		return FWG_E_XML_ROOT_NOT_FOUND_ERROR;
	}
  
    // Validate the File
    XMLRoot = XMLDoc.GetRoot();
    if( XMLRoot->GetName() != wxT("scene") ) 
	{
        FWGLOG_ERROR(wxT("Error: Missing <scene>" ), m_pLogger);
        return FWG_E_XML_ROOT_NOT_FOUND_ERROR;
    }
 
    // figure out where to attach any nodes we create
    m_pAttachNode = pAttachNode;
    if(!m_pAttachNode) 
	{
        m_pAttachNode = m_sceneMgr->getRootSceneNode();
	}
 
    // Process the scene
    FWG_RETURN_FAIL(ProcessScene(XMLRoot));
 
    
	return FWG_NO_ERROR;
}
 
GameErrorCode RenderDotSceneLoader::ProcessScene(wxXmlNode* XMLRoot)
{
    // Process the scene parameters
    wxString version = XMLRoot->GetAttribute( wxString(wxT("formatVersion")), wxT("unknown"));
	wxString attr;
	
    wxString message = "Parsing dotScene file with version " + version;
    if(XMLRoot->GetAttribute(wxT("ID"), &attr))
	{
        message += wxT(", id ") + attr;
	}
	
    if(XMLRoot->GetAttribute(wxT("sceneManager"), &attr))
	{
        message += wxT(", scene manager ") + attr;
	}
	attr.Clear();
    if(XMLRoot->GetAttribute(wxT("minOgreVersion"), &attr))
	{
        message += wxT(", min. Ogre version ") + attr;
	}
	attr.Clear();
    if(XMLRoot->GetAttribute(wxT("author"), &attr))
	{
        message += wxT(", author ") + attr;
	}
 
    FWGLOG_DEBUG_FORMAT(wxT("%s"), m_pLogger, message, FWGLOG_ENDVAL);
 
    wxXmlNode *pElement = XMLRoot->GetChildren();
	
	while(pElement)
	{
 		// Process nodes (?)
		if(pElement->GetName() == wxT("nodes")) 
		{
			FWG_RETURN_FAIL(ProcessNodes(pElement));
		// Process externals (?)
		} else if(pElement->GetName() == wxT("externals")) {
			FWG_RETURN_FAIL(ProcessExternals(pElement));
		// Process environment (?)
		} else if(pElement->GetName() == wxT("environment")) {
			FWG_RETURN_FAIL(processEnvironment(pElement));
		// Process terrain (?)
		} else if(pElement->GetName() == wxT("terrain")) {
			FWG_RETURN_FAIL(processTerrain(pElement));
		// Process userDataReference (?)
		} else if(pElement->GetName() == wxT("userDataReference")) {
			FWG_RETURN_FAIL(processUserDataReference(pElement));
	 	// Process octree (?)
		} else if(pElement->GetName() == wxT("octree")) {
			FWG_RETURN_FAIL(processOctree(pElement));
		// Process light (?)
		} else if(pElement->GetName() == wxT("light")) {
			FWG_RETURN_FAIL(processLight(pElement));
		// Process camera (?)
		} else if(pElement->GetName() == wxT("camera")) {
			FWG_RETURN_FAIL(processCamera(pElement));
		} else {
			ProcessUnknownTag(pElement);
		}
	}
	
	return FWG_NO_ERROR;
}
 
GameErrorCode RenderDotSceneLoader::ProcessNodes(wxXmlNode *XMLNode)
{
	GameErrorCode result = FWG_NO_ERROR;
    wxXmlNode *pElement = XMLNode->GetChildren();
	
 
	while(pElement)
	{
		// Process node (*)
		if (pElement->GetName() == wxT("node"))
		{
			if(FWG_FAILED(result = ProcessNode(pElement)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Process node failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
				return result;
			}
		}
	 
		// Process position (?)
		if(pElement->GetName() == wxT("position"))
		{
			FWGLOG_WARNING_FORMAT(wxT("'%s' for tag '%s'  is not supported"), m_pLogger
					, wxT("position"), XMLNode->GetName().data().AsInternal(), FWGLOG_ENDVAL);
			//m_pAttachNode->setPosition(parseVector3(pElement));
			//m_pAttachNode->setInitialState();
		}
	 
		// Process rotation (?)
		if(pElement->GetName() == wxT("rotation"))
		{
			FWGLOG_WARNING_FORMAT(wxT("'%s' for tag '%s'  is not supported"), m_pLogger
					, wxT("rotation"), XMLNode->GetName().data().AsInternal(), FWGLOG_ENDVAL);

			//m_pAttachNode->setOrientation(parseQuaternion(pElement));
			//m_pAttachNode->setInitialState();
		}
	 
		// Process scale (?)
		if(pElement->GetName() == wxT("scale"))
		{
			FWGLOG_WARNING_FORMAT(wxT("'%s' for tag '%s'  is not supported"), m_pLogger
					, wxT("scale"), XMLNode->GetName().data().AsInternal(), FWGLOG_ENDVAL);
			//m_pAttachNode->setScale(parseVector3(pElement));
			//m_pAttachNode->setInitialState();
		}
		
		pElement = pElement->GetNext();
	
	}
	
	
	return result;
}
 
GameErrorCode RenderDotSceneLoader::ProcessExternals(wxXmlNode *XMLNode)
{
    //! @todo Implement this
	FWGLOG_DEBUG(wxT("Process externals is not supported!"), m_pLogger);
	return FWG_NO_ERROR;
}
 
GameErrorCode RenderDotSceneLoader::ProcessEnvironment(wxXmlNode *XMLNode)
{
	GameErrorCode result = FWG_NO_ERROR;
    wxXmlNode *pElement = XMLNode->GetChildren();
 
	while(pElement)
	{
		// Process fog (?)
		if(pElement->GetName() == wxT("fog"))
		{
			if( FWG_FAILED( result = ProcessFog(pElement)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Process element '%s' failed ( 0x%08x ) on line %d"), m_pLogger,
									pElement->GetName().data().AsInternal(),
									result,
									pElement->GetLineNumber(),
									FWGLOG_ENDVAL);
				return result;
			}
		// Process skyBox (?)
		} else if(pElement->GetName() == wxT("skyBox")) {
			if( FWG_FAILED( result = ProcessSkyBox(pElement)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Process element '%s' failed ( 0x%08x ) on line %d"), m_pLogger,
									pElement->GetName().data().AsInternal(),
									result,
									pElement->GetLineNumber(),
									FWGLOG_ENDVAL);
				return result;
			}
	 
		// Process skyDome (?)
		} else if(pElement->GetName() == wxT("skyDome")) {
			if( FWG_FAILED( result = ProcessSkyDome(pElement)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Process element '%s' failed ( 0x%08x ) on line %d"), m_pLogger,
									pElement->GetName().data().AsInternal(),
									result,
									pElement->GetLineNumber(),
									FWGLOG_ENDVAL);
				return result;
			}
	 
		// Process skyPlane (?)
		} else if(pElement->GetName() == wxT("skyPlane")) {
			if( FWG_FAILED( result = ProcessSkyPlane(pElement)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Process element '%s' failed ( 0x%08x ) on line %d"), m_pLogger,
									pElement->GetName().data().AsInternal(),
									result,
									pElement->GetLineNumber(),
									FWGLOG_ENDVAL);
				return result;
			}
	 
		// Process clipping (?)
		} else if(pElement->GetName() == wxT("clipping")) {
			if( FWG_FAILED( result = ProcessClipping(pElement)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Process element '%s' failed ( 0x%08x ) on line %d"), m_pLogger,
									pElement->GetName().data().AsInternal(),
									result,
									pElement->GetLineNumber(),
									FWGLOG_ENDVAL);
				return result;
			}
	 
		// Process colourAmbient (?)
		} else if(pElement->GetName() == wxT("colourAmbient")) {
			m_sceneMgr->setAmbientLight(parseColour(pElement))))
			{
				FWGLOG_ERROR_FORMAT(wxT("Process element '%s' failed ( 0x%08x ) on line %d"), m_pLogger,
									pElement->GetName().data().AsInternal(),
									result,
									pElement->GetLineNumber(),
									FWGLOG_ENDVAL);
				return result;
			}
	 
		// Process colourBackground (?)
		//! @todo Set the background colour of all viewports (RenderWindow has to be provided then)
		} else if(pElement->GetName() == wxT("colourBackground")) {
			;//mSceneMgr->set(parseColour(pElement));
	 
		// Process userDataReference (?)
		} else if(pElement->GetName() == wxT("userDataReference")) {
			processUserDataReference(pElement);
			
		} else {
			ProcessUnknownTag(pElement);
		}
			
		pElement = pElement->GetNext();
		
	}
	
	return result;
}
 
void RenderDotSceneLoader::processTerrain(TiXmlElement *XMLNode)
{
    //! @todo Implement this
}
 
void RenderDotSceneLoader::processUserDataReference(TiXmlElement *XMLNode, SceneNode *pParent)
{
    //! @todo Implement this
}
 
void RenderDotSceneLoader::processOctree(TiXmlElement *XMLNode)
{
    //! @todo Implement this
}
 
GameErrorCode RenderDotSceneLoader::ProcessLight(wxXmlNode *XMLNode, Ogre::SceneNode *pParent)
{
    // Process attributes
    wxString name = XMLNode->GetAttribute(wxT("name"));
    wxString id = XMLNode->GetAttribute(wxT("id"));
	
	Light *pLight = nullptr;
	
    // Create the light
	if(name.IsEmpty())
	{
		pLight = m_sceneMgr->createLight();
	} else {
		pLight = m_sceneMgr->createLight(name);
	}
    
    if(pParent)
	{
        pParent->attachObject(pLight);
	}
 
    wxString sValue = XMLNode->GetAttribute(wxT("type"));
    if(sValue == wxT("point"))
	{
        pLight->setType(Light::LT_POINT);
    } else if(sValue == wxT("directional")) {
        pLight->setType(Light::LT_DIRECTIONAL);
    } else if(sValue == wxT("spot")) {
        pLight->setType(Light::LT_SPOTLIGHT);
	} else if(sValue == wxT("radPoint")) {
        pLight->setType(Light::LT_POINT);
	}
 
    pLight->setVisible(GetAttribBool(XMLNode, wxT("visible"), true));
    pLight->setCastShadows(GetAttribBool(XMLNode, wxT("castShadows"), true));
 
    wxXmlNode *pElement = XMLNode->GetChildren();
 
	while(pElement)
	{
		// Process position (?)
		if(pElement->GetName() == wxT("position"))
		{
			pLight->setPosition(ParseVector3(pElement));
		// Process normal (?)
		} else if(pElement->GetName() == wxT("normal")) {
			pLight->setDirection(ParseVector3(pElement));
		// Process colourDiffuse (?)
		} else if(pElement->GetName() == wxT("colourDiffuse")) {
			pLight->setDiffuseColour(ParseColour(pElement));
		// Process colourSpecular (?)
		} else if(pElement->GetName() == wxT("colourSpecular")) {
			pLight->setSpecularColour(ParseColour(pElement));
		// Process lightRange (?)
		} else if(pElement->GetName() == wxT("lightRange")) {
			ProcessLightRange(pElement, pLight);
		// Process lightAttenuation (?)
		} else if(pElement->GetName() == wxT("lightAttenuation")) {
			ProcessLightAttenuation(pElement, pLight);
		// Process userDataReference (?)
		} else if(pElement->GetName() == wxT("userDataReference")) {
			FWGLOG_WARNING_FORMAT(wxT("Tag '%s' is not supported - line %d"), m_pLogger
					, XMLNode->GetName().data().AsInternal()
					, XMLNode->GetLineNumber()
					, FWGLOG_ENDVAL);
			//processUserDataReference(pElement, pLight);
		} else {
			ProcessUnknownTag(pElement);
		}
		pElement = pElement->GetNext();		
	}
	
	return FWG_NO_ERROR;
}
 
GameErrorCode RenderDotSceneLoader::ProcessCamera(wxXmlNode *XMLNode, Ogre::SceneNode *pParent)
{
    // Process attributes
    wxString name = XMLNode->GetAttribute(wxT("name"));
    wxString id = XMLNode->GetAttribute(wxT("id"));
    Ogre::Real fov = GetAttribReal(XMLNode, wxT("fov"), 45);
    Ogre::Real aspectRatio = GetAttribReal(XMLNode, wxT("aspectRatio"), 1.3333);
    wxString projectionType = XMLNode->GetAttribute(wxT("projectionType"), wxT("perspective"));
 
    // Create the camera
    Ogre::Camera *pCamera = m_sceneMgr->createCamera(name.ToStdString());
    if(pParent)
	{
        pParent->attachObject(pCamera);
	}
 
    // Set the field-of-view
    //! @todo Is this always in degrees?
    pCamera->setFOVy(Ogre::Degree(fov));
 
    // Set the aspect ratio
    pCamera->setAspectRatio(aspectRatio);
 
    // Set the projection type
    if(projectionType == wxT("perspective"))
	{
        pCamera->setProjectionType(Ogre::PT_PERSPECTIVE);
    } else if(projectionType == "orthographic") {
        pCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
	}
 
    wxXmlNode *pElement = XMLNode->GetChildren();
 
	while(pElement)
	{
 
		// Process clipping (?)
		if(pElement->GetName() == wxT("clipping"))
		{
			Ogre::Real nearDist = GetAttribReal(pElement, "near");
			pCamera->setNearClipDistance(nearDist);
	 
			Ogre::Real farDist =  getAttribReal(pElement, "far", 50000.0);
			pCamera->setFarClipDistance(farDist);
		
		// Process position (?)
		} else if(pElement->GetName() == wxT("position")) {
			pCamera->setPosition(ParseVector3(pElement));
	 
		// Process rotation (?)
		} else if(pElement->GetName() == wxT("rotation")) {
			pCamera->setOrientation(ParseQuaternion(pElement));
	 
		// Process normal (?)
		} else if(pElement->GetName() == wxT("normal")) {
			FWGLOG_WARNING_FORMAT(wxT("Tag '%s' is not supported - line %d"), m_pLogger
					, XMLNode->GetName().data().AsInternal()
					, XMLNode->GetLineNumber()
					, FWGLOG_ENDVAL);
	 
		// Process lookTarget (?)
		} else if(pElement->GetName() == wxT("lookTarget")) {
			FWGLOG_WARNING_FORMAT(wxT("Tag '%s' is not supported - line %d"), m_pLogger
					, XMLNode->GetName().data().AsInternal()
					, XMLNode->GetLineNumber()
					, FWGLOG_ENDVAL);
	 
		// Process trackTarget (?)
		} else if(pElement->GetName() == wxT("trackTarget")) {
			FWGLOG_WARNING_FORMAT(wxT("Tag '%s' is not supported - line %d"), m_pLogger
					, XMLNode->GetName().data().AsInternal()
					, XMLNode->GetLineNumber()
					, FWGLOG_ENDVAL);
	 
		// Process userDataReference (?)
		} else if(pElement->GetName() == wxT("userDataReference")) {
			FWGLOG_WARNING_FORMAT(wxT("Tag '%s' is not supported - line %d"), m_pLogger
					, XMLNode->GetName().data().AsInternal()
					, XMLNode->GetLineNumber()
					, FWGLOG_ENDVAL);
		} else {
			ProcessUnknownTag(pElement);
		}
		
		pElement = pElement->GetNext();	
	}
	
	return FWG_NO_ERROR;
}
 
GameErrorCode RenderDotSceneLoader::ProcessNode(wxXmlNode *XMLNode, SceneNode *pParent)
{
	GameErrorCode result = FWG_NO_ERROR;
    // Construct the node's name
	wxString name = m_sPrependNode + XMLNode->GetAttribute(wxT("name"));
 
    // Create the scene node
    Ogre::SceneNode *pNode = nullptr;
    if(name.empty())
    {
        // Let Ogre choose the name
        if(pParent)
		{
            pNode = pParent->createChildSceneNode();
		} else {
            pNode = m_pAttachNode->createChildSceneNode();
		}
    } else {
        // Provide the name
        if(pParent)
		{
            pNode = pParent->createChildSceneNode(name);
        } else {
            pNode = m_pAttachNode->createChildSceneNode(name);
		}
    }
 
    // Process other attributes
    wxString id = XMLNode->GetAttribute(wxT("id"));
    bool isTarget = GetAttribBool(XMLNode, wxT("isTarget"));
 
    wxXmlNode *pElement = nullptr;
 
	while(pElement)
	{
		// Process position (?)
		if(pElement->GetName() == wxT("position"))
		{
			pNode->setPosition(ParseVector3(pElement));
			pNode->setInitialState();
		 	 
		// Process rotation (?)
		} else if(pElement->GetName() == wxT("rotation")) {
			pNode->setOrientation(ParseQuaternion(pElement));
			pNode->setInitialState();

		// Process scale (?)
		} else if(pElement->GetName() == wxT("scale")) {
			pNode->setScale(ParseVector3(pElement));
			pNode->setInitialState();
		
		// Process lookTarget (?)
		} else if(pElement->GetName() == wxT("lookTarget")) {
			if(FWG_FAILED(result = ProcessLookTarget(pElement, pNode)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Process element '%s' failed ( 0x%08x ) on line %d"), m_pLogger,
									pElement->GetName().data().AsInternal(),
									result,
									pElement->GetLineNumber(),
									FWGLOG_ENDVAL);
				return result;
			}
		
		// Process trackTarget (?)
		} else if(pElement->GetName() == wxT("trackTarget")) {
			if(FWG_FAILED(result = ProcessTrackTarget(pElement, pNode)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Process element '%s' failed ( 0x%08x ) on line %d"), m_pLogger,
									pElement->GetName().data().AsInternal(),
									result,
									pElement->GetLineNumber(),
									FWGLOG_ENDVAL);
				return result;
			}
			
		
		// Process node (*)
		} else if(pElement->GetName() == wxT("node")) {
			if(FWG_FAILED(result = ProcessNode(pElement, pNode)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Process element '%s' failed ( 0x%08x ) on line %d"), m_pLogger,
									pElement->GetName().data().AsInternal(),
									result,
									pElement->GetLineNumber(),
									FWGLOG_ENDVAL);
				return result;
			}
	 
		// Process entity (*)
		} else if(pElement->GetName() == wxT("entity")) {
			if(FWG_FAILED(result = ProcessEntity(pElement, pNode)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Process element '%s' failed ( 0x%08x ) on line %d"), m_pLogger,
									pElement->GetName().data().AsInternal(),
									result,
									pElement->GetLineNumber(),
									FWGLOG_ENDVAL);
				return result;
			}
			
		// Process light (*)
		} else if(pElement->GetName() == wxT("light")) {
			if(FWG_FAILED(result = ProcessLight(pElement, pNode)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Process element '%s' failed ( 0x%08x ) on line %d"), m_pLogger,
									pElement->GetName().data().AsInternal(),
									result,
									pElement->GetLineNumber(),
									FWGLOG_ENDVAL);
				return result;
			}
			 
		// Process camera (*)
		} else if(pElement->GetName() == wxT("camera")) {
			if(FWG_FAILED(result = ProcessCamera(pElement, pNode)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Process element '%s' failed ( 0x%08x ) on line %d"), m_pLogger,
									pElement->GetName().data().AsInternal(),
									result,
									pElement->GetLineNumber(),
									FWGLOG_ENDVAL);
				return result;
			}
		
	 
		// Process particleSystem (*)
		} else if(pElement->GetName() == wxT("particleSystem")) {
			if(FWG_FAILED(result = ProcessParticleSystem(pElement, pNode)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Process element '%s' failed ( 0x%08x ) on line %d"), m_pLogger,
									pElement->GetName().data().AsInternal(),
									result,
									pElement->GetLineNumber(),
									FWGLOG_ENDVAL);
				return result;
			}
		
	 
		// Process billboardSet (*)
		} else if(pElement->GetName() == wxT("billboardSet")) {
			if(FWG_FAILED(result = ProcessBillboardSet(pElement, pNode)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Process element '%s' failed ( 0x%08x ) on line %d"), m_pLogger,
									pElement->GetName().data().AsInternal(),
									result,
									pElement->GetLineNumber(),
									FWGLOG_ENDVAL);
				return result;
			}
	 
		// Process plane (*)
		} else if(pElement->GetName() == wxT("plane")) {
			if(FWG_FAILED(result = ProcessPlane(pElement, pNode)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Process element '%s' failed ( 0x%08x ) on line %d"), m_pLogger,
									pElement->GetName().data().AsInternal(),
									result,
									pElement->GetLineNumber(),
									FWGLOG_ENDVAL);
				return result;
			}
	 
		// Process userDataReference (?)
		} else if(pElement->GetName() == wxT("userDataReference")) {
			FWGLOG_WARNING_FORMAT(wxT("Tag '%s' on line %d is not supported"),m_pLogger
								, pElement->GetName().data().AsInternal()
								, pElement->GetLineNumber()
								, FWGLOG_ENDVAL);
		} else {
			ProcessUnknownTag(pElement);
		}
		
		pElement = pElement->GetNext();
	}
	
	return result;
}
 
GameErrorCode RenderDotSceneLoader::ProcessLookTarget(wxXmlNode */*XMLNode*/, SceneNode */*pParent*/)
{
	FWGLOG_WARNING(wxT("ProcessLookTarget is not implemented yet"), m_pLogger);
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}
 
GameErrorCode RenderDotSceneLoader::ProcessTrackTarget(wxXmlNode */*XMLNode*/, SceneNode */*pParent*/)
{
	FWGLOG_WARNING(wxT("ProcessTrackTarget is not implemented yet"), m_pLogger);
	return FWG_E_NOT_IMPLEMENTED_ERROR;
    // Process attributes
    //String nodeName = getAttrib(XMLNode, "nodeName");
    //
    //TiXmlElement *pElement;
    //
    //// Process localDirection (?)
    //Vector3 localDirection = Vector3::NEGATIVE_UNIT_Z;
    //pElement = XMLNode->FirstChildElement("localDirection");
    //if(pElement)
    //    localDirection = parseVector3(pElement);
    //
    //// Process offset (?)
    //Vector3 offset = Vector3::ZERO;
    //pElement = XMLNode->FirstChildElement("offset");
    //if(pElement)
    //    offset = parseVector3(pElement);
    //
    //// Setup the track target
    //try
    //{
    //    SceneNode *pTrackNode = m_sceneMgr->getSceneNode(nodeName);
    //    pParent->setAutoTracking(true, pTrackNode, localDirection, offset);
    //}
    //catch(Ogre::Exception &/*e*/)
    //{
    //    LogManager::getSingleton().logMessage("[RenderDotSceneLoader] Error processing a track target!");
    //}
}
 
GameErrorCode RenderDotSceneLoader::ProcessEntity(wxXmlNode *XMLNode, Ogre::SceneNode *pParent)
{
    // Process attributes
    wxString name = XMLNode->GetAttribute(wxT("name"));
    wxString id = XMLNode->GetAttribute(wxT("id"));
    wxString meshFile = XMLNode->GetAttribute(wxT("meshFile"));
    wxString materialFile = XMLNode->GetAttribute(wxT("materialFile"));
    bool isStatic = GetAttribBool(XMLNode, wxT("static"), false);
    bool castShadows = GetAttribBool(XMLNode, wxT("castShadows"), true);
 
    // TEMP: Maintain a list of static and dynamic objects
    if(isStatic)
	{
        staticObjects.push_back(name);
	} else {
        dynamicObjects.push_back(name);
	}
	
	// Create the entity
	Ogre::Entity *pEntity = nullptr;
	try
	{
		Ogre::MeshManager::getSingleton().load(meshFile, m_sGroupName.ToStdString());
		if(name.IsEmpty())
		{
			pEntity = m_sceneMgr->createEntity(meshFile);
		} else {
			pEntity = m_sceneMgr->createEntity(name, meshFile);
		}
		
		pEntity->setCastShadows(castShadows);
		pParent->attachObject(pEntity);
	
		if(!materialFile.IsEmpty())
		{
			pEntity->setMaterialName(materialFile);
		}
	} catch(Ogre::Exception &e)	{
		FWGLOG_ERROR_FORMAT(wxT("Error loading an entity - ogre exception: '%s'"), m_pLogger
								, wxString::FromUTF8(e.getFullDescription().c_str()).GetData().AsInternal()
								, FWGLOG_ENDVAL);
		return FWG_E_RENDER_SYSTEM_ERROR;
	}
 
    wxXmlNode *pElement = XMLNode->GetChildren();
 
	while(pElement)
	{
		// Process vertexBuffer (?)
		if(pElement->GetName() == wxT("vertexBuffer"))
		{
			FWGLOG_WARNING_FORMAT(wxT("Tag '%s' on line %d is not supported"),m_pLogger
								, pElement->GetName().data().AsInternal()
								, pElement->GetLineNumber()
								, FWGLOG_ENDVAL);
		} else if(pElement->GetName() == wxT("indexBuffer")) {
			FWGLOG_WARNING_FORMAT(wxT("Tag '%s' on line %d is not supported"),m_pLogger
								, pElement->GetName().data().AsInternal()
								, pElement->GetLineNumber()
								, FWGLOG_ENDVAL);			
		}		// Process userDataReference (?)
		else if(pElement->GetName() == wxT("userDataReference")) {
			FWGLOG_WARNING_FORMAT(wxT("Tag '%s' on line %d is not supported"),m_pLogger
								, pElement->GetName().data().AsInternal()
								, pElement->GetLineNumber()
								, FWGLOG_ENDVAL);
		}
	}
 
	return FWG_NO_ERROR;
}
 
GameErrorCode RenderDotSceneLoader::ProcessParticleSystem(wxXmlNode *XMLNode, Ogre::SceneNode *pParent)
{
    // Process attributes
    wxString name = XMLNode->GetAttribute(wxT("name"));
    wxString id = XMLNode->GetAttribute(wxT("id"));
    wxString file = XMLNode->GetAttribute(wxT("file"));
 
    // Create the particle system
    try
    {
        Ogre::ParticleSystem *pParticles = m_sceneMgr->createParticleSystem(name, file);
        pParent->attachObject(pParticles);
    } catch(Ogre::Exception &e) {
        FWGLOG_ERROR_FORMAT(wxT("Error creating a particle system: %s"), m_pLogger
								, wxString::FromUTF8(e.getFullDescription().c_str()).GetData().AsInternal()
								, FWGLOG_ENDVAL);
		return FWG_E_RENDER_SYSTEM_ERROR;
    }
	
	return FWG_NO_ERROR;
}
 
GameErrorCode RenderDotSceneLoader::ProcessBillboardSet(wxXmlNode *, Ogre::SceneNode *)
{
    //! @todo Implement this
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}
 
GameErrorCode RenderDotSceneLoader::ProcessPlane(wxXmlNode *, Ogre::SceneNode *)
{
    //! @todo Implement this
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}
 
GameErrorCode RenderDotSceneLoader::ProcessFog(wxXmlNode *XMLNode)
{
    // Process attributes
    Ogre::Real expDensity = GetAttribReal(XMLNode, wxT("expDensity"), 0.001);
    Ogre::Real linearStart = GetAttribReal(XMLNode, wxT("linearStart"), 0.0);
    Ogre::Real linearEnd = GetAttribReal(XMLNode, wxT("linearEnd"), 1.0);
 
    Ogre::FogMode mode = Ogre::FOG_NONE;
    wxString sMode = XMLNode->GetAttribute(wxT("mode"));
    if(sMode == wxT("none"))
	{
        mode = FOG_NONE;
    } else if(sMode == wxT("exp")) {
        mode = FOG_EXP;
    } else if(sMode == wxT("exp2")) {
        mode = FOG_EXP2;
    } else if(sMode == wxT("linear")) {
        mode = FOG_LINEAR;
	}
 
    wxXmlNode *pElement = XMLNode->GetChildren();
	Ogre::ColourValue colourDiffuse = ColourValue::White;
	while(pElement)
	{
		// Process colourDiffuse (?)
		
		if(pElement->GetName() == wxT("colourDiffuse"))
		{
			colourDiffuse = ParseColour(pElement);
		} else {
			ProcessUnknownTag(pElement);
		}
		
		pElement = pElement->GetNext();
	}
 
    // Setup the fog
    m_sceneMgr->setFog(mode, colourDiffuse, expDensity, linearStart, linearEnd);
	
	return FWG_NO_ERROR;
}
 
GameErrorCode RenderDotSceneLoader::ProcessSkyBox(wxXmlNode *XMLNode)
{
    // Process attributes
    wxString material = XMLNode->GetAttribute(wxT("material"));
    Ogre::Real distance = GetAttribReal(XMLNode, wxT("distance"), 5000);
    bool drawFirst = GetAttribBool(XMLNode, wxT("drawFirst"), true);
 
    wxXmlNode *pElement = XMLNode->GetChildren();
 
	Ogre::Quaternion rotation = Quaternion::IDENTITY;
	while(pElement)
	{
		// Process rotation (?)
		if(pElement->GetName() == wxT("rotation"))
		{
			rotation = ParseQuaternion(pElement);
		} else {
			ProcessUnknownTag(pElement);
		}
		
		pElement = pElement->GetNext();
			
	}
 
    // Setup the sky box
    m_sceneMgr->setSkyBox(true, material, distance, drawFirst, rotation, m_sGroupName);
	
	return FWG_NO_ERROR;
}
 
void RenderDotSceneLoader::processSkyDome(TiXmlElement *XMLNode)
{
    // Process attributes
    String material = XMLNode->Attribute("material");
    Real curvature = getAttribReal(XMLNode, "curvature", 10);
    Real tiling = getAttribReal(XMLNode, "tiling", 8);
    Real distance = getAttribReal(XMLNode, "distance", 4000);
    bool drawFirst = getAttribBool(XMLNode, "drawFirst", true);
 
    TiXmlElement *pElement;
 
    // Process rotation (?)
    Quaternion rotation = Quaternion::IDENTITY;
    pElement = XMLNode->FirstChildElement("rotation");
    if(pElement)
        rotation = parseQuaternion(pElement);
 
    // Setup the sky dome
    m_sceneMgr->setSkyDome(true, material, curvature, tiling, distance, drawFirst, rotation, 16, 16, -1, m_sGroupName);
}
 
void RenderDotSceneLoader::processSkyPlane(TiXmlElement *XMLNode)
{
    // Process attributes
    String material = getAttrib(XMLNode, "material");
    Real planeX = getAttribReal(XMLNode, "planeX", 0);
    Real planeY = getAttribReal(XMLNode, "planeY", -1);
    Real planeZ = getAttribReal(XMLNode, "planeX", 0);
    Real planeD = getAttribReal(XMLNode, "planeD", 5000);
    Real scale = getAttribReal(XMLNode, "scale", 1000);
    Real bow = getAttribReal(XMLNode, "bow", 0);
    Real tiling = getAttribReal(XMLNode, "tiling", 10);
    bool drawFirst = getAttribBool(XMLNode, "drawFirst", true);
 
    // Setup the sky plane
    Plane plane;
    plane.normal = Vector3(planeX, planeY, planeZ);
    plane.d = planeD;
    m_sceneMgr->setSkyPlane(true, plane, material, scale, tiling, drawFirst, bow, 1, 1, m_sGroupName);
}
 
void RenderDotSceneLoader::processClipping(TiXmlElement *XMLNode)
{
    //! @todo Implement this
 
    // Process attributes
    Real fNear = getAttribReal(XMLNode, "near", 0);
    Real fFar = getAttribReal(XMLNode, "far", 1);
}
 
void RenderDotSceneLoader::ProcessLightRange(wxXmlNode *XMLNode, Ogre::Light *pLight)
{
    // Process attributes
    Ogre::Real inner = GetAttribReal(XMLNode, "inner");
    Ogre::Real outer = GetAttribReal(XMLNode, "outer");
    Ogre::Real falloff = GetAttribReal(XMLNode, "falloff", 1.0);
 
    // Setup the light range
    pLight->setSpotlightRange(Ogre::Angle(inner), Ogre::Angle(outer), falloff);
	
	return FWG_NO_ERROR;
}
 
void RenderDotSceneLoader::ProcessLightAttenuation(wxXmlNode *XMLNode, Ogre::Light *pLight)
{
    // Process attributes
    Ogre::Real range = GetAttribReal(XMLNode, "range");
    Ogre::Real constant = GetAttribReal(XMLNode, "constant");
    Ogre::Real linear = GetAttribReal(XMLNode, "linear", 1.0);
    Ogre::Real quadratic = GetAttribReal(XMLNode, "quadratic");
 
    // Setup the light attenuation
    pLight->setAttenuation(range, constant, linear, quadratic);
}
 
String RenderDotSceneLoader::getAttrib(TiXmlElement *XMLNode, const String &attrib, const String &defaultValue)
{
    if(XMLNode->Attribute(attrib))
        return XMLNode->Attribute(attrib);
    else
        return defaultValue;
}
 
Ogre::Real RenderDotSceneLoader::GetAttribReal(wxXmlNode *XMLNode, const wxString &attrib, Ogre::Real defaultValue)
{
	Ogre::Real retVal = defaultValue;
	wxString realStr;
    if(XMLNode->GetAttribute(attrib, &realStr))
	{
		double temp = 0.0;
		if(realStr.ToDouble(temp))
		{
			retVal = static_cast<Ogre::Real>(temp);
		} else {
			FWGLOG_WARNING_FORMAT(wxT("Attribute '%s' from tag '%s' on line %d not contains real value ('%s')"), m_pLogger
									, attrib.data().AsInternal()
									, XMLNode->GetName().data().AsInternal()
									, XMLNode->GetLineNumber()
									, realStr.data().AsInternal()
									, FWGLOG_ENDVAL);
		}
    }
	
	return retVal;
}
 
bool RenderDotSceneLoader::GetAttribBool(wxXmlNode *XMLNode, const wxString &attrib, bool defaultValue)
{
    wxString attrValue;
	if(!XMLNode->GetAttribute(attrib, &attrValue))
	{
        return defaultValue;
	}
	
	if(attrValue == wxT("true"))
	{
        return true;
	}
 
    return false;
}
 
Ogre::Vector3 RenderDotSceneLoader::ParseVector3(wxXmlNode *XMLNode)
{
	double temp = 0.0;
	
	Ogre::Real vec[3] = {0.0, 0.0, 0.0};
	
	if(XMLNode->GetAttribute(wxT("x"), wxT("0.0")).ToDouble(&temp))
	{
		vec[0] = static_cast<Ogre::Real>(temp);
	}
	
	if(XMLNode->GetAttribute(wxT("y"), wxT("0.0")).ToDouble(&temp))
	{
		vec[1] = static_cast<Ogre::Real>(temp);
	}
	
	if(XMLNode->GetAttribute(wxT("z"), wxT("0.0")).ToDouble(&temp))
	{
		vec[2] = static_cast<Ogre::Real>(temp);
	}
	
    return Ogre::Vector3(vec);
}
 
Ogre::Quaternion RenderDotSceneLoader::ParseQuaternion(wxXmlNode *XMLNode)
{
	double temp = 0.0;
	if(XMLNode->HasAttribute(wxT("qx")))
    {
		Ogre::Real qx = 0.0; 
		Ogre::Real qy = 0.0;
		Ogre::Real qz = 0.0;
		Ogre::Real qw = 0.0;
		
		if(XMLNode->GetAttribute(wxT("qx"), wxT("0.0")).ToDouble(&temp))
		{
			qx = static_cast<Ogre::Real>(temp);
		}
		
		if(XMLNode->GetAttribute(wxT("qy"), wxT("0.0")).ToDouble(&temp))
		{
			qy = static_cast<Ogre::Real>(temp);
		}
		
		if(XMLNode->GetAttribute(wxT("qz"), wxT("0.0")).ToDouble(&temp))
		{
			qz = static_cast<Ogre::Real>(temp);
		}
		
		if(XMLNode->GetAttribute(wxT("qw"), wxT("0.0")).ToDouble(&temp))
		{
			qw = static_cast<Ogre::Real>(temp);
		}
		
		return Ogre::Quaternion(qw,qx,qy,qz);
    }
    else if(XMLNode->HasAttribute(wxT("axisX")))
    {
		Ogre::Vector3 axis(Ogre::Vector3::ZERO);
		Ogre::Real angle = 0.0;
		
		if(XMLNode->GetAttribute(wxT("axisX"), wxT("0.0")).ToDouble(&temp))
		{
			axis.x = static_cast<Ogre::Real>(temp);
		}
		
		if(XMLNode->GetAttribute(wxT("axisY"), wxT("0.0")).ToDouble(&temp))
		{
			axis.y = static_cast<Ogre::Real>(temp);
		}
		
		if(XMLNode->GetAttribute(wxT("axisZ"), wxT("0.0")).ToDouble(&temp))
		{
			axis.z = static_cast<Ogre::Real>(temp);
		}
		
		if(XMLNode->GetAttribute(wxT("angle"), wxT("0.0")).ToDouble(&temp))
		{
			angle = static_cast<Ogre::Real>(temp);
		}
		
        return Ogre::Quaternion(Ogre::Angle(angle), axis);
    } 
	else if(XMLNode->HasAttribute(wxT("angleX")))
    {
		FWGLOG_WARNING(wxT("Reading from angles is not supported"), m_pLogger);
    }
	
	return Ogre::Quaternion;
}
 
Ogre::ColourValue RenderDotSceneLoader::ParseColour(wxXmlNode *XMLNode)
{
    return Ogre::ColourValue(
        GetAttribReal(XMLNode, wxT("r")),
        GetAttribReal(XMLNode, wxT("g")),
        GetAttribReal(XMLNode, wxT("b")),
        XMLNode->HasAttribute(wxT("a"))? GetAttribReal(XMLNode, wxT("a")) : 1.0
    );
}
 
String RenderDotSceneLoader::getProperty(const String &ndNm, const String &prop)
{
    for ( unsigned int i = 0 ; i < nodeProperties.size(); i++ )
    {
        if ( nodeProperties[i].nodeName == ndNm && nodeProperties[i].propertyNm == prop )
        {
            return nodeProperties[i].valueName;
        }
    }
 
    return "";
}
 
void RenderDotSceneLoader::processUserDataReference(TiXmlElement *XMLNode, Entity *pEntity)
{
    String str = XMLNode->Attribute("id");
    pEntity->setUserAny(Any(str));
}

void RenderDotSceneLoader::ProcessUnknownTag(wxXmlNode* xmlNode)
{
	if(xmlNode->GetType() != wxXML_COMMENT_NODE)
	{
		FWGLOG_INFO_FORMAT(wxT("Tag '%s' with type '%d' on line '%d' is not known"), m_pLogger
																, xmlNode->GetName()
																, (int)xmlNode->GetType()
																, xmlNode->GetLineNumber()
																, FWGLOG_ENDVAL);
	}
}
