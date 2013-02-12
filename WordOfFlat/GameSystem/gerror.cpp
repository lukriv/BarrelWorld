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
	default:
		errorStr.assign(_T("Unknown Error"));
		break;
	}
}
