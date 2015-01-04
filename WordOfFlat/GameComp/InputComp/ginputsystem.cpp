#include "ginputsystem.h"

#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreStringConverter.h>
#include <CEGUI/CEGUI.h>

#include <GameResHold/gdeftables.h>
#include "ginputcomp.h"


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
	wxCriticalSectionLocker lock(m_callbackArrayLock);
	if(m_callbackArray[arg.key] != 0)
	{
		m_callbackArray[arg.key]->Invoke(true);
	}
	return true;
}

bool GameInputSystem::keyReleased(const OIS::KeyEvent& arg)
{
	wxCriticalSectionLocker lock(m_callbackArrayLock);
	if(m_callbackArray[arg.key] != 0)
	{
		m_callbackArray[arg.key]->Invoke(false);
	}
	return true;
}

bool GameInputSystem::mouseMoved(const OIS::MouseEvent& arg)
{
	if(m_processMenuEvents)
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(static_cast<float>(arg.state.X.abs), static_cast<float>(arg.state.Y.abs));
	return true;
}

bool GameInputSystem::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	if(m_processMenuEvents)
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(ConvertMouseButtonId(id));
	return true;
}

bool GameInputSystem::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	if(m_processMenuEvents)
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(ConvertMouseButtonId(id));
	return true;
}

GameErrorCode GameInputSystem::Initialize(Ogre::RenderWindow * pRenderWindow, bool processMenuEvents)
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
	
	m_processMenuEvents = processMenuEvents;

	m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputMgr->createInputObject(OIS::OISKeyboard, true));
	m_pMouse = static_cast<OIS::Mouse*>(m_pInputMgr->createInputObject(OIS::OISMouse, true));
	
	m_pMouse->getMouseState().height = pRenderWindow->getHeight();
	m_pMouse->getMouseState().width = pRenderWindow->getWidth();
	
	m_pKeyboard->setEventCallback(this);
	m_pMouse->setEventCallback(this);
	
	return FWG_NO_ERROR;
}

void GameInputSystem::UnregisterAllCallbacks()
{
	wxCriticalSectionLocker lock(m_callbackArrayLock);
	for (wxDword i = 0; i < 255; ++i)
	{
		if(m_callbackArray[i] != nullptr)
		{
			GameDelete(m_callbackArray[i]);
			m_callbackArray[i] = nullptr;
		}
	}
	
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


/// create methods ///
static const wxChar* FACTORY_INPUT_UP		 = wxT("up");
static const wxChar* FACTORY_INPUT_DOWN		 = wxT("down");
static const wxChar* FACTORY_INPUT_LEFT		 = wxT("left");
static const wxChar* FACTORY_INPUT_RIGHT	 = wxT("right");
static const wxChar* FACTORY_INPUT_FORWARD	 = wxT("forward");
static const wxChar* FACTORY_INPUT_BACKWARD	 = wxT("backward");

GameErrorCode GameInputSystem::CreateAndRegisterInputComponent( const InputDef &inputDef, InputComponent *&pNewInputComp)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<InputComponent> spInputComp;
	FWG_RETURN_FAIL(GameNewChecked(spInputComp.OutRef()));
	
	for(InputDef::TInputMap::ConstIterator iter = inputDef.m_inputMap.Begin();
		iter != inputDef.m_inputMap.End();
		++iter)
	{
	
		if(iter->first.Cmp(FACTORY_INPUT_UP) == 0)
		{
			if(FWG_FAILED(result = RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																	, spInputComp.In()
																	, &InputComponent::SetMoveUp))) 
			{
				FWGLOG_ERROR_FORMAT(wxT("Register input callback moveUp failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
		} else if (iter->first.Cmp(FACTORY_INPUT_DOWN) == 0) {
			if(FWG_FAILED(result = RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																	, spInputComp.In()
																	, &InputComponent::SetMoveDown))) 
			{
				FWGLOG_ERROR_FORMAT(wxT("Register input callback moveDown failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
		} else if (iter->first.Cmp(FACTORY_INPUT_LEFT) == 0) {
			if(FWG_FAILED(result = RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																	, spInputComp.In()
																	, &InputComponent::SetMoveLeft))) 
			{
				FWGLOG_ERROR_FORMAT(wxT("Register input callback moveLeft failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
		} else if (iter->first.Cmp(FACTORY_INPUT_RIGHT) == 0) {
			if(FWG_FAILED(result = RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																	, spInputComp.In()
																	, &InputComponent::SetMoveRight))) 
			{
				FWGLOG_ERROR_FORMAT(wxT("Register input callback moveRight failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
		} else if (iter->first.Cmp(FACTORY_INPUT_FORWARD) == 0) {
			if(FWG_FAILED(result = RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																	, spInputComp.In()
																	, &InputComponent::SetMoveForward))) 
			{
				FWGLOG_ERROR_FORMAT(wxT("Register input callback moveForward failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
		} else if (iter->first.Cmp(FACTORY_INPUT_BACKWARD) == 0) {
			if(FWG_FAILED(result = RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																	, spInputComp.In()
																	, &InputComponent::SetMoveBackward))) 
			{
				FWGLOG_ERROR_FORMAT(wxT("Register input callback moveBackward failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
		} else {
			FWGLOG_WARNING_FORMAT(wxT("Cannot register unknown input action '%s'"), m_spLogger, iter->first.GetData().AsInternal(), FWGLOG_ENDVAL);
		}
	}
	
	pNewInputComp = spInputComp.Detach();
	
	return FWG_NO_ERROR;
}



