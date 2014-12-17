#include "ginputsystem.h"

#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreStringConverter.h>
#include <CEGUI/CEGUI.h>

CEGUI::MouseButton ConvertMouseButtonId(OIS::MouseButtonID mouseButtonId)
{
	switch(mouseButtonId)
	{
	case OIS::MB_Left:
		return CEGUI::LeftButton;
		
	case OIS::MB_Right:
		return CEGUI::RightButton;
		
	case OIS::MB_Middle:
		return CEGUI::MiddleButton;
		
	case OIS::MB_Button3: 
		return CEGUI::X1Button;
		
	case OIS::MB_Button4:
		return CEGUI::X2Button;
		
	default:
		return CEGUI::NoButton;
	}
}

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
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(static_cast<float>(arg.state.X.abs), static_cast<float>(arg.state.Y.abs));
	return true;
}

bool GameInputSystem::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(ConvertMouseButtonId(id));
	return true;
}

bool GameInputSystem::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(ConvertMouseButtonId(id));
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

	m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputMgr->createInputObject(OIS::OISKeyboard, true));
	m_pMouse = static_cast<OIS::Mouse*>(m_pInputMgr->createInputObject(OIS::OISMouse, true));
	
	m_pMouse->getMouseState().height = pRenderWindow->getHeight();
	m_pMouse->getMouseState().width = pRenderWindow->getWidth();
	
	m_pKeyboard->setEventCallback(this);
	m_pMouse->setEventCallback(this);
	
	return FWG_NO_ERROR;
}

void GameInputSystem::Uninitialize()
{
	if(m_pInputMgr)
	{
		if(m_pKeyboard)
		{
			m_pInputMgr->destroyInputObject(m_pKeyboard);
			m_pKeyboard = nullptr;
		}
		
		if(m_pMouse)
		{
			m_pInputMgr->destroyInputObject(m_pMouse);
			m_pMouse = nullptr;
		}
		
		OIS::InputManager::destroyInputSystem(m_pInputMgr);
		m_pInputMgr = nullptr;
	}
}

GameInputSystem::~GameInputSystem()
{
	Uninitialize();
}

GameErrorCode GameInputSystem::ProcessInputs()
{
	m_pKeyboard->capture();
	m_pMouse->capture();
	
	return FWG_NO_ERROR;
}
