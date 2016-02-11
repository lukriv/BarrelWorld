#include "gxmlutils.h"


#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/tokenzr.h>
#include <wx/xml/xml.h>
#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>
#include <GameSystem/glog.h>
#include <GameXmlDefinitions/gxmldefs.h>
#include "gxmlkeydefs.h"

///////////////////////////////////////////////
// ConvertTo
///////////////////////////////////////////////
GameErrorCode GameXmlUtils::ConvertToDword(const wxString& input, wxDword& output)
{
	unsigned long value = 0;
	if(!input.ToULong(&value))
	{
		return FWG_E_BAD_CONVERSION_ERROR;
	}
	
	output = value;
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlUtils::ConvertToFloat(const wxString& input, float& output)
{
	double value = 0.0;
	if(!input.ToDouble(&value))
	{
		return FWG_E_BAD_CONVERSION_ERROR;
	}
	
	output = static_cast<float>(value);
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlUtils::ConvertToInt32(const wxString& input, wxInt32& output)
{
	long value = 0;
	if(!input.ToLong(&value))
	{
		return FWG_E_BAD_CONVERSION_ERROR;
	}
	
	output = value;
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlUtils::ConvertToQuat(const wxString& input, Ogre::Quaternion& resultQuat)
{
	wxArrayString arrStr = wxStringTokenize (input, wxString(wxT(",;")), wxTOKEN_RET_EMPTY_ALL);
	double dTemp = 0;
	Ogre::Quaternion tempQuat;
	
	// check count
	if(arrStr.GetCount() != 4)
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	for ( wxInt32 i = 0; i < 4; i++)
	{
		if(!arrStr.Item(i).ToDouble(&dTemp))
		{
			return FWG_E_BAD_CONVERSION_ERROR;
		}
		
		tempQuat[i] = static_cast<Ogre::Real>(dTemp);
		
	}
	
	resultQuat = tempQuat;
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlUtils::ConvertToVec3(const wxString& input, Ogre::Vector3& resultVec)
{
	wxArrayString arrStr = wxStringTokenize (input, wxString(wxT(",;")), wxTOKEN_RET_EMPTY_ALL);
	double dTemp = 0;
	Ogre::Vector3 tempVec;
	
	// check count
	if(arrStr.GetCount() != 3)
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	for ( wxInt32 i = 0; i < 3; i++)
	{
		if(!arrStr.Item(i).ToDouble(&dTemp))
		{
			return FWG_E_BAD_CONVERSION_ERROR;
		}
		
		tempVec[i] = static_cast<Ogre::Real>(dTemp);
		
	}
	
	resultVec = tempVec;
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlUtils::ConvertToKeyCode(const wxString& input, wxInt32& output)
{
	for(wxDword i = 0; i < WXSIZEOF(KeyTable); ++i)
	{
		if(input.IsSameAs(KeyTable[i].m_keyName))
		{
			output = KeyTable[i].m_keyCode;
			return FWG_NO_ERROR;
		}
	}
	
	return FWG_E_OBJECT_NOT_FOUND_ERROR;	
}



///////////////////////////////////////////////
// ConvertFrom
///////////////////////////////////////////////
GameErrorCode GameXmlUtils::ConvertFromDword(wxDword input, wxString& output)
{
	output.assign(wxString::Format(wxT("%lu"), input));
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlUtils::ConvertFromFloat(float input, wxString& output)
{
	double temp = static_cast<double>(input);
	output.assign(wxString::FromDouble(temp, 5));
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlUtils::ConvertFromInt32(wxInt32 input, wxString& output)
{
	output.assign(wxString::Format(wxT("%ld"), input));
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlUtils::ConvertFromQuat(const Ogre::Quaternion& input, wxString& output)
{
	output.assign(wxString::Format(wxT("%.5f,%.5f,%.5f,%.5f"), input.x, input.y, input.z, input.w));
	return FWG_NO_ERROR;	
}

GameErrorCode GameXmlUtils::ConvertFromVec3(const Ogre::Vector3& input, wxString& output)
{
	output.assign(wxString::Format(wxT("%.5f,%.5f,%.5f"), input.x, input.y, input.z));
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlUtils::ConvertFromKeyCode(wxInt32 input, wxString& output)
{
	for(wxDword i = 0; i < WXSIZEOF(KeyTable); ++i)
	{
		if(input == KeyTable[i].m_keyCode)
		{
			output = KeyTable[i].m_keyName;
			return FWG_NO_ERROR;
		}
	}
	
	return FWG_E_OBJECT_NOT_FOUND_ERROR;
}

///////////////////////////////////////////////
// Other
///////////////////////////////////////////////
GameErrorCode GameXmlUtils::GetNodeContent(wxXmlNode* pNode, wxString& content, GameLogger* pLogger)
{
	wxXmlNode* pChild = pNode->GetChildren();
	if ( (pChild == nullptr) || (pChild->GetType() != wxXML_TEXT_NODE) )
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag with name '%s' has no text content on line: %d"),
						pLogger, pNode->GetName().GetData().AsInternal(), pNode->GetLineNumber(), FWGLOG_ENDVAL);
		return FWG_E_XML_INVALID_TAG_ERROR;
	} else {
		content = pChild->GetContent();
	}
	
	return FWG_NO_ERROR;
}



void GameXmlUtils::ProcessUnknownTag(wxXmlNode* pNode, GameLogger* pLogger)
{
	if(pNode->GetType() != wxXML_COMMENT_NODE)
	{
		FWGLOG_INFO_FORMAT(wxT("Tag '%s' with type '%d' on line '%d' is not known"), pLogger
																, pNode->GetName().GetData().AsInternal()
																, (int)pNode->GetType()
																, pNode->GetLineNumber()
																, FWGLOG_ENDVAL);
	}
}

GameErrorCode GameXmlUtils::CheckTagAndType(wxXmlNode* pNode, const wxString& tag, const wxString& type, GameLogger* pLogger)
{
	wxString typeValue;
	if(pNode->GetName() != tag)
	{
		FWGLOG_ERROR_FORMAT(wxT("Invalid tag name '%s' (expected '%s') on line '%d'"), pLogger
																, pNode->GetName().GetData().AsInternal()
																, tag.GetData().AsInternal()
																, pNode->GetLineNumber()
																, FWGLOG_ENDVAL);
	
		return FWG_E_XML_INVALID_TAG_ERROR;
	}
	
	if(!pNode->GetAttribute(GAME_TAG_ATTR_TYPE, &typeValue))
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' does not contain attribute '%s' on line '%d'"), pLogger
																, pNode->GetName().GetData().AsInternal()
																, GAME_TAG_ATTR_TYPE
																, pNode->GetLineNumber()
																, FWGLOG_ENDVAL);
		return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
	}
	
	if(!typeValue.IsSameAs(type))
	{
		FWGLOG_ERROR_FORMAT(wxT("Unexpected type '%s' (expected '%s') within tag '%s' on line '%d'"), pLogger
																, typeValue.GetData().AsInternal()
																, type.GetData().AsInternal()
																, pNode->GetName().GetData().AsInternal()
																, pNode->GetLineNumber()
																, FWGLOG_ENDVAL);
		return FWG_E_XML_INVALID_ATTR_ERROR;
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlUtils::GetKeyValue(wxXmlNode* pNode, wxString& action, wxInt32& keyCode, GameLogger* pLogger)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxString keyCodeStr;
	if(!pNode->GetName().IsSameAs(GAME_TAG_PARAM_INPUT_KEY))
	{
		FWGLOG_ERROR_FORMAT(wxT("Invalid tag name '%s' (expected '%s') on line '%d'"), pLogger
																, pNode->GetName().GetData().AsInternal()
																, GAME_TAG_PARAM_INPUT_KEY
																, pNode->GetLineNumber()
																, FWGLOG_ENDVAL);
	
		return FWG_E_XML_INVALID_TAG_ERROR;
	}
	
	if(!pNode->GetAttribute(GAME_TAG_ATTR_ACTION, &action))
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' does not contain attribute '%s' on line '%d'"), pLogger
																, pNode->GetName().GetData().AsInternal()
																, GAME_TAG_ATTR_ACTION
																, pNode->GetLineNumber()
																, FWGLOG_ENDVAL);
		return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
	}
	
	if(!pNode->GetAttribute(GAME_TAG_ATTR_VALUE, &keyCodeStr))
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' does not contain attribute '%s' on line '%d'"), pLogger
																, pNode->GetName().GetData().AsInternal()
																, GAME_TAG_ATTR_VALUE
																, pNode->GetLineNumber()
																, FWGLOG_ENDVAL);
		return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
	}
	
	if(FWG_FAILED(result = ConvertToKeyCode(keyCodeStr, keyCode)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Tag '%s' does not contain correct key value on line '%d'"), pLogger
																, pNode->GetName().GetData().AsInternal()
																, pNode->GetLineNumber()
																, FWGLOG_ENDVAL);
		return result;
	}
	
	return result;
}


