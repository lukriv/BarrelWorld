#include "gclientengine.h"



static const wxChar* TESTING_TITLE = wxT("Flat World Client");

GameErrorCode GameClientEngine::CreateWindow()
{
	GameErrorCode result = FWG_NO_ERROR;
	// Create the main window
	m_pRoot = new Ogre::Root("", "");
	if (m_pRoot == NULL)
	{
		result = FWG_E_MEMORY_ALLOCATION_ERROR;
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateWindow() : Create ogre root failed: 0x%08x"),
			m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	m_pRoot->loadPlugin("RenderSystem_GL");
	Ogre::RenderSystemList::const_iterator it = m_pRoot->getAvailableRenderers().begin();
	Ogre::RenderSystem *pRenSys = NULL;
	
	if(m_pRoot->getAvailableRenderers().size() == 1)
	{
		pRenSys = *it;
	}
	m_pRoot->setRenderSystem(pRenSys);
	m_pRoot->initialise(false);
	
	
	m_pRenderWindow = m_pRoot->createRenderWindow(m_settings.m_screenTitle.ToStdString()
												, m_settings.m_screenWidth
												, m_settings.m_screenHeight
												, false
												, 0);
												
	if(m_pRenderWindow == NULL)
	{
		result = FWG_E_MEMORY_ALLOCATION_ERROR;
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateWindow() : Create render window failed: 0x%08x"),
			m_pLogger, result, FWGLOG_ENDVAL);
		return result;		
	}
												
												
	
	return result;
}

GameErrorCode GameClientEngine::Initialize(GameLogger* pLogger)
{
	GameErrorCode result = FWG_NO_ERROR;
	if (m_isInitialized)
	{
		return FWG_NO_ERROR;
	}
	
	m_pLogger = pLogger;
	
	if(FWG_FAILED(result = LoadSettings()))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::Initialize() : Load setting failed: 0x%08x"),
			m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = CreateWindow()))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::Initialize() : Create window failed: 0x%08x"),
			m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}

	//m_pSceneGenerator = new (std::nothrow) GameTestSceneGenerator();
	//if (m_pSceneGenerator == NULL) 
	//{
	//	return FWG_E_MEMORY_ALLOCATION_ERROR;
	//}
	
	//m_spEntityFactory.Attach(new (std::nothrow) GameEntityFactory());
	//if (m_spEntityFactory == NULL) return FWG_E_MEMORY_ALLOCATION_ERROR;
	
	//if(FWG_FAILED(result = m_spEntityFactory->Initialize(m_spResHolder.In(), pLogger))) 
	//{
	//	FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::Initialize() : Initialize entity factory failed: 0x%08x"),
	//		m_pLogger, result, FWGLOG_ENDVAL);
	//	return result;
	//}
	
	m_isInitialized = true;
	return result;
}

GameErrorCode GameClientEngine::LoadSettings(wxChar* pFileName)
{
	if(pFileName == NULL)
	{
		m_settings.m_screenWidth = 800;
		m_settings.m_screenHeight = 600;
		m_settings.m_screenTitle.assign(TESTING_TITLE);
		m_settings.m_worldName.Clear();
	} else {
		return FWG_E_NOT_IMPLEMENTED_ERROR;
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode GameClientEngine::MainLoop()
{
	GameErrorCode result = FWG_NO_ERROR;

	while(true) {}
	return result;
}



GameErrorCode GameClientEngine::CreateTestingWorld()
{
	GameErrorCode result = FWG_NO_ERROR;

	Ogre::MeshPtr spMsh = Ogre::MeshManager::getSingleton().createManual("TestingCube", "TestGroup");
	
	// create submesh
	Ogre::SubMesh* pSubMesh = spMsh->createSubMesh();
	
	const float sqrt13 = 0.577350269f; // sqrt(1/3) - for normals
	
	/// Define the vertices (8 vertices, each consisting of 2 groups of 3 floats
    const size_t nVertices = 8;
    const size_t vbufCount = 3*2*nVertices;
    float vertices[vbufCount] = {
            -1.0,1.0,-1.0,        //0 position
            -sqrt13,sqrt13,-sqrt13,     //0 normal
            1.0,1.0,-1.0,         //1 position
            sqrt13,sqrt13,-sqrt13,      //1 normal
            1.0,-1.0,-1.0,        //2 position
            sqrt13,-sqrt13,-sqrt13,     //2 normal
            -1.0,-1.0,-1.0,       //3 position
            -sqrt13,-sqrt13,-sqrt13,    //3 normal
            -1.0,1.0,1.0,         //4 position
            -sqrt13,sqrt13,sqrt13,      //4 normal
            1.0,1.0,1.0,          //5 position
            sqrt13,sqrt13,sqrt13,       //5 normal
            1.0,-1.0,1.0,         //6 position
            sqrt13,-sqrt13,sqrt13,      //6 normal
            -1.0,-1.0,1.0,        //7 position
            -sqrt13,-sqrt13,sqrt13,     //7 normal
    };
	
	Ogre::RenderSystem *pRenSys = m_pRoot->getRenderSystem();
	Ogre::RGBA colours[nVertices];
    Ogre::RGBA *pColour = colours;
	
    // Use render system to convert colour value since colour packing varies
    rs->convertColourValue(ColourValue(1.0,0.0,0.0), pColour++); //0 colour
    rs->convertColourValue(ColourValue(1.0,1.0,0.0), pColour++); //1 colour
    rs->convertColourValue(ColourValue(0.0,1.0,0.0), pColour++); //2 colour
    rs->convertColourValue(ColourValue(0.0,0.0,0.0), pColour++); //3 colour
    rs->convertColourValue(ColourValue(1.0,0.0,1.0), pColour++); //4 colour
    rs->convertColourValue(ColourValue(1.0,1.0,1.0), pColour++); //5 colour
    rs->convertColourValue(ColourValue(0.0,1.0,1.0), pColour++); //6 colour
    rs->convertColourValue(ColourValue(0.0,0.0,1.0), pColour++); //7 colour
	
	/// Define 12 triangles (two triangles per cube face)
    /// The values in this table refer to vertices in the above table
    const size_t ibufCount = 36;
    unsigned short faces[ibufCount] = {
            0,2,3,
            0,1,2,
            1,6,2,
            1,5,6,
            4,6,5,
            4,7,6,
            0,7,4,
            0,3,7,
            0,5,1,
            0,4,5,
            2,7,3,
            2,6,7
    };
	
	/// Create vertex data structure for 8 vertices shared between submeshes
    spMsh->sharedVertexData = new VertexData();
    spMsh->sharedVertexData->vertexCount = nVertices;
	
	/// Create declaration (memory format) of vertex data
    Ogre::VertexDeclaration* decl = spMsh->sharedVertexData->vertexDeclaration;
	size_t offset = 0;
	
	// 1st buffer
    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
    offset += VertexElement::getTypeSize(Ogre::VET_FLOAT3);
    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
    offset += VertexElement::getTypeSize(Ogre::VET_FLOAT3);
	
	/// Allocate vertex buffer of the requested number of vertices (vertexCount) 
    /// and bytes per vertex (offset)
    HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
								offset, spMsh->sharedVertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	
	/// Upload the vertex data to the card
    vbuf->writeData(0, vbuf->getSizeInBytes(), vertices, true);
 
    /// Set vertex buffer binding so buffer 0 is bound to our vertex buffer
    Ogre::VertexBufferBinding* bind = msh->sharedVertexData->vertexBufferBinding; 
    bind->setBinding(0, vbuf);
	
	return result;
	
}


