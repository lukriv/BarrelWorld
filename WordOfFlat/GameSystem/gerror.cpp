#include "gerror.h"


void GameConvertErrCode2String(GameErrorCode errCode, wxString &errorStr)
{
	switch (errCode)
	{
	default:
		errorStr.assign(_T("Unknown Error"));
		break;
	}
}
