#include "gclientengine.h"
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreMeshManager.h>
#include <OGRE/OgreSubMesh.h>
#include <GameSystem/gerror.h>
#include <GameSystem/new.h>
#include <GameXmlDefinitions/gxmlloader.h>
//#include <GameLoader/gtestloader.h>


static const wxChar* TESTING_TITLE = wxT("Flat World Client");


GameErrorCode GameClientEngine::Initialize(GameLogger* pLogger)
{
	GameErrorCode result = FWG_NO_ERROR;
	if (m_isInitialized) {
		return FWG_NO_ERROR;
	}

	m_pLogger = pLogger;

	// load ogre settings
	if(FWG_FAILED(result = LoadSettings())) {
		FWGLOG_ERROR_FORMAT(wxT("Load setting failed: 0x%08x"),
		                    m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	} else {
		FWGLOG_INFO(wxT("Setting was loaded successfuly"), m_pLogger);
	}
	
	// initialize resources
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("res", "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, false);

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	
	// create new component manager
	FWG_RETURN_FAIL(GameNewChecked(m_spCompManager.OutRef()));

	// create window and sets render system
	if(FWG_FAILED(result = m_spCompManager->Initialize(m_settings))) {
		FWGLOG_ERROR_FORMAT(wxT("Initialize component manager failed: 0x%08x"),
		                    m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	} else {
		FWGLOG_INFO(wxT("Component manager was initialized successfuly"), m_pLogger);
	}

	// initialize game logic (component manager)
	FWG_RETURN_FAIL(GameNewChecked(m_spGameLogic.OutRef()));
	if (FWG_FAILED(result = m_spGameLogic.In()->Initialize(pLogger, m_pRenderWindow, pSceneManager, m_spInputSystem.In(), m_spGameMenu.In()))) {
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::Initialize() : Client Logic initialize failed: 0x%08x"), pLogger, result, FWGLOG_ENDVAL);
		return result;
	} else {
		FWGLOG_INFO(wxT("GameLogic initialization was successful"), m_pLogger);
	}


	// initialize game resources
	FWG_RETURN_FAIL(GameNewChecked(m_spDefHolder.OutRef()));

	//GameTestResourceLoader loader;
    //
	//FWG_RETURN_FAIL(loader.Initialize(m_pLogger));
	GameXmlResourceLoader loader;
	if(FWG_FAILED(result = loader.Initialize(wxT("example.xml"), wxT("res/"), pLogger)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Xml loader initialize failed: 0x%08x"), pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	FWG_RETURN_FAIL(loader.Load(*m_spDefHolder));
	


	FWGLOG_INFO(wxT("GameClientEngine initialized"), pLogger);
	
	m_isInitialized = true;
	return result;
}

GameErrorCode GameClientEngine::LoadSettings(wxChar* pFileName)
{
	if(pFileName == NULL) {
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

	GameRenderListener listener(this);

	FWGLOG_DEBUG(wxT("Main loop started"), m_pLogger);
	
	//register frame listener
	m_pRoot->addFrameListener(&listener);

	if(FWG_FAILED( result = m_spGameLogic->LoadGame(*m_spDefHolder)))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::MainLoop() : load game logic failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	} else {
		FWGLOG_DEBUG(wxT("Game loaded"), m_pLogger);
	}

	if(FWG_FAILED( result = m_spGameLogic->StartGame()))
	{
		FWGLOG_ERROR_FORMAT(wxT("Start game logic failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
		return result;		
	} else {
		FWGLOG_DEBUG(wxT("Game logic started"), m_pLogger);
	}

	//enter render crit section
	//m_spGameLogic.In()->GetRenderLocker().Enter();
	FWGLOG_DEBUG(wxT("Start rendering"), m_pLogger);
	m_pRoot->startRendering();
	
	if(FWG_FAILED(result = m_spGameLogic.In()->StopGame()))
	{
		FWGLOG_ERROR_FORMAT(wxT("Stop game logic failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
		return result;		
	} else {
		FWGLOG_DEBUG(wxT("Game logic stopped"), m_pLogger);
	}
	
	
	FWGLOG_DEBUG(wxT("Removing listener"), m_pLogger);
	m_pRoot->removeFrameListener(&listener);

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
	//m_spGameLogic.In()->Uninitialize();
	//m_spGameMenu.In()->Uninitialize();
	
	m_spGameLogic.Release();
	m_spGameMenu.Release();

	// release Input system and OIS Input Manager
	m_spInputSystem.Release();
	if(m_pInputMgr != nullptr)
	{
		OIS::InputManager::destroyInputSystem(m_pInputMgr);
		m_pInputMgr = nullptr;
	}


}



bool GameClientEngine::GameRenderListener::frameStarted(const Ogre::FrameEvent& evt)
{
	m_pOwner->m_spGameLogic.In()->GetRenderLocker().Enter();
	//FWGLOG_TRACE(wxT("entered to render lock"), m_pOwner->m_pLogger);
	return true;
}

bool GameClientEngine::GameRenderListener::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	// leave render section
	m_pOwner->m_spGameLogic.In()->GetRenderLocker().Leave();
	//FWGLOG_TRACE(wxT("leaved render lock"), m_pOwner->m_pLogger);
	
	// process inputs
	m_pOwner->m_spInputSystem->ProcessInputs();
	if (m_pOwner->m_spGameLogic->IsStopped())
	{
		return false;	
	} else {
		m_pOwner->m_spGameLogic.In()->GetRenderLocker().Enter();
		//FWGLOG_TRACE(wxT("entered to render lock"), m_pOwner->m_pLogger);
		return true;
	}
}

bool GameClientEngine::GameRenderListener::frameEnded(const Ogre::FrameEvent& evt)
{
	m_pOwner->m_spGameLogic.In()->GetRenderLocker().Leave();
	//FWGLOG_TRACE(wxT("leaved render lock"), m_pOwner->m_pLogger);
	return true;
}


