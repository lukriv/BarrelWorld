#ifndef __GAME_ERROR_H__
#define __GAME_ERROR_H__

#include "gdefs.h"
#include <wx/string.h>

typedef wxInt32 GameErrorCode;

/*! \brief FWG_FAILED returns false when some error or warning occured
 * 
 * Use at this manner:
 * if(FWG_FAILED(result))
 * {
 * 		...ErrorCodeHandling...
 * }
 */ 
#define FWG_FAILED(x) (0>(x))

/*! \brief FWG_SUCCEDED returns false when no error or warning occured
 * 
 * Use at this manner:
 * if(FWG_SUCCEDED(result))
 * {
 * 		...when succed do this...
 * }
 */
#define FWG_SUCCEDED(x) (0<=(x))

#define FWG_RETURN_FAIL(x) 	\
	{\
		GameErrorCode resultErrcode = (x);\
		if (FWG_FAILED(resultErrcode))\
		{\
			return resultErrcode;\
		}\
	}


/* template error 
#define FWG_E_NAME_ERROR 0xE0000000
FWG_UNUSED(static const wxChar* FWG_E_NAME_ERROR_STR) = wxT("FWG_E_NAME_ERROR");
*/

#define FWG_NO_ERROR 0x00000000
FWG_UNUSED(static const wxChar* FWG_NO_ERROR_STR) = wxT("FWG_NO_ERROR");
//FWG_UNUSED(FWG_NO_ERROR_STR);

#define FWG_E_OBJECT_NOT_INITIALIZED_ERROR 0xE0000001
FWG_UNUSED(static const wxChar* FWG_E_OBJECT_NOT_INITIALIZED_ERROR_STR) = wxT("FWG_E_OBJECT_NOT_INITIALIZED_ERROR");

#define FWG_E_MEMORY_ALLOCATION_ERROR 0xE0000002
FWG_UNUSED(static const wxChar* FWG_E_MEMORY_ALLOCATION_ERROR_STR) = wxT("FWG_E_MEMORY_ALLOCATION_ERROR");

#define FWG_E_OPEN_FILE_ERROR 0xE0000003
FWG_UNUSED(static const wxChar* FWG_E_OPEN_FILE_ERROR_STR) = wxT("FWG_E_OPEN_FILE_ERROR");

#define FWG_E_NOT_IMPLEMENTED_ERROR 0xE0000004
FWG_UNUSED(static const wxChar* FWG_E_NOT_IMPLEMENTED_ERROR_STR) = wxT("FWG_E_NOT_IMPLEMENTED_ERROR");

#define FWG_E_FULL_BUFFER_ERROR 0xE0000005
FWG_UNUSED(static const wxChar* FWG_E_FULL_BUFFER_ERROR_STR) = wxT("FWG_E_FULL_BUFFER_ERROR");

#define FWG_E_THREAD_ALREADY_RUNNING_ERROR 0xE0000006
FWG_UNUSED(static const wxChar* FWG_E_THREAD_ALREADY_RUNNING_ERROR_STR) = wxT("FWG_E_THREAD_ALREADY_RUNNING_ERROR");

#define FWG_E_THREAD_NOT_RUNNING_ERROR 0xE0000007
FWG_UNUSED(static const wxChar* FWG_E_THREAD_NOT_RUNNING_ERROR_STR) = wxT("FWG_E_THREAD_NOT_RUNNING_ERROR");

#define FWG_E_THREAD_ALREADY_KILLED_ERROR 0xE0000008
FWG_UNUSED(static const wxChar* FWG_E_THREAD_ALREADY_KILLED_ERROR_STR) = wxT("FWG_E_THREAD_ALREADY_KILLED_ERROR");

#define FWG_E_NO_RESOURCES_ERROR 0xE0000009
FWG_UNUSED(static const wxChar* FWG_E_NO_RESOURCES_ERROR_STR) = wxT("FWG_E_NO_RESOURCES_ERROR");

#define FWG_E_MISC_ERROR 0xE000000a
FWG_UNUSED(static const wxChar* FWG_E_MISC_ERROR_STR) = wxT("FWG_E_MISC_ERROR");

#define FWG_E_TIMEOUT_WARNING 0xA000000b
FWG_UNUSED(static const wxChar* FWG_E_TIMEOUT_WARNING_STR) = wxT("FWG_E_TIMEOUT_WARNING");

