#include "gmenu.h"

#include <MyGUI/MyGUI_Button.h>

void GameMenu::SwitchExitButton()
{
	m_pButtonExit->setVisible(!m_pButtonExit->getVisible());
}

GameErrorCode GameMenu::Create()
{
	GameErrorCode result = FWG_NO_ERROR;
	
	m_pButtonExit = m_pMenuRes->GetGuiManager()->createWidget<MyGUI::Button>("Button", 10, 10, 200, 26, MyGUI::Align::Default, "Main", "exit");
	m_pButtonExit->setCaption("Exit");
	// set callback
	m_pButtonExit->eventMouseButtonClick += MyGUI::newDelegate(m_pClientLogic, &ClientGameLogic::SetExitMenu); // CLASS_POINTER is pointer to instance of a 
	
	m_pButtonSwitch = m_pMenuRes->GetGuiManager()->createWidget<MyGUI::Button>("Button", 10, 40, 200, 26, MyGUI::Align::Default, "Main", "exit_display");
	m_pButtonSwitch->setCaption("Show Exit Button");
	m_pButtonSwitch->eventMouseButtonClick += MyGUI::newDelegate(m_pClientLogic, &ClientGameLogic::SetShowExit); // CLASS_POINTER is pointer to instance of a 
	
	return result;
}

void GameMenu::Destroy()
{
	m_pMenuRes->GetGuiManager()->destroyWidget(m_pButtonExit);
	m_pButtonExit = NULL;
	
	m_pMenuRes->GetGuiManager()->destroyWidget(m_pButtonSwitch);
	m_pButtonSwitch = NULL;
	
}

GameMenu::GameMenu() : GameMenuBase()
					, m_pButtonExit(nullptr)
					, m_pButtonSwitch(nullptr) {}


GameErrorCode GameMenu::Initialize()
{
	
	return FWG_NO_ERROR;
}

GameMenu::~GameMenu()
{
	Destroy();
}
