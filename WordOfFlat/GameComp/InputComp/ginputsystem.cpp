#include "ginputsystem.h"

#include <OGRE/OgreRenderWindow.h>
#include <MyGUI/MyGUI_InputManager.h>


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

GameErrorCode GameInputSystem::Initialize(Ogre::RenderWindow * pRenderWindow)
{
	if(pRenderWindow == nullptr)
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	size_t hWnd = 0;
	OIS::ParamList paramList;
	pRenderWindow->getCustomAttribute(Ogre::String("WINDOW"), &hWnd);

	paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));

	m_pInputMgr = OIS::InputManager::createInputSystem(paramList);
	
	if(m_pInputMgr == nullptr)
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}

	m_pKeyboard = static_cast<OIS::Keyboard*>(m_pParent->createInputObject(OIS::OISKeyboard, true));
	m_pMouse = static_cast<OIS::Mouse*>(m_pParent->createInputObject(OIS::OISMouse, true));
	
	m_pMouse->getMouseState().height = m_pRenderWindow->getHeight();
	m_pMouse->getMouseState().width = pRenderWindow->getWidth();
	
	m_pKeyboard->setEventCallback(this);
	m_pMouse->setEventCallback(this);
	
	return FWG_NO_ERROR;
}

GameInputSystem::~GameInputSystem()
{
	if(m_pInputMgr)
	{
		if(m_pKeyboard)
		{
			m_pInputMgr->destroyInputObject(m_pKeyboard);
		}
		
		if(m_pMouse)
		{
			m_pInputMgr->destroyInputObject(m_pMouse);
		}
		
		OIS::InputManager::destroyInputSystem(m_pInputMgr);
		m_pInputMgr = nullptr;
	}
}

GameErrorCode GameInputSystem::ProcessInputs()
{
	m_pKeyboard->capture();
	m_pMouse->capture();
	
	return FWG_NO_ERROR;
}
