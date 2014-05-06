#include "gmenu.h"

GameMenu::GameMenu() : m_pGui(NULL), m_pGuiPlatform(NULL)
{}

GameMenu::~GameMenu()
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

GameErrorCode GameMenu::Initialize(GameLogger* pLogger, Ogre::RenderWindow* pWindow, Ogre::SceneManager* pSceneManager)
{
	
	m_pLogger = pLogger;
	
	// initialize MyGUI
	FWG_RETURN_FAIL(GameNewChecked(m_pGuiPlatform));
	m_pGuiPlatform->initialise(pWindow, pSceneManager);
	
	m_pGui = new MyGUI::Gui();
	m_pGui->initialise();
}
