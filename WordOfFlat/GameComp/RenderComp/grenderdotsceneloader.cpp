#include "grenderRenderDotSceneLoader.h"

RenderDotSceneLoader::RenderDotSceneLoader(GameLogger *pLogger) : m_pLogger(pLogger)
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
    if(XMLRoot->GetAttribute(wxT("ID"), &attr)
	{
        message += ", id " + attr;
	}
	
    if(XMLRoot->GetAttribute(wxT("sceneManager"), &attr)
	{
        message += ", scene manager " + attr);
	}
	attr.Clear();
    if(XMLRoot->GetAttribute(wxT("minOgreVersion"), &attr)
	{
        message += ", min. Ogre version " + attr);
	}
	attr.Clear();
    if(XMLRoot->GetAttribute(wxT("author"), &attr)
	{
        message += ", author " + attr);
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
			FWG_RETURN_FAIL(processExternals(pElement));
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
 
void RenderDotSceneLoader::processExternals(TiXmlElement *XMLNode)
{
    //! @todo Implement this
}
 
void RenderDotSceneLoader::processEnvironment(TiXmlElement *XMLNode)
{
    TiXmlElement *pElement;
 
    // Process fog (?)
    pElement = XMLNode->FirstChildElement("fog");
    if(pElement)
        processFog(pElement);
 
    // Process skyBox (?)
    pElement = XMLNode->FirstChildElement("skyBox");
    if(pElement)
        processSkyBox(pElement);
 
    // Process skyDome (?)
    pElement = XMLNode->FirstChildElement("skyDome");
    if(pElement)
        processSkyDome(pElement);
 
    // Process skyPlane (?)
    pElement = XMLNode->FirstChildElement("skyPlane");
    if(pElement)
        processSkyPlane(pElement);
 
    // Process clipping (?)
    pElement = XMLNode->FirstChildElement("clipping");
    if(pElement)
        processClipping(pElement);
 
    // Process colourAmbient (?)
    pElement = XMLNode->FirstChildElement("colourAmbient");
    if(pElement)
        m_sceneMgr->setAmbientLight(parseColour(pElement));
 
    // Process colourBackground (?)
    //! @todo Set the background colour of all viewports (RenderWindow has to be provided then)
    pElement = XMLNode->FirstChildElement("colourBackground");
    if(pElement)
        ;//mSceneMgr->set(parseColour(pElement));
 
    // Process userDataReference (?)
    pElement = XMLNode->FirstChildElement("userDataReference");
    if(pElement)
        processUserDataReference(pElement);
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
 
void RenderDotSceneLoader::processLight(TiXmlElement *XMLNode, SceneNode *pParent)
{
    // Process attributes
    String name = getAttrib(XMLNode, "name");
    String id = getAttrib(XMLNode, "id");
 
    // Create the light
    Light *pLight = m_sceneMgr->createLight(name);
    if(pParent)
        pParent->attachObject(pLight);
 
    String sValue = getAttrib(XMLNode, "type");
    if(sValue == "point")
        pLight->setType(Light::LT_POINT);
    else if(sValue == "directional")
        pLight->setType(Light::LT_DIRECTIONAL);
    else if(sValue == "spot")
        pLight->setType(Light::LT_SPOTLIGHT);
    else if(sValue == "radPoint")
        pLight->setType(Light::LT_POINT);
 
    pLight->setVisible(getAttribBool(XMLNode, "visible", true));
    pLight->setCastShadows(getAttribBool(XMLNode, "castShadows", true));
 
    TiXmlElement *pElement;
 
    // Process position (?)
    pElement = XMLNode->FirstChildElement("position");
    if(pElement)
        pLight->setPosition(parseVector3(pElement));
 
    // Process normal (?)
    pElement = XMLNode->FirstChildElement("normal");
    if(pElement)
        pLight->setDirection(parseVector3(pElement));
 
    // Process colourDiffuse (?)
    pElement = XMLNode->FirstChildElement("colourDiffuse");
    if(pElement)
        pLight->setDiffuseColour(parseColour(pElement));
 
    // Process colourSpecular (?)
    pElement = XMLNode->FirstChildElement("colourSpecular");
    if(pElement)
        pLight->setSpecularColour(parseColour(pElement));
 
    // Process lightRange (?)
    pElement = XMLNode->FirstChildElement("lightRange");
    if(pElement)
        processLightRange(pElement, pLight);
 
    // Process lightAttenuation (?)
    pElement = XMLNode->FirstChildElement("lightAttenuation");
    if(pElement)
        processLightAttenuation(pElement, pLight);
 
    // Process userDataReference (?)
    pElement = XMLNode->FirstChildElement("userDataReference");
    if(pElement)
        ;//processUserDataReference(pElement, pLight);
}
 
void RenderDotSceneLoader::processCamera(TiXmlElement *XMLNode, SceneNode *pParent)
{
    // Process attributes
    String name = getAttrib(XMLNode, "name");
    String id = getAttrib(XMLNode, "id");
    Real fov = getAttribReal(XMLNode, "fov", 45);
    Real aspectRatio = getAttribReal(XMLNode, "aspectRatio", 1.3333);
    String projectionType = getAttrib(XMLNode, "projectionType", "perspective");
 
    // Create the camera
    Camera *pCamera = m_sceneMgr->createCamera(name);
    if(pParent)
        pParent->attachObject(pCamera);
 
    // Set the field-of-view
    //! @todo Is this always in degrees?
    pCamera->setFOVy(Ogre::Degree(fov));
 
    // Set the aspect ratio
    pCamera->setAspectRatio(aspectRatio);
 
    // Set the projection type
    if(projectionType == "perspective")
        pCamera->setProjectionType(PT_PERSPECTIVE);
    else if(projectionType == "orthographic")
        pCamera->setProjectionType(PT_ORTHOGRAPHIC);
 
    TiXmlElement *pElement;
 
    // Process clipping (?)
    pElement = XMLNode->FirstChildElement("clipping");
    if(pElement)
    {
        Real nearDist = getAttribReal(pElement, "near");
        pCamera->setNearClipDistance(nearDist);
 
        Real farDist =  getAttribReal(pElement, "far");
        pCamera->setFarClipDistance(farDist);
    }
 
    // Process position (?)
    pElement = XMLNode->FirstChildElement("position");
    if(pElement)
        pCamera->setPosition(parseVector3(pElement));
 
    // Process rotation (?)
    pElement = XMLNode->FirstChildElement("rotation");
    if(pElement)
        pCamera->setOrientation(parseQuaternion(pElement));
 
    // Process normal (?)
    pElement = XMLNode->FirstChildElement("normal");
    if(pElement)
        ;//!< @todo What to do with this element?
 
    // Process lookTarget (?)
    pElement = XMLNode->FirstChildElement("lookTarget");
    if(pElement)
        ;//!< @todo Implement the camera look target
 
    // Process trackTarget (?)
    pElement = XMLNode->FirstChildElement("trackTarget");
    if(pElement)
        ;//!< @todo Implement the camera track target
 
    // Process userDataReference (?)
    pElement = XMLNode->FirstChildElement("userDataReference");
    if(pElement)
        ;//!< @todo Implement the camera user data reference
}
 
GameErrorCode RenderDotSceneLoader::ProcessNode(wxXmlNode *XMLNode, SceneNode *pParent)
{
	GameErrorCode result = FWG_NO_ERROR;
    // Construct the node's name
	wxString name = m_sPrependNode + XMLNode->GetAttribute(wxT("name"));
 
    // Create the scene node
    SceneNode *pNode = nullptr;
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
			pNode->setPosition(parseVector3(pElement));
			pNode->setInitialState();
		}
	 
		// Process rotation (?)
		pElement = XMLNode->FirstChildElement("rotation");
		if(pElement)
		{
			pNode->setOrientation(parseQuaternion(pElement));
			pNode->setInitialState();
		}
	 
		// Process scale (?)
		pElement = XMLNode->FirstChildElement("scale");
		if(pElement)
		{
			pNode->setScale(parseVector3(pElement));
			pNode->setInitialState();
		}
	 
		// Process lookTarget (?)
		pElement = XMLNode->FirstChildElement("lookTarget");
		if(pElement)
			processLookTarget(pElement, pNode);
	 
		// Process trackTarget (?)
		pElement = XMLNode->FirstChildElement("trackTarget");
		if(pElement)
			processTrackTarget(pElement, pNode);
	 
		// Process node (*)
		pElement = XMLNode->FirstChildElement("node");
		while(pElement)
		{
			processNode(pElement, pNode);
			pElement = pElement->NextSiblingElement("node");
		}
	 
		// Process entity (*)
		pElement = XMLNode->FirstChildElement("entity");
		while(pElement)
		{
			processEntity(pElement, pNode);
			pElement = pElement->NextSiblingElement("entity");
		}
	 
		// Process light (*)
		pElement = XMLNode->FirstChildElement("light");
		while(pElement)
		{
			processLight(pElement, pNode);
			pElement = pElement->NextSiblingElement("light");
		}
	 
		// Process camera (*)
		pElement = XMLNode->FirstChildElement("camera");
		while(pElement)
		{
			processCamera(pElement, pNode);
			pElement = pElement->NextSiblingElement("camera");
		}
	 
		// Process particleSystem (*)
		pElement = XMLNode->FirstChildElement("particleSystem");
		while(pElement)
		{
			processParticleSystem(pElement, pNode);
			pElement = pElement->NextSiblingElement("particleSystem");
		}
	 
		// Process billboardSet (*)
		pElement = XMLNode->FirstChildElement("billboardSet");
		while(pElement)
		{
			processBillboardSet(pElement, pNode);
			pElement = pElement->NextSiblingElement("billboardSet");
		}
	 
		// Process plane (*)
		pElement = XMLNode->FirstChildElement("plane");
		while(pElement)
		{
			processPlane(pElement, pNode);
			pElement = pElement->NextSiblingElement("plane");
		}
	 
		// Process userDataReference (?)
		pElement = XMLNode->FirstChildElement("userDataReference");
		if(pElement)
			processUserDataReference(pElement, pNode);
		
		pElement = pElement->GetNext();
	}
	
	return result;
}
 
void RenderDotSceneLoader::processLookTarget(TiXmlElement *XMLNode, SceneNode *pParent)
{
    //! @todo Is this correct? Cause I don't have a clue actually
 
    // Process attributes
    String nodeName = getAttrib(XMLNode, "nodeName");
 
    Node::TransformSpace relativeTo = Node::TS_PARENT;
    String sValue = getAttrib(XMLNode, "relativeTo");
    if(sValue == "local")
        relativeTo = Node::TS_LOCAL;
    else if(sValue == "parent")
        relativeTo = Node::TS_PARENT;
    else if(sValue == "world")
        relativeTo = Node::TS_WORLD;
 
    TiXmlElement *pElement;
 
    // Process position (?)
    Vector3 position;
    pElement = XMLNode->FirstChildElement("position");
    if(pElement)
        position = parseVector3(pElement);
 
    // Process localDirection (?)
    Vector3 localDirection = Vector3::NEGATIVE_UNIT_Z;
    pElement = XMLNode->FirstChildElement("localDirection");
    if(pElement)
        localDirection = parseVector3(pElement);
 
    // Setup the look target
    try
    {
        if(!nodeName.empty())
        {
            SceneNode *pLookNode = m_sceneMgr->getSceneNode(nodeName);
            position = pLookNode->_getDerivedPosition();
        }
 
        pParent->lookAt(position, relativeTo, localDirection);
    }
    catch(Ogre::Exception &/*e*/)
    {
        LogManager::getSingleton().logMessage("[RenderDotSceneLoader] Error processing a look target!");
    }
}
 
void RenderDotSceneLoader::processTrackTarget(TiXmlElement *XMLNode, SceneNode *pParent)
{
    // Process attributes
    String nodeName = getAttrib(XMLNode, "nodeName");
 
    TiXmlElement *pElement;
 
    // Process localDirection (?)
    Vector3 localDirection = Vector3::NEGATIVE_UNIT_Z;
    pElement = XMLNode->FirstChildElement("localDirection");
    if(pElement)
        localDirection = parseVector3(pElement);
 
    // Process offset (?)
    Vector3 offset = Vector3::ZERO;
    pElement = XMLNode->FirstChildElement("offset");
    if(pElement)
        offset = parseVector3(pElement);
 
    // Setup the track target
    try
    {
        SceneNode *pTrackNode = m_sceneMgr->getSceneNode(nodeName);
        pParent->setAutoTracking(true, pTrackNode, localDirection, offset);
    }
    catch(Ogre::Exception &/*e*/)
    {
        LogManager::getSingleton().logMessage("[RenderDotSceneLoader] Error processing a track target!");
    }
}
 
void RenderDotSceneLoader::processEntity(TiXmlElement *XMLNode, SceneNode *pParent)
{
    // Process attributes
    String name = getAttrib(XMLNode, "name");
    String id = getAttrib(XMLNode, "id");
    String meshFile = getAttrib(XMLNode, "meshFile");
    String materialFile = getAttrib(XMLNode, "materialFile");
    bool isStatic = getAttribBool(XMLNode, "static", false);;
    bool castShadows = getAttribBool(XMLNode, "castShadows", true);
 
    // TEMP: Maintain a list of static and dynamic objects
    if(isStatic)
        staticObjects.push_back(name);
    else
        dynamicObjects.push_back(name);
 
    TiXmlElement *pElement;
 
    // Process vertexBuffer (?)
    pElement = XMLNode->FirstChildElement("vertexBuffer");
    if(pElement)
        ;//processVertexBuffer(pElement);
 
    // Process indexBuffer (?)
    pElement = XMLNode->FirstChildElement("indexBuffer");
    if(pElement)
        ;//processIndexBuffer(pElement);
 
    // Create the entity
    Entity *pEntity = 0;
    try
    {
        MeshManager::getSingleton().load(meshFile, m_sGroupName);
        pEntity = m_sceneMgr->createEntity(name, meshFile);
        pEntity->setCastShadows(castShadows);
        pParent->attachObject(pEntity);
 
        if(!materialFile.empty())
            pEntity->setMaterialName(materialFile);
    }
    catch(Ogre::Exception &/*e*/)
    {
        LogManager::getSingleton().logMessage("[RenderDotSceneLoader] Error loading an entity!");
    }
 
    // Process userDataReference (?)
    pElement = XMLNode->FirstChildElement("userDataReference");
    if(pElement)
        processUserDataReference(pElement, pEntity);
 
 
}
 
void RenderDotSceneLoader::processParticleSystem(TiXmlElement *XMLNode, SceneNode *pParent)
{
    // Process attributes
    String name = getAttrib(XMLNode, "name");
    String id = getAttrib(XMLNode, "id");
    String file = getAttrib(XMLNode, "file");
 
    // Create the particle system
    try
    {
        ParticleSystem *pParticles = m_sceneMgr->createParticleSystem(name, file);
        pParent->attachObject(pParticles);
    }
    catch(Ogre::Exception &/*e*/)
    {
        LogManager::getSingleton().logMessage("[RenderDotSceneLoader] Error creating a particle system!");
    }
}
 
void RenderDotSceneLoader::processBillboardSet(TiXmlElement *XMLNode, SceneNode *pParent)
{
    //! @todo Implement this
}
 
void RenderDotSceneLoader::processPlane(TiXmlElement *XMLNode, SceneNode *pParent)
{
    //! @todo Implement this
}
 
void RenderDotSceneLoader::processFog(TiXmlElement *XMLNode)
{
    // Process attributes
    Real expDensity = getAttribReal(XMLNode, "expDensity", 0.001);
    Real linearStart = getAttribReal(XMLNode, "linearStart", 0.0);
    Real linearEnd = getAttribReal(XMLNode, "linearEnd", 1.0);
 
    FogMode mode = FOG_NONE;
    String sMode = getAttrib(XMLNode, "mode");
    if(sMode == "none")
        mode = FOG_NONE;
    else if(sMode == "exp")
        mode = FOG_EXP;
    else if(sMode == "exp2")
        mode = FOG_EXP2;
    else if(sMode == "linear")
        mode = FOG_LINEAR;
 
    TiXmlElement *pElement;
 
    // Process colourDiffuse (?)
    ColourValue colourDiffuse = ColourValue::White;
    pElement = XMLNode->FirstChildElement("colourDiffuse");
    if(pElement)
        colourDiffuse = parseColour(pElement);
 
    // Setup the fog
    m_sceneMgr->setFog(mode, colourDiffuse, expDensity, linearStart, linearEnd);
}
 
void RenderDotSceneLoader::processSkyBox(TiXmlElement *XMLNode)
{
    // Process attributes
    String material = getAttrib(XMLNode, "material");
    Real distance = getAttribReal(XMLNode, "distance", 5000);
    bool drawFirst = getAttribBool(XMLNode, "drawFirst", true);
 
    TiXmlElement *pElement;
 
    // Process rotation (?)
    Quaternion rotation = Quaternion::IDENTITY;
    pElement = XMLNode->FirstChildElement("rotation");
    if(pElement)
        rotation = parseQuaternion(pElement);
 
    // Setup the sky box
    m_sceneMgr->setSkyBox(true, material, distance, drawFirst, rotation, m_sGroupName);
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
 
void RenderDotSceneLoader::processLightRange(TiXmlElement *XMLNode, Light *pLight)
{
    // Process attributes
    Real inner = getAttribReal(XMLNode, "inner");
    Real outer = getAttribReal(XMLNode, "outer");
    Real falloff = getAttribReal(XMLNode, "falloff", 1.0);
 
    // Setup the light range
    pLight->setSpotlightRange(Angle(inner), Angle(outer), falloff);
}
 
void RenderDotSceneLoader::processLightAttenuation(TiXmlElement *XMLNode, Light *pLight)
{
    // Process attributes
    Real range = getAttribReal(XMLNode, "range");
    Real constant = getAttribReal(XMLNode, "constant");
    Real linear = getAttribReal(XMLNode, "linear");
    Real quadratic = getAttribReal(XMLNode, "quadratic");
 
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
 
Real RenderDotSceneLoader::getAttribReal(TiXmlElement *XMLNode, const String &attrib, Real defaultValue)
{
    if(XMLNode->Attribute(attrib))
        return StringConverter::parseReal(XMLNode->Attribute(attrib));
    else
        return defaultValue;
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
 
Vector3 RenderDotSceneLoader::parseVector3(TiXmlElement *XMLNode)
{
    return Vector3(
        StringConverter::parseReal(XMLNode->Attribute("x")),
        StringConverter::parseReal(XMLNode->Attribute("y")),
        StringConverter::parseReal(XMLNode->Attribute("z"))
    );
}
 
Quaternion RenderDotSceneLoader::parseQuaternion(TiXmlElement *XMLNode)
{
    //! @todo Fix this crap!
 
    Quaternion orientation;
 
    if(XMLNode->Attribute("qx"))
    {
        orientation.x = StringConverter::parseReal(XMLNode->Attribute("qx"));
        orientation.y = StringConverter::parseReal(XMLNode->Attribute("qy"));
        orientation.z = StringConverter::parseReal(XMLNode->Attribute("qz"));
        orientation.w = StringConverter::parseReal(XMLNode->Attribute("qw"));
    }
    else if(XMLNode->Attribute("axisX"))
    {
        Vector3 axis;
        axis.x = StringConverter::parseReal(XMLNode->Attribute("axisX"));
        axis.y = StringConverter::parseReal(XMLNode->Attribute("axisY"));
        axis.z = StringConverter::parseReal(XMLNode->Attribute("axisZ"));
        Real angle = StringConverter::parseReal(XMLNode->Attribute("angle"));;
        orientation.FromAngleAxis(Ogre::Angle(angle), axis);
    }
    else if(XMLNode->Attribute("angleX"))
    {
        Vector3 axis;
        axis.x = StringConverter::parseReal(XMLNode->Attribute("angleX"));
        axis.y = StringConverter::parseReal(XMLNode->Attribute("angleY"));
        axis.z = StringConverter::parseReal(XMLNode->Attribute("angleZ"));
        //orientation.FromAxes(&axis);
        //orientation.F
    }
 
    return orientation;
}
 
ColourValue RenderDotSceneLoader::parseColour(TiXmlElement *XMLNode)
{
    return ColourValue(
        StringConverter::parseReal(XMLNode->Attribute("r")),
        StringConverter::parseReal(XMLNode->Attribute("g")),
        StringConverter::parseReal(XMLNode->Attribute("b")),
        XMLNode->Attribute("a") != NULL ? StringConverter::parseReal(XMLNode->Attribute("a")) : 1
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
