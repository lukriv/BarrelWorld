#ifndef __GAME_XML_UTILS_H__
#define __GAME_XML_UTILS_H__

#include <wx/defs.h>
#include <GameSystem/gerror.h>

namespace Ogre {
	class Vector3;
	class Quaternion;
};

class wxString;
class wxXmlNode;
class GameLogger;

class GameXmlUtils
{

public:
	static GameErrorCode ConvertToVec3(const wxString& input, Ogre::Vector3& resultVec);
	static GameErrorCode ConvertToQuat(const wxString& input, Ogre::Quaternion& resultQuat);
	static GameErrorCode ConvertToInt32(const wxString& input, wxInt32& output);
	static GameErrorCode ConvertToDword(const wxString& input, wxDword& output);
	static GameErrorCode ConvertToFloat(const wxString& input, float& output);
	static GameErrorCode ConvertToKeyCode(const wxString& input, wxInt32& output);
	
	static GameErrorCode ConvertFromVec3(const Ogre::Vector3& input, wxString& output);
	static GameErrorCode ConvertFromQuat(const Ogre::Quaternion& input, wxString& output);
	static GameErrorCode ConvertFromInt32( wxInt32 input, wxString& output);
	static GameErrorCode ConvertFromDword( wxDword input, wxString& output);
	static GameErrorCode ConvertFromFloat( float input, wxString& output);
	static GameErrorCode ConvertFromKeyCode( wxInt32 input, wxString& output);
	
	
	
	static GameErrorCode GetNodeContent(wxXmlNode *pNode, wxString& content, GameLogger *pLogger = nullptr);
	static GameErrorCode GetKeyValue(wxXmlNode *pNode, wxString &action, wxInt32 &keyCode, GameLogger *pLogger = nullptr);
	
	/**
	 * @brief Check if tag has correct name and correct type
	 * @param pNode Node to check
	 * @param tag Tag name
	 * @param type Type name
	 * 
	 * @return Returns FWG_NO_ERROR if everything is ok, or error if something is wrong
	 */
	static GameErrorCode CheckTagAndType(wxXmlNode *pNode, const wxString &tag, const wxString &type, GameLogger* pLogger = nullptr);
	
	static void ProcessUnknownTag(wxXmlNode *pNode, GameLogger *pLogger);

};


#endif //__GAME_XML_LOADER_H__