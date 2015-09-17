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

GameErrorCode InputComponent::GetKeyValue(wxXmlNode *pNode, wxString &action, wxInt32 &keyCode, GameLogger *pLogger)
{
	wxString tempAction;
	wxString tempValue;
	long 	tempKeyCode = -1;

	// get value
	if(!pNode->GetAttribute(GAME_ATTR_ACTION_STR, &tempAction))
	{
		FWGLOG_ERROR_FORMAT(wxT("'%s' attribut from tag '%s' is missing on line: %d"),
				m_spLogger, GAME_ATTR_ACTION_STR, pNode->GetName().GetData().AsInternal(), pNode->GetLineNumber(), FWGLOG_ENDVAL);
		return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
	}

	// get value
	if(!pNode->GetAttribute(GAME_ATTR_VALUE_STR, &tempValue))
	{
		FWGLOG_ERROR_FORMAT(wxT("'%s' attribut from tag '%s' is missing on line: %d"),
				m_spLogger, GAME_ATTR_VALUE_STR, pNode->GetName().GetData().AsInternal(), pNode->GetLineNumber(), FWGLOG_ENDVAL);
		return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
	}
	
	// found keycode for stringValue
	// check if value is not direct keycode
	if(!tempValue.ToLong(&tempKeyCode))
	{
		tempKeyCode = -1; // reset keycode
		for(wxDword i = 0; i < WXSIZEOF(KeyTable); i++)
		{
			if(tempValue.Cmp(KeyTable[i].m_keyName) == 0)
			{
				tempKeyCode = KeyTable[i].m_keyCode;
				break;
			}
		}
		
		// check if search was successful
		if(tempKeyCode == -1)
		{
			FWGLOG_ERROR_FORMAT(wxT("'%s' attribut from tag '%s' has wrong format on line: %d"),
				m_spLogger, GAME_ATTR_VALUE_STR, pNode->GetName().GetData().AsInternal(), pNode->GetLineNumber(), FWGLOG_ENDVAL);
			return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
		}
	}
	
	
	// swap string containers
	action.swap(tempAction);
	keyCode = tempKeyCode;
	
	
	return FWG_NO_ERROR;
}
