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
		
	default:
		errorStr.assign(_T("Unknown Error"));
		break;
	}
}
