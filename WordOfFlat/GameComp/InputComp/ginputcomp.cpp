#include "ginputcomp.h"
#include <wx/xml/xml.h>
#include <GameXmlDefinitions/gxmldefs.h>
#include <GameXmlDefinitions/gxmlkeydefs.h>
#include <GameXmlDefinitions/gxmlutils.h>

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

GameErrorCode InputComponent::Initialize(GameInputSystem* pInputSystem)
{
	if(pInputSystem == nullptr)
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	m_pInputSystem = pInputSystem;
	return FWG_NO_ERROR;
}


GameErrorCode InputComponent::ReceiveMessage(TaskMessage&)
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

GameErrorCode InputComponent::CreateInputDef(wxXmlNode* pNode, InputDef& output)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxInt32 keyCode = 0;
	wxString action;
	
	wxXmlNode *child = pNode->GetChildren();
	
	while (child)
	{
		if(child->GetName() == GAME_TAG_PARAM_INPUT_KEY) 
		{
			keyCode = 0;
			action.Clear(); // clear tempValue for sure
			
			if(FWG_FAILED(result = GetKeyValue(child, action, keyCode)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Read key attributes failed: 0x%08x"), m_pInputSystem->GetLogger(), result, FWGLOG_ENDVAL);
				return result;
			}
			
			if(FWG_FAILED(result = output.m_inputMap.Insert(action, keyCode)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Insert key action failed (action='%s' on line='%d'): 0x%08x"), m_pInputSystem->GetLogger()
										, action.GetData().AsInternal(), child->GetLineNumber(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else {
			// found unknown tag
			GameXmlUtils::ProcessUnknownTag(child, m_pInputSystem->GetLogger());
			return FWG_E_XML_UNKNOWN_TAG_ERROR;
		}
		
		child = child->GetNext();
		
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode InputComponent::GetKeyValue(wxXmlNode *pNode, wxString &action, wxInt32 &keyCode)
{
	wxString tempAction;
	wxString tempValue;
	long 	tempKeyCode = -1;

	// get value
	if(!pNode->GetAttribute(GAME_TAG_ATTR_ACTION, &tempAction))
	{
		FWGLOG_ERROR_FORMAT(wxT("'%s' attribut from tag '%s' is missing on line: %d"),
				m_pInputSystem->GetLogger(), GAME_TAG_ATTR_ACTION, pNode->GetName().GetData().AsInternal(), pNode->GetLineNumber(), FWGLOG_ENDVAL);
		return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
	}

	// get value
	if(!pNode->GetAttribute(GAME_TAG_ATTR_VALUE, &tempValue))
	{
		FWGLOG_ERROR_FORMAT(wxT("'%s' attribut from tag '%s' is missing on line: %d"),
				m_pInputSystem->GetLogger(), GAME_TAG_ATTR_VALUE, pNode->GetName().GetData().AsInternal(), pNode->GetLineNumber(), FWGLOG_ENDVAL);
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
				m_pInputSystem->GetLogger(), GAME_TAG_ATTR_VALUE, pNode->GetName().GetData().AsInternal(), pNode->GetLineNumber(), FWGLOG_ENDVAL);
			return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
		}
	}
	
	
	// swap string containers
	action.swap(tempAction);
	keyCode = tempKeyCode;
	
	
	return FWG_NO_ERROR;
}

GameErrorCode InputComponent::SetKeyValue(wxXmlNode* pParentNode, wxString& action, wxInt32& keyCode)
{
}
