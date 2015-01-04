#include "gmenumgr.h"

#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <wx/encconv.h>

GameMenuSystem::GameMenuSystem(GameLogger *pLogger) : m_pLogger(pLogger)
													, m_pGuiSystem(nullptr) {}

GameMenuSystem::~GameMenuSystem()
{
	Uninitialize();
}

GameErrorCode GameMenuSystem::Initialize( Ogre::RenderWindow* pWindow )
{
	GameErrorCode result = FWG_NO_ERROR;
	try {
		CEGUI::OgreRenderer& ogreRenderer =  CEGUI::OgreRenderer::create (*pWindow);
		CEGUI::OgreResourceProvider* pOgreResource = &CEGUI::OgreRenderer::createOgreResourceProvider();
		m_pGuiSystem = &CEGUI::System::create( ogreRenderer, reinterpret_cast<CEGUI::ResourceProvider*>(pOgreResource) );
	} catch (CEGUI::Exception &exc) {
		
		//if(!convert.IsOk())
		//{
		//	FWGLOG_WARNING(wxT("Converter is not OK!!!"), m_pLogger);
		//}
		
		FWGLOG_ERROR_FORMAT(wxT("CEGUI error '%s', message: '%s', function: '%s', filename: '%s', line: '%d' "), m_pLogger
						, exc.getName().c_str()
						, exc.getMessage().c_str()
						, exc.getFunctionName().c_str()
						, exc.getFileName().c_str()
						, exc.getLine()
						, FWGLOG_ENDVAL );
		result = FWG_E_MENU_SYSTEM_ERROR;
	}
	
	return result;
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

