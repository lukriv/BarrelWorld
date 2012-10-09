#ifndef __GAME_ERROR_H__
#define __GAME_ERROR_H__

#include <wx/defs.h>

typedef wxInt32 GameErrorCode;

void GameConvertErrCode2String(GameErrorCode errCode, wxString &errorStr);

#endif //__GAME_ERROR_H__