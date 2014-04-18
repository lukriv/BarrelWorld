#include "gclientengine.h"
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreMeshManager.h>
#include <OGRE/OgreSubMesh.h>
#include "../GameSystem/gerror.h"
#include "../GameSystem/new.h"
#include "../GameLoader/gtestloader.h"
#include "MyGUI/MyGUI_PointerManager.h"
#include "MyGUI/MyGUI_Button.h"


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
	
	m_pSceneManager = m_pRoot->createSceneManager(Ogre::ST_GENERIC);
	
	size_t hWnd = 0;
	OIS::ParamList paramList;
	m_pRenderWindow->getCustomAttribute(Ogre::String("WINDOW"), &hWnd);
 
	paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));
	
	m_pInputMgr = OIS::InputManager::createInputSystem(paramList);
	
	FWG_RETURN_FAIL(GameNewChecked(m_pInputComp, m_pInputMgr));
	
	if(FWG_FAILED( result = m_pInputComp->Initialize(m_pRenderWindow->getWidth(), m_pRenderWindow->getHeight())))
	{
		return result;
	}
	
	// initialize resources
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("res", "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, false);
	
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	
	// initialize MyGUI
	m_pGuiPlatform = new MyGUI::OgrePlatform();
	m_pGuiPlatform->initialise(m_pRenderWindow, m_pSceneManager);
	
	m_pGui = new MyGUI::Gui();
	m_pGui->initialise();
	
	// initialize factory and game resources
	FWG_RETURN_FAIL(GameNewChecked(m_spEntityFactory.OutRef()));
	
	FWG_RETURN_FAIL(m_spEntityFactory->Initialize(m_pLogger));
	
	FWG_RETURN_FAIL(GameNewChecked(m_spDefHolder.OutRef()));
	
	GameTestResourceLoader loader;
	
	FWG_RETURN_FAIL(loader.Initialize(m_pLogger));
	
	FWG_RETURN_FAIL(loader.Load(*m_spDefHolder));
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
	GameEntityFactory::InOutSystems entityParams;
	
	entityParams.m_pSceneMgr = m_pSceneManager;
	
	// create scene manager
	FWG_RETURN_FAIL(m_spEntityFactory.In()->CreateAllEntities(*m_spDefHolder,entityParams, m_componentManager));
	
	// Create the camera
	Ogre::Camera* camera = m_pSceneManager->createCamera("PlayerCam");
 
	// Position it at 500 in Z direction
	camera->setPosition(Ogre::Vector3(-8,0,0));
	// Look back along -Z
	camera->lookAt(Ogre::Vector3(0,0,0));
	camera->setNearClipDistance(1);
	
	m_pSceneManager->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
	
	// Create a Light and set its position
    Ogre::Light* light = m_pSceneManager->createLight("MainLight");
    light->setPosition(20.0f, 80.0f, 50.0f);
	
	Ogre::Viewport *viewPort = m_pRenderWindow->addViewport(camera);
	viewPort->setBackgroundColour(Ogre::ColourValue(0.0f,0.0f,0.0f));
	
	viewPort->setOverlaysEnabled(true);
	
	m_pGuiPlatform->getRenderManagerPtr()->setActiveViewport(0);
	
	camera->setAspectRatio(Ogre::Real(viewPort->getActualWidth()) / Ogre::Real(viewPort->getActualHeight()));
	
	MyGUI::ButtonPtr button = m_pGui->createWidget<MyGUI::Button>("Button", 10, 10, 300, 26, MyGUI::Align::Default, "Main");
	button->setCaption("exit");
	// set callback
	button->eventMouseButtonClick += MyGUI::newDelegate(this, &GameClientEngine::SetExit); // CLASS_POINTER is pointer to instance of a 
	
	
	
	while(!m_pInputComp->Exit()) 
	{
		m_pInputComp->ProcessInputs();
		m_pRoot->renderOneFrame();
		wxThread::Sleep(17);
	}
	return result;
}



