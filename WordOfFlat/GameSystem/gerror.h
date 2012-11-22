#ifndef __GAME_ERROR_H__
#define __GAME_ERROR_H__

#include <wx/defs.h>
#include <wx/string.h>

typedef wxInt32 GameErrorCode;

#define FWG_NO_ERROR 0x00000000
static const wxChar* FWG_NO_ERROR_STR = wxT("FWG_NO_ERROR");

#define FWG_E_OBJECT_NOT_INITIALIZED_ERROR 0xE0000001
static const wxChar* FWG_E_OBJECT_NOT_INITIALIZED_ERROR_STR = wxT("FWG_E_OBJECT_NOT_INITIALIZED_ERROR");

#define FWG_E_MEMORY_ALLOCATION_ERROR 0xE0000002
static const wxChar* FWG_E_MEMORY_ALLOCATION_ERROR_STR = wxT("FWG_E_MEMORY_ALLOCATION_ERROR");


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