#include "ginputsystem.h"
#include "MyGUI/MyGUI_InputManager.h"


bool GameInputSystem::keyPressed(const OIS::KeyEvent& arg)
{
	if(m_callbackArray[arg.key] != 0)
	{
		m_callbackArray[arg.key]->Invoke(true);
	}
	return true;
}

bool GameInputSystem::keyReleased(const OIS::KeyEvent& arg)
{
	if(m_callbackArray[arg.key] != 0)
	{
		m_callbackArray[arg.key]->Invoke(false);
	}
	return true;
}

bool GameInputSystem::mouseMoved(const OIS::MouseEvent& arg)
{
	MyGUI::InputManager::getInstance().injectMouseMove(arg.state.X.abs, arg.state.Y.abs, arg.state.Z.abs);
	return true;
}

bool GameInputSystem::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	MyGUI::InputManager::getInstance().injectMousePress(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
	return true;
}

bool GameInputSystem::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	MyGUI::InputManager::getInstance().injectMouseRelease(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
	return true;
}

GameErrorCode GameInputSystem::Initialize(wxInt32 width, wxInt32 height)
{
	m_pKeyboard = static_cast<OIS::Keyboard*>(m_pParent->createInputObject(OIS::OISKeyboard, true));
	m_pMouse = static_cast<OIS::Mouse*>(m_pParent->createInputObject(OIS::OISMouse, true));
	
	m_pMouse->getMouseState().height = height;
	m_pMouse->getMouseState().width = width;
	
	m_pKeyboard->setEventCallback(this);
	m_pMouse->setEventCallback(this);
	
	return FWG_NO_ERROR;
}

GameInputSystem::~GameInputSystem()
{
	if(m_pKeyboard)
	{
		m_pParent->destroyInputObject(m_pKeyboard);
	}
	
	if(m_pMouse)
	{
		m_pParent->destroyInputObject(m_pMouse);
	}
}

GameErrorCode GameInputSystem::ProcessInputs()
{
	m_pKeyboard->capture();
	m_pMouse->capture();
	
	return FWG_NO_ERROR;
}