GameErrorCode GameClientEngine::CreateTestingWorld()
{
	GameErrorCode result = FWG_NO_ERROR;

	Ogre::MeshPtr spMsh = Ogre::MeshManager::getSingleton().createManual("TestingCube", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	
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
    pRenSys->convertColourValue(Ogre::ColourValue(1.0,0.0,0.0), pColour++); //0 colour
    pRenSys->convertColourValue(Ogre::ColourValue(1.0,1.0,0.0), pColour++); //1 colour
    pRenSys->convertColourValue(Ogre::ColourValue(0.0,1.0,0.0), pColour++); //2 colour
    pRenSys->convertColourValue(Ogre::ColourValue(0.0,0.0,0.0), pColour++); //3 colour
    pRenSys->convertColourValue(Ogre::ColourValue(1.0,0.0,1.0), pColour++); //4 colour
    pRenSys->convertColourValue(Ogre::ColourValue(1.0,1.0,1.0), pColour++); //5 colour
    pRenSys->convertColourValue(Ogre::ColourValue(0.0,1.0,1.0), pColour++); //6 colour
    pRenSys->convertColourValue(Ogre::ColourValue(0.0,0.0,1.0), pColour++); //7 colour
	
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
    spMsh->sharedVertexData = new Ogre::VertexData();
    spMsh->sharedVertexData->vertexCount = nVertices;
	
	/// Create declaration (memory format) of vertex data
    Ogre::VertexDeclaration* decl = spMsh->sharedVertexData->vertexDeclaration;
	size_t offset = 0;
	
	// 1st buffer
    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
	
	/// Allocate vertex buffer of the requested number of vertices (vertexCount) 
    /// and bytes per vertex (offset)
    Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
								offset, spMsh->sharedVertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	
	/// Upload the vertex data to the card
    vbuf->writeData(0, vbuf->getSizeInBytes(), vertices, true);
 
    /// Set vertex buffer binding so buffer 0 is bound to our vertex buffer
    Ogre::VertexBufferBinding* bind = spMsh->sharedVertexData->vertexBufferBinding; 
    bind->setBinding(0, vbuf);
	
	// 2nd buffer
    offset = 0;
    decl->addElement(1, offset, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR);
    /// Allocate vertex buffer of the requested number of vertices (vertexCount) 
    /// and bytes per vertex (offset)
    vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
        offset, spMsh->sharedVertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
    /// Upload the vertex data to the card
    vbuf->writeData(0, vbuf->getSizeInBytes(), colours, true);
 
    /// Set vertex buffer binding so buffer 1 is bound to our colour buffer
    bind->setBinding(1, vbuf);
 
	
	/// Allocate index buffer of the requested number of vertices (ibufCount) 
    Ogre::HardwareIndexBufferSharedPtr ibuf = Ogre::HardwareBufferManager::getSingleton().
        createIndexBuffer(
        Ogre::HardwareIndexBuffer::IT_16BIT, 
        ibufCount, 
        Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
 
    /// Upload the index data to the card
    ibuf->writeData(0, ibuf->getSizeInBytes(), faces, true);
 
    /// Set parameters of the submesh
    pSubMesh->useSharedVertices = true;
    pSubMesh->indexData->indexBuffer = ibuf;
    pSubMesh->indexData->indexCount = ibufCount;
    pSubMesh->indexData->indexStart = 0;
 
    /// Set bounding information (for culling)
    spMsh->_setBounds(Ogre::AxisAlignedBox(-1,-1,-1,1,1,1));
    spMsh->_setBoundingSphereRadius(Ogre::Math::Sqrt(3*1*1));
 
    /// Notify -Mesh object that it has been loaded
    spMsh->load();
	
	
	//material prepare
	Ogre::MaterialPtr spMaterial = Ogre::MaterialManager::getSingleton().create(
				"Test/ColourTest",
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	
	spMaterial->getTechnique(0)->getPass(0)->setVertexColourTracking(Ogre::TVC_AMBIENT);
	
	return result;
	
}

GameClientEngine::~GameClientEngine()
{
	if(m_pGui)
	{
		m_pGui->shutdown();
		delete m_pGui;
		m_pGui = NULL;
	}
	
	if(m_pGuiPlatform)
	{
		m_pGuiPlatform->shutdown();
		delete m_pGuiPlatform;
		m_pGuiPlatform = NULL;
	}
}