#define FWG_E_UNKNOWN_TARGET_ERROR 0xE000000c
FWG_UNUSED(static const wxChar* FWG_E_UNKNOWN_TARGET_ERROR_STR) = wxT("FWG_E_UNKNOWN_TARGET_ERROR");

#define FWG_E_STREAM_EOF_ERROR 0xE000000d
FWG_UNUSED(static const wxChar* FWG_E_STREAM_EOF_ERROR_STR) = wxT("FWG_E_STREAM_EOF_ERROR");

#define FWG_E_STREAM_WRITE_ERROR 0xE000000e
FWG_UNUSED(static const wxChar* FWG_E_STREAM_WRITE_ERROR_STR) = wxT("FWG_E_STREAM_WRITE_ERROR");

#define FWG_E_STREAM_READ_ERROR 0xE000000f
FWG_UNUSED(static const wxChar* FWG_E_STREAM_READ_ERROR_STR) = wxT("FWG_E_STREAM_READ_ERROR");

#define FWG_E_PORT_ERROR 0xE0000010
FWG_UNUSED(static const wxChar* FWG_E_PORT_ERROR_STR) = wxT("FWG_E_PORT_ERROR");

#define FWG_E_INVALID_OPERATION_ERROR 0xE0000011
FWG_UNUSED(static const wxChar* FWG_E_INVALID_OPERATION_ERROR_STR) = wxT("FWG_E_INVALID_OPERATION_ERROR");

#define FWG_E_IO_ERROR 0xE0000012
FWG_UNUSED(static const wxChar* FWG_E_IO_ERROR_STR) = wxT("FWG_E_IO_ERROR");

#define FWG_E_INVALID_ADDRESS_ERROR 0xE0000013
FWG_UNUSED(static const wxChar* FWG_E_INVALID_ADDRESS_ERROR_STR) = wxT("FWG_E_INVALID_ADDRESS_ERROR");

#define FWG_E_INVALID_SOCKET_ERROR 0xE0000014
FWG_UNUSED(static const wxChar* FWG_E_INVALID_SOCKET_ERROR_STR) = wxT("FWG_E_INVALID_SOCKET_ERROR");

#define FWG_E_NOHOST_ERROR 0xE0000015
FWG_UNUSED(static const wxChar* FWG_E_NOHOST_ERROR_STR) = wxT("FWG_E_NOHOST_ERROR");

#define FWG_E_INVALID_PORT_ERROR 0xE0000016
FWG_UNUSED(static const wxChar* FWG_E_INVALID_PORT_ERROR_STR) = wxT("FWG_E_INVALID_PORT_ERROR");

#define FWG_E_WOULD_BLOCK_WARNING 0xA0000017
FWG_UNUSED(static const wxChar* FWG_E_WOULD_BLOCK_WARNING_STR) = wxT("FWG_E_WOULD_BLOCK_WARNING");

#define FWG_E_MEMORY_ERROR 0xE0000018
FWG_UNUSED(static const wxChar* FWG_E_MEMORY_ERROR_STR) = wxT("FWG_E_MEMORY_ERROR");

#define FWG_E_INVALID_PARAMETER_ERROR 0xE0000019
FWG_UNUSED(static const wxChar* FWG_E_INVALID_PARAMETER_ERROR_STR) = wxT("FWG_E_INVALID_PARAMETER_ERROR");

#define FWG_E_DISCONNECTED_ERROR 0xE000001a
FWG_UNUSED(static const wxChar* FWG_E_DISCONNECTED_ERROR_STR) = wxT("FWG_E_DISCONNECTED_ERROR");

#define FWG_E_INVALID_INDEX_ERROR 0xE000001b
FWG_UNUSED(static const wxChar* FWG_E_INVALID_INDEX_ERROR_STR) = wxT("FWG_E_INVALID_INDEX_ERROR");

#define FWG_E_OBJECT_NOT_FOUND_ERROR 0xE000001c
FWG_UNUSED(static const wxChar* FWG_E_OBJECT_NOT_FOUND_ERROR_STR) = wxT("FWG_E_OBJECT_NOT_FOUND_ERROR");

#define FWG_E_ANIMATION_ENDS_INFORMATION 0x6000001d
FWG_UNUSED(static const wxChar* FWG_E_ANIMATION_ENDS_INFORMATION_STR) = wxT("FWG_E_ANIMATION_ENDS_INFORMATION");

