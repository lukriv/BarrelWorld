#include "ginputcomp.h"

static ControlStruct::MouseButtonState ConvertMouseButtonId(OIS::MouseButtonID id)
{
	switch(id)
	{
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


GameErrorCode InputComponent::ReceiveMessage(TaskMessage&)
{
    return FWG_NO_ERROR;
}

GameErrorCode InputComponent::Update()
{
    return FWG_NO_ERROR;
}

void InputComponent::OnMouseMoved(const OIS::MouseState& arg)
{
	m_ctrlStruct.SetRelativeMousePosition(arg.X.rel, arg.Y.rel);
	m_ctrlStruct.SetAbsoluteMousePosition(arg.X.abs, arg.Y.abs);
}

void InputComponent::OnMousePressed(const OIS::MouseState& arg, OIS::MouseButtonID id)
{
	m_ctrlStruct.SetMouseButton(ConvertMouseButtonId(id));
}

void InputComponent::OnMouseReleased(const OIS::MouseState& arg, OIS::MouseButtonID id)
{
	m_ctrlStruct.ReleaseMouseButton(ConvertMouseButtonId(id));
}

