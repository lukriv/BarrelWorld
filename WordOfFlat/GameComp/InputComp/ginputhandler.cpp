#include "ginputhandler.h"
#include <wx/xml/xml.h>
#include <GameXmlDefinitions/gxmldefs.h>
#include <GameXmlDefinitions/gxmlkeydefs.h>
#include <GameXmlDefinitions/gxmlutils.h>

static ControlStruct::MouseButtonState ConvertMouseButtonId(OIS::MouseButtonID id)
{
    switch(id) {
    case OIS::MB_Left:
	return ControlStruct::MOUSE_BUTTON_LEFT;
    case OIS::MB_Right:
	return ControlStruct::MOUSE_BUTTON_RIGHT;
    case OIS::MB_Middle:
	return ControlStruct::MOUSE_BUTTON_MIDDLE;
    default:
	return ControlStruct::MOUSE_BUTTON_NONE;
    }
}

GameErrorCode InputHandler::Initialize(GameInputSystem* pInputSystem)
{
    if(pInputSystem == nullptr) {
	return FWG_E_INVALID_PARAMETER_ERROR;
    }
    m_pInputSystem = pInputSystem;
    return FWG_NO_ERROR;
}

void InputHandler::OnMouseMoved(const OIS::MouseState& arg)
{
    m_ctrlStruct.SetRelativeMousePosition(arg.X.rel, arg.Y.rel);
    m_ctrlStruct.SetAbsoluteMousePosition(arg.X.abs, arg.Y.abs);
}

void InputHandler::OnMousePressed(const OIS::MouseState& arg, OIS::MouseButtonID id)
{
    m_ctrlStruct.SetMouseButton(ConvertMouseButtonId(id));
}

void InputHandler::OnMouseReleased(const OIS::MouseState& arg, OIS::MouseButtonID id)
{
    m_ctrlStruct.ReleaseMouseButton(ConvertMouseButtonId(id));
}

void InputHandler::SetDefinition(InputDef* pDef)
{
	if(!m_spDefinition.IsEmpty())
	{
		// input component was already registered - unregister it
		m_pInputSystem->UnregisterCallback(this);
		m_pInputSystem->UnregisterCallbackClass(this);
	}
	
	m_spDefinition = pDef;
}

InputDef* InputHandler::GetDefinition() const 
{
	return m_spDefinition.In();
}