#define FWG_E_END_OF_SEQUENCE_INFORMATION 0x6000001e
FWG_UNUSED(static const wxChar* FWG_E_END_OF_SEQUENCE_INFORMATION_STR) = wxT("FWG_E_END_OF_SEQUENCE_INFORMATION");

#define FWG_E_OBJECT_ALREADY_EXISTS_ERROR 0xE000001f
FWG_UNUSED(static const wxChar* FWG_E_OBJECT_ALREADY_EXISTS_ERROR_STR) = wxT("FWG_E_OBJECT_ALREADY_EXISTS_ERROR");

#define FWG_E_RENDER_OBJECT_ALREADY_ATTACHED_ERROR 0xE0000020
FWG_UNUSED(static const wxChar* FWG_E_RENDER_OBJECT_ALREADY_ATTACHED_ERROR_STR) = wxT("FWG_E_RENDER_OBJECT_ALREADY_ATTACHED_ERROR");

#define FWG_E_OBJECT_NOT_EXIST_ERROR 0xE0000021
FWG_UNUSED(static const wxChar* FWG_E_OBJECT_NOT_EXIST_ERROR_STR) = wxT("FWG_E_OBJECT_NOT_EXIST_ERROR");

#define FWG_E_XML_ROOT_NOT_FOUND_ERROR 0xE0000022
FWG_UNUSED(static const wxChar* FWG_E_XML_ROOT_NOT_FOUND_ERROR_STR) = wxT("FWG_E_XML_ROOT_NOT_FOUND_ERROR");

#define FWG_E_XML_UNKNOWN_TAG_ERROR 0xE0000023
FWG_UNUSED(static const wxChar* FWG_E_XML_UNKNOWN_TAG_ERROR_STR) = wxT("FWG_E_XML_UNKNOWN_TAG_ERROR");

#define FWG_E_XML_INVALID_TAG_ERROR 0xE0000024
FWG_UNUSED(static const wxChar* FWG_E_XML_INVALID_TAG_ERROR_STR) = wxT("FWG_E_XML_INVALID_TAG_ERROR");

#define FWG_E_XML_INVALID_ATTR_ERROR 0xE0000025
FWG_UNUSED(static const wxChar* FWG_E_XML_INVALID_ATTR_ERROR_STR) = wxT("FWG_E_XML_INVALID_ATTR_ERROR_ERROR");

#define FWG_E_XML_TAG_NOT_FOUND_ERROR 0xE0000026
FWG_UNUSED(static const wxChar* FWG_E_XML_TAG_NOT_FOUND_ERROR_STR) = wxT("FWG_E_XML_TAG_NOT_FOUND_ERROR");

#define FWG_E_XML_ATTR_NOT_FOUND_ERROR 0xE0000027
FWG_UNUSED(static const wxChar* FWG_E_XML_ATTR_NOT_FOUND_ERROR_STR) = wxT("FWG_E_XML_ATTR_NOT_FOUND_ERROR");

#define FWG_E_MENU_SYSTEM_ERROR 0xE0000028
FWG_UNUSED(static const wxChar* FWG_E_MENU_SYSTEM_ERROR_STR) = wxT("FWG_E_MENU_SYSTEM_ERROR");

#define FWG_E_RENDER_SYSTEM_ERROR 0xE0000029
FWG_UNUSED(static const wxChar* FWG_E_RENDER_SYSTEM_ERROR_STR) = wxT("FWG_E_RENDER_SYSTEM_ERROR");

#define FWG_E_BAD_CONVERSION_ERROR 0xE000002a
FWG_UNUSED(static const wxChar* FWG_E_BAD_CONVERSION_ERROR_STR) = wxT("FWG_E_BAD_CONVERSION_ERROR");

#define FWG_E_INVALID_DATA_ERROR 0xE000002b
FWG_UNUSED(static const wxChar* FWG_E_INVALID_DATA_ERROR_STR) = wxT("FWG_E_INVALID_DATA_ERROR");


/*!
 * \brief Converts error code to error code string
 * 
 * It could be useful for error string convertor or for logger.
 * 
 * \param[in] errCode Error code
 * \param[out] errorStr Returned string
 */
void GameConvertErrCode2String(GameErrorCode errCode, wxString &errorStr);

#endif //__GAME_ERROR_H__