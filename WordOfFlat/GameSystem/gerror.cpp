#include "gerror.h"


void GameConvertErrCode2String(GameErrorCode errCode, wxString &errorStr)
{
	switch (errCode)
	{
	case FWG_NO_ERROR:
		errorStr.assign(FWG_NO_ERROR_STR);
		break;
	case FWG_E_OBJECT_NOT_INITIALIZED_ERROR:
		errorStr.assign(FWG_E_OBJECT_NOT_INITIALIZED_ERROR_STR);
		break;
	case FWG_E_MEMORY_ALLOCATION_ERROR:
		errorStr.assign(FWG_E_MEMORY_ALLOCATION_ERROR_STR);
		break;
	case FWG_E_OPEN_FILE_ERROR:
		errorStr.assign(FWG_E_OPEN_FILE_ERROR_STR);
		break;
	case FWG_E_NOT_IMPLEMENTED_ERROR:
		errorStr.assign(FWG_E_NOT_IMPLEMENTED_ERROR_STR);
		break;
	case FWG_E_FULL_BUFFER_ERROR:
		errorStr.assign(FWG_E_FULL_BUFFER_ERROR_STR);
		break;
	case FWG_E_THREAD_ALREADY_RUNNING_ERROR:
		errorStr.assign(FWG_E_THREAD_ALREADY_RUNNING_ERROR_STR);
		break;
	case FWG_E_THREAD_NOT_RUNNING_ERROR:
		errorStr.assign(FWG_E_THREAD_NOT_RUNNING_ERROR_STR);
		break;
	case FWG_E_THREAD_ALREADY_KILLED_ERROR:
		errorStr.assign(FWG_E_THREAD_ALREADY_KILLED_ERROR_STR);
		break;
	case FWG_E_NO_RESOURCES_ERROR:
		errorStr.assign(FWG_E_NO_RESOURCES_ERROR_STR);
		break;
	case FWG_E_MISC_ERROR:
		errorStr.assign(FWG_E_MISC_ERROR_STR);
		break;
	case FWG_E_TIMEOUT_WARNING:
		errorStr.assign(FWG_E_TIMEOUT_WARNING_STR);
		break;
	case FWG_E_UNKNOWN_TARGET_ERROR:
		errorStr.assign(FWG_E_UNKNOWN_TARGET_ERROR_STR);
		break;
	case FWG_E_STREAM_EOF_ERROR:
		errorStr.assign(FWG_E_STREAM_EOF_ERROR_STR);
		break;
	case FWG_E_STREAM_WRITE_ERROR:
		errorStr.assign(FWG_E_STREAM_WRITE_ERROR_STR);
		break;
	case FWG_E_STREAM_READ_ERROR:
		errorStr.assign(FWG_E_STREAM_READ_ERROR_STR);
		break;
	case FWG_E_PORT_ERROR:
		errorStr.assign(FWG_E_PORT_ERROR_STR);
		break;
	case FWG_E_INVALID_OPERATION_ERROR:
		errorStr.assign(FWG_E_INVALID_OPERATION_ERROR_STR);
		break;
	case FWG_E_IO_ERROR:
		errorStr.assign(FWG_E_IO_ERROR_STR);
		break;
	case FWG_E_INVALID_ADDRESS_ERROR:
		errorStr.assign(FWG_E_INVALID_ADDRESS_ERROR_STR);
		break;
	case FWG_E_INVALID_SOCKET_ERROR:
		errorStr.assign(FWG_E_INVALID_SOCKET_ERROR_STR);
		break;
	case FWG_E_NOHOST_ERROR:
		errorStr.assign(FWG_E_NOHOST_ERROR_STR);
		break;
	case FWG_E_INVALID_PORT_ERROR:
		errorStr.assign(FWG_E_INVALID_PORT_ERROR_STR);
		break;
	case FWG_E_WOULD_BLOCK_WARNING:
		errorStr.assign(FWG_E_WOULD_BLOCK_WARNING_STR);
		break;
	case FWG_E_MEMORY_ERROR:
		errorStr.assign(FWG_E_MEMORY_ERROR_STR);
		break;
	case FWG_E_INVALID_PARAMETER_ERROR:
		errorStr.assign(FWG_E_INVALID_PARAMETER_ERROR_STR);
		break;
	case FWG_E_DISCONNECTED_ERROR:
		errorStr.assign(FWG_E_DISCONNECTED_ERROR_STR);
		break;
	case FWG_E_INVALID_INDEX_ERROR:
		errorStr.assign(FWG_E_INVALID_INDEX_ERROR_STR);
		break;
	case FWG_E_OBJECT_NOT_FOUND_ERROR:
		errorStr.assign(FWG_E_OBJECT_NOT_FOUND_ERROR_STR);
		break;
	case FWG_E_ANIMATION_ENDS_INFORMATION:
		errorStr.assign(FWG_E_ANIMATION_ENDS_INFORMATION_STR);
		break;
	case FWG_E_END_OF_SEQUENCE_INFORMATION:
		errorStr.assign(FWG_E_END_OF_SEQUENCE_INFORMATION_STR);
		break;
	case FWG_E_OBJECT_ALREADY_EXISTS_ERROR:
		errorStr.assign(FWG_E_OBJECT_ALREADY_EXISTS_ERROR_STR);
		break;
	case FWG_E_RENDER_OBJECT_ALREADY_ATTACHED_ERROR:
		errorStr.assign(FWG_E_RENDER_OBJECT_ALREADY_ATTACHED_ERROR_STR);
		break;
	case FWG_E_OBJECT_NOT_EXIST_ERROR:
		errorStr.assign(FWG_E_OBJECT_NOT_EXIST_ERROR_STR);
		break;
	case FWG_E_XML_ROOT_NOT_FOUND_ERROR:
		errorStr.assign(FWG_E_XML_ROOT_NOT_FOUND_ERROR_STR);
		break;
	case FWG_E_XML_UNKNOWN_TAG_ERROR:
		errorStr.assign(FWG_E_XML_UNKNOWN_TAG_ERROR_STR);
		break;
	case FWG_E_XML_INVALID_TAG_ERROR:
		errorStr.assign(FWG_E_XML_INVALID_TAG_ERROR_STR);
		break;
	case FWG_E_XML_INVALID_ATTR_ERROR:
		errorStr.assign(FWG_E_XML_INVALID_ATTR_ERROR_STR);
		break;
	case FWG_E_XML_TAG_NOT_FOUND_ERROR:
		errorStr.assign(FWG_E_XML_TAG_NOT_FOUND_ERROR_STR);
		break;
	case FWG_E_XML_ATTR_NOT_FOUND_ERROR:
		errorStr.assign(FWG_E_XML_ATTR_NOT_FOUND_ERROR_STR);
		break;
	case FWG_E_MENU_SYSTEM_ERROR:
		errorStr.assign(FWG_E_MENU_SYSTEM_ERROR_STR);
		break;
	case FWG_E_RENDER_SYSTEM_ERROR:
		errorStr.assign(FWG_E_RENDER_SYSTEM_ERROR_STR);
		break;
	case FWG_E_BAD_CONVERSION_ERROR:
		errorStr.assign(FWG_E_BAD_CONVERSION_ERROR_STR);
		break;
	default:
		errorStr.assign(_T("Unknown Error"));
		break;
	}
}
