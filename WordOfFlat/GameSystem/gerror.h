#ifndef __GAME_ERROR_H__
#define __GAME_ERROR_H__

#include <wx/defs.h>
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

#define FWG_NO_ERROR 0x00000000
static const wxChar* FWG_NO_ERROR_STR = wxT("FWG_NO_ERROR");

#define FWG_E_OBJECT_NOT_INITIALIZED_ERROR 0xE0000001
static const wxChar* FWG_E_OBJECT_NOT_INITIALIZED_ERROR_STR = wxT("FWG_E_OBJECT_NOT_INITIALIZED_ERROR");

#define FWG_E_MEMORY_ALLOCATION_ERROR 0xE0000002
static const wxChar* FWG_E_MEMORY_ALLOCATION_ERROR_STR = wxT("FWG_E_MEMORY_ALLOCATION_ERROR");

#define FWG_E_OPEN_FILE_ERROR 0xE0000003
static const wxChar* FWG_E_OPEN_FILE_ERROR_STR = wxT("FWG_E_OPEN_FILE_ERROR");

#define FWG_E_NOT_IMPLEMENTED_ERROR 0xE0000004
static const wxChar* FWG_E_NOT_IMPLEMENTED_ERROR_STR = wxT("FWG_E_NOT_IMPLEMENTED_ERROR");

#define FWG_E_FULL_BUFFER_ERROR 0xE0000005
static const wxChar* FWG_E_FULL_BUFFER_ERROR_STR = wxT("FWG_E_FULL_BUFFER_ERROR");

#define FWG_E_THREAD_ALREADY_RUNNING_ERROR 0xE0000006
static const wxChar* FWG_E_THREAD_ALREADY_RUNNING_ERROR_STR = wxT("FWG_E_THREAD_ALREADY_RUNNING_ERROR");

#define FWG_E_THREAD_NOT_RUNNING_ERROR 0xE0000007
static const wxChar* FWG_E_THREAD_NOT_RUNNING_ERROR_STR = wxT("FWG_E_THREAD_NOT_RUNNING_ERROR");

#define FWG_E_THREAD_ALREADY_KILLED_ERROR 0xE0000008
static const wxChar* FWG_E_THREAD_ALREADY_KILLED_ERROR_STR = wxT("FWG_E_THREAD_ALREADY_KILLED_ERROR");

#define FWG_E_NO_RESOURCES_ERROR 0xE0000009
static const wxChar* FWG_E_NO_RESOURCES_ERROR_STR = wxT("FWG_E_NO_RESOURCES_ERROR");

#define FWG_E_MISC_ERROR 0xE000000a
static const wxChar* FWG_E_MISC_ERROR_STR = wxT("FWG_E_MISC_ERROR");

#define FWG_E_TIMEOUT_WARNING 0xA000000b
static const wxChar* FWG_E_TIMEOUT_WARNING_STR = wxT("FWG_E_TIMEOUT_WARNING");

#define FWG_E_UNKNOWN_TARGET_ERROR 0xE000000c
static const wxChar* FWG_E_UNKNOWN_TARGET_ERROR_STR = wxT("FWG_E_UNKNOWN_TARGET_ERROR");



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