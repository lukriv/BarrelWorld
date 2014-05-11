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

GameErrorCode GameMenu::PrepareIngameMenu(ClientGameLogic* pGameLogic)
{
	GameErrorCode result = FWG_NO_ERROR;
	
	m_pGuiPlatform->getRenderManagerPtr()->setActiveViewport(0);
	
	MyGUI::ButtonPtr button = m_pGui->createWidget<MyGUI::Button>("Button", 10, 10, 300, 26, MyGUI::Align::Default, "Main");
	button->setCaption("exit");
	// set callback
	button->eventMouseButtonClick += MyGUI::newDelegate(pGameLogic, &ClientGameLogic::SetExit); // CLASS_POINTER is pointer to instance of a 
	
	return result;
}
