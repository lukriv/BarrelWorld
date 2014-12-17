#include "gmenumgr.h"

#include <CEGUI/RendererModules/Ogre/Renderer.h>

GameMenuSystem::GameMenuSystem(GameLogger *pLogger) : m_pLogger(pLogger)
													, m_pGuiSystem(nullptr) {}

GameMenuSystem::~GameMenuSystem()
{
	Uninitialize();
}

GameErrorCode GameMenuSystem::Initialize( Ogre::RenderWindow* pWindow )
{
	
	CEGUI::OgreRenderer& ogreRenderer =  CEGUI::OgreRenderer::create (*pWindow);
	CEGUI::OgreResourceProvider* pOgreResource = &CEGUI::OgreRenderer::createOgreResourceProvider();
	m_pGuiSystem = &CEGUI::System::create( ogreRenderer, reinterpret_cast<CEGUI::ResourceProvider*>(pOgreResource) );
	
	return FWG_NO_ERROR;
}

void GameMenuSystem::Uninitialize()
{
	
    if (m_pGuiSystem)
	{
		// get extern renderer and resource provider
		CEGUI::OgreRenderer* pRenderer = reinterpret_cast<CEGUI::OgreRenderer*>(m_pGuiSystem->getRenderer());
		CEGUI::OgreResourceProvider* pResourceProvider = reinterpret_cast<CEGUI::OgreResourceProvider*>(m_pGuiSystem->getResourceProvider());
	
		// destroy cegui system
		CEGUI::System::destroy();
		m_pGuiSystem = nullptr;

		CEGUI::OgreRenderer::destroyOgreResourceProvider(*pResourceProvider);
		CEGUI::OgreRenderer::destroy(*pRenderer);
	}
	
}

