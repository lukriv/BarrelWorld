#include "gmenu.h"

#include <MyGUI/MyGUI_Button.h>
#include "gclientlogic.h"

void GameMenu::SwitchExitButton()
{
	m_pButtonExit->setVisible(!m_pButtonExit->getVisible());
}

GameErrorCode GameMenu::Create()
{
	GameErrorCode result = FWG_NO_ERROR;
	
	if(m_visible)
	{
		return FWG_E_INVALID_OPERATION_ERROR;
	}
	
	m_pButtonExit = m_spMenuRes.In()->GetGuiManager()->createWidget<MyGUI::Button>("Button", 10, 10, 200, 26, MyGUI::Align::Default, "Main", "exit");
	m_pButtonExit->setCaption("Exit");
	// set callback
	m_pButtonExit->eventMouseButtonClick += MyGUI::newDelegate(m_pClientLogic, &ClientGameLogic::SetExitMenu); // CLASS_POINTER is pointer to instance of a 
	
	m_pButtonSwitch = m_spMenuRes.In()->GetGuiManager()->createWidget<MyGUI::Button>("Button", 10, 40, 200, 26, MyGUI::Align::Default, "Main", "exit_display");
	m_pButtonSwitch->setCaption("Show Exit Button");
	m_pButtonSwitch->eventMouseButtonClick += MyGUI::newDelegate(m_pClientLogic, &ClientGameLogic::SetShowExit); // CLASS_POINTER is pointer to instance of a 
	
	m_visible = true;
	
	return result;
}

GameErrorCode GameMenu::Destroy()
{
	if(!m_visible)
	{
		return FWG_NO_ERROR;
	}
	m_spMenuRes.In()->GetGuiManager()->destroyWidget(m_pButtonExit);
	m_pButtonExit = NULL;
	
	m_spMenuRes.In()->GetGuiManager()->destroyWidget(m_pButtonSwitch);
	m_pButtonSwitch = NULL;
	
	m_visible = false;
	
	return FWG_NO_ERROR;
}

GameMenu::GameMenu(GameMenuResources* pMenuRes) : GameMenuBase(pMenuRes)
											, m_pClientLogic(NULL)
											, m_visible(false)
{}

GameErrorCode GameMenu::Initialize(Ogre::RenderWindow* pWindow, Ogre::SceneManager* pSceneManager, ClientGameLogic *pCliLogic)
{
	if(pCliLogic == NULL)
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	m_pClientLogic = pCliLogic;
	
	return FWG_NO_ERROR;
}

GameMenu::~GameMenu()
{
	Destroy();
}
