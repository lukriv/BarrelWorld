#include "ginputsystem.h"

#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreStringConverter.h>

#include <GameResHold/gdeftables.h>
#include "ginputcomp.h"
#include "gcharinput.h"
#include "ginputfreecam.h"


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
	wxCriticalSectionLocker lock(m_callbackArrayLock);
	for (auto iter = m_mouseCallbacks.Begin(); iter != m_mouseCallbacks.End(); ++iter)
	{
		(*iter)->OnMouseMoved(arg.state);
	}
	return true;
}

bool GameInputSystem::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	wxCriticalSectionLocker lock(m_callbackArrayLock);
	for (auto iter = m_mouseCallbacks.Begin(); iter != m_mouseCallbacks.End(); ++iter)
	{
		(*iter)->OnMousePressed(arg.state, id);
	}
	return true;
}

bool GameInputSystem::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	wxCriticalSectionLocker lock(m_callbackArrayLock);
	for (auto iter = m_mouseCallbacks.Begin(); iter != m_mouseCallbacks.End(); ++iter)
	{
		(*iter)->OnMouseReleased(arg.state, id);
	}
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




GameErrorCode GameInputSystem::CreateAndRegisterInputComponent( const InputDef &inputDef, InputComponent *&pNewInputComp)
{
	switch (inputDef.m_inputType) {
		case InputDef::INPUT_TYPE_GENERIC:
			break;
		case InputDef::INPUT_TYPE_CHARACTER:
			return CharacterInput::CreateObject(inputDef, *this, pNewInputComp);
		case InputDef::INPUT_TYPE_FREE_CAMERA:
			return FreeCameraInput::CreateObject(inputDef, *this, pNewInputComp);
		case InputDef::INPUT_TYPE_GENERIC_MOUSE:
			return GenericMouseInput::CreateObject(inputDef, *this, pNewInputComp);
		default:
			FWGLOG_ERROR(wxT("Unknown input component type"), m_spLogger);
			return FWG_E_INVALID_PARAMETER_ERROR;
			break;
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode GameInputSystem::RegisterCallback(InputMouseCallback* pMouseCallback)
{
	if(pMouseCallback != nullptr)
	{
		wxCriticalSectionLocker lock(m_callbackArrayLock);
		if(m_mouseCallbacks.Exists(pMouseCallback))
		{
			return FWG_E_OBJECT_ALREADY_EXISTS_ERROR;
		} else {
			return m_mouseCallbacks.Insert(pMouseCallback);
		}
	}
	
	return FWG_E_INVALID_PARAMETER_ERROR;
}

void GameInputSystem::UnregisterCallback(InputMouseCallback* pMouseCallback)
{
	if(pMouseCallback != nullptr)
	{
		wxCriticalSectionLocker lock(m_callbackArrayLock);
		if(m_mouseCallbacks.Exists(pMouseCallback))
		{
			m_mouseCallbacks.Remove(pMouseCallback);
		}
	}
}
